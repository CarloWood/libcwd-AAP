# Existing libcwd location-cache storage overview

This note maps the current address-to-location cache data structures and the entry points that reach them. Paths are relative to `$REPOROOT`.

## Executive summary

The current cache is two-level:

```text
libcwd::dwarf::objfiles_ct::ST_list_instance
  stores placement-constructed std::list<objfiles_ct*>
    each element is actually a dwarf::objfile_ct*
      each objfile_ct owns M_function_symbols
        type std::set<symbol_ct, symbol_key_greater>
        each symbol_ct represents one function address range plus DWARF/name metadata
```

There is **no current cached address-to-file:line table**. Object files and function ranges are cached, but every successful `location_ct` file/line lookup still calls libdw again via `dwarf_getsrc_die`, `dwarf_lineno`, and `dwarf_linesrc`.

## Global object-file cache

The global cache root is `libcwd::dwarf::objfiles_ct::ST_list_instance`.

Declared in `include/cwd_dwarf.h:82-97`:

```cpp
namespace libcwd::dwarf {

using object_files_ct = std::list<objfiles_ct*>;

class objfiles_ct
{
 protected:
  friend object_files_ct const& NEEDS_READ_LOCK_object_files();
  friend object_files_ct& NEEDS_WRITE_LOCK_object_files();
  static char ST_list_instance[sizeof(object_files_ct)];

  libcwd::object_file_ct M_object_file;

 public:
  objfiles_ct(char const* filepath) : M_object_file(filepath) { }
  libcwd::object_file_ct const* get_object_file() const { return &M_object_file; }
};

}
```

Defined in `dwarf.cc:545`:

```cpp
char objfiles_ct::ST_list_instance[sizeof(object_files_ct)] __attribute__((__aligned__));
```

Placement-initialized in `dwarf::ST_init` at `dwarf.cc:506-511`:

```cpp
LIBCWD_DEFER_CANCEL;
DWARF_ACQUIRE_WRITE_LOCK;
new (&NEEDS_WRITE_LOCK_object_files()) object_files_ct;
DWARF_RELEASE_WRITE_LOCK;
LIBCWD_RESTORE_CANCEL;
```

Accessors in `include/cwd_dwarf.h:100-120` reinterpret the raw storage as the real list:

```cpp
object_files_ct const& NEEDS_READ_LOCK_object_files();
object_files_ct& NEEDS_WRITE_LOCK_object_files();
```

These accessors are internal and assume the corresponding DWARF object-file lock is already held.

## Object lookup entry point

The canonical internal entry point for “which loaded object owns this address?” is:

```cpp
dwarf::objfile_ct* dwarf::get_object_file(void const* addr LIBCWD_COMMA_TSD_PARAM);
```

Defined at `dwarf.cc:1497-1518`. It:

1. Converts `addr` to `uintptr_t`.
2. Acquires the object-files read lock.
3. Calls `NEEDS_READ_LOCK_find_object_file(int_addr)`.
4. If not found and not statically linked, calls `dl_iterate_phdr` to lazily load the shared object that contains the address.

The actual list scan is `NEEDS_READ_LOCK_find_object_file` at `dwarf.cc:1488-1494`:

```cpp
object_files_ct::const_iterator i(NEEDS_READ_LOCK_object_files().begin());
for(; i != NEEDS_READ_LOCK_object_files().end(); ++i)
  if (static_cast<objfile_ct const*>(*i)->get_start_addr() < addr &&
      addr < static_cast<objfile_ct const*>(*i)->get_end_addr())
    break;
```

So the first cache entry point is the global `std::list<objfiles_ct*>`; the main helper that hides it is `dwarf::get_object_file(void const* addr)`.

## Per-object cache: `dwarf::objfile_ct`

`objfile_ct` is the actual cached object-file node. It derives from `objfiles_ct`, so entries in the global `std::list<objfiles_ct*>` are downcast to `objfile_ct*` when location lookup needs implementation details.

Declared at `dwarf.cc:277-311`:

```cpp
class objfile_ct : public objfiles_ct
{
  friend class libcwd::location_ct;

 private:
  Dwarf* dwarf_handle_;
  int dwarf_fd_;
  uintptr_t M_lbase;
  uintptr_t M_start_addr;
  uintptr_t M_end_addr;
  function_symbols_ct M_function_symbols;

 public:
  objfile_ct(char const* filename, uintptr_t base_addr,
      uintptr_t start_addr, uintptr_t end_addr);
  ~objfile_ct();

  bool initialize(char const* filename LIBCWD_COMMA_ALLOC_OPT(bool is_libc)
      LIBCWD_COMMA_TSD_PARAM);
  void deinitialize(LIBCWD_TSD_PARAM);
  uintptr_t get_lbase() const { return M_lbase; }
  uintptr_t get_start_addr() const { return M_start_addr; }
  uintptr_t get_end_addr() const { return M_end_addr; }

  symbol_ct const* find_symbol(symbol_ct const& search_key) const;

  bool is_initialized() const { return dwarf_fd_ != -1; }

 private:
  void extract_and_add_function_symbols(char const* cu_name, Elf& elf,
      Dwarf_Die* die_ptr LIBCWD_COMMA_TSD_PARAM);
  void open_dwarf(LIBCWD_TSD_PARAM);
  void close_dwarf(LIBCWD_TSD_PARAM);
};
```

Important members:

- `dwarf_handle_`: open libdw handle for this object's debug info.
- `dwarf_fd_`: file descriptor backing `dwarf_handle_`.
- `M_lbase`: object load base; subtracted from runtime PCs before libdw source lookup.
- `M_start_addr`, `M_end_addr`: executable address range used to decide whether an address belongs to this object.
- `M_function_symbols`: the cached function-range set for this object.

The per-object function-symbol cache type is declared at `dwarf.cc:273`:

```cpp
using function_symbols_ct = std::set<symbol_ct, symbol_key_greater>;
```

The symbol lookup entry point is `objfile_ct::find_symbol` at `dwarf.cc:1451-1457`:

```cpp
symbol_ct const* objfile_ct::find_symbol(symbol_ct const& search_key) const
{
  auto iter = M_function_symbols.find(search_key);
  if (iter == M_function_symbols.end())
    return nullptr;
  return &*iter;
}
```

## Per-range cache entry: `dwarf::symbol_ct`

Each `symbol_ct` represents a function address range plus enough metadata to recover the owning CU and mangled function name.

Declared at `dwarf.cc:222-262`:

```cpp
class symbol_ct
{
 private:
  Dwarf_Addr real_start_;               // Key
  Dwarf_Addr real_end_;

  mutable objfile_ct* objfile_;         // Data
  mutable Dwarf_Die die_;
  mutable char const* cu_name_;
  mutable char const* func_name_;
  mutable char const* linkage_name_;

 public:
  symbol_ct(Dwarf_Addr start, Dwarf_Addr end,
      objfile_ct* objfile, Dwarf_Die const* die,
      char const* cu_name, char const* func_name,
      char const* linkage_name);

  symbol_ct(Dwarf_Addr start, Dwarf_Addr end,
      objfile_ct* objfile, char const* linkage_name);

  symbol_ct(Dwarf_Addr start) : real_start_(start), real_end_(start + 1) { }

  Dwarf_Addr real_start() const { return real_start_; }
  Dwarf_Addr real_end() const { return real_end_; }
  objfile_ct const* objfile() const { return objfile_; }
  Dwarf_Die const* die() const { return &die_; }

  bool diecu(Dwarf_Die* cu_die_out) const
  {
    return die_.addr ? dwarf_diecu(&die_, cu_die_out, NULL, NULL) != nullptr : false;
  }

  char const* cu_name() const { return cu_name_; }
  char const* func_name() const { return func_name_; }
  char const* linkage_name() const { return linkage_name_; }
  char const* name() const { return linkage_name_ ? linkage_name_ : func_name_; }

  void set_func_name(char const* func_name) const { func_name_ = func_name; }
  void set_linkage_name(char const* linkage_name) const { linkage_name_ = linkage_name; }
  void set_real_end(Dwarf_Addr real_end) { real_end_ = real_end; }

  bool operator==(symbol_ct const&) const { DoutFatal(dc::core, "Calling operator=="); }
  friend struct symbol_key_greater;
};
```

Two real constructors exist:

- DWARF-backed constructor at `dwarf.cc:320-325`; stores a copied `Dwarf_Die` and CU/function/linkage names.
- `.symtab` fallback constructor at `dwarf.cc:327-331`; has no usable `Dwarf_Die`, so `diecu()` returns false.

The search-key constructor is used like this in `location_ct::M_pc_location` (`dwarf.cc:1647-1649`):

```cpp
uintptr_t int_addr = reinterpret_cast<uintptr_t>(addr);
symbol_ct search_key(int_addr);
symbol_ct const* symbol = object_file->find_symbol(search_key);
```

`symbol_key_greater` at `dwarf.cc:264-271` orders ranges by non-overlap:

```cpp
struct symbol_key_greater
{
  bool operator()(symbol_ct const& a, symbol_ct const& b) const
  {
    return a.real_start() >= b.real_end();
  }
};
```

This lets `std::set::find(symbol_ct(addr))` find the cached function range that contains `addr`.

## How the cache is populated

Initial object discovery happens in `dwarf::ST_init` (`dwarf.cc:475-543`):

- Constructs the global `object_files_ct` list.
- Computes the executable path.
- Calls `dl_iterate_phdr(dl_iterate_phdr_callback, fullpath.data())` for current loaded objects when not statically linked.
- `dl_iterate_phdr_callback` computes executable `PT_LOAD` ranges and calls `load_object_file` (`dwarf.cc:365-393`).

`load_object_file` (`dwarf.cc:333-363`) allocates an `objfile_ct`, calls `objfile_ct::initialize`, and returns the initialized object.

`objfile_ct::initialize` (`dwarf.cc:1157-1256`) populates `M_function_symbols`:

- Opens debug info with `open_dwarf`.
- Builds an `Elf` helper and processes relocations / `.symtab`.
- Iterates all compilation units with `dwarf_nextcu` and `dwarf_offdie`.
- Calls `extract_and_add_function_symbols` for each CU.
- Adds `.symtab` fallback symbols that do not overlap DWARF symbols.
- Pushes the object into `NEEDS_WRITE_LOCK_object_files()`.

`extract_and_add_function_symbols` (`dwarf.cc:940-1155`) recursively walks DIEs, selects `DW_TAG_subprogram` definitions, extracts linkage/function names and ranges, and inserts `symbol_ct` records into `M_function_symbols`.

Loaded shared objects can also be added later:

- `dwarf::get_object_file` lazily loads an object containing a missing address via `dl_iterate_phdr_callback2` (`dwarf.cc:1507-1513`).
- The interposed `dlopen` uses `dl_iterate_phdr_callback3` to add newly loaded objects (`dwarf.cc:1772-1821`).
- The interposed `dlclose` calls `objfile_ct::deinitialize` to erase an object from the global list and close its DWARF handle (`dwarf.cc:1823-1859`).

## Lookup path from public API to cache

### Source location lookup

Public use:

```cpp
#include <libcwd/debug.h>

libcwd::location_ct loc(
    (char*)__builtin_return_address(0) + libcwd::builtin_return_address_offset);

if (loc.is_known())
  std::cerr << loc.file() << ':' << loc.line() << '\n';

Dout(dc::notice, "called from " << loc);
```

Optional formatting:

```cpp
libcwd::location_format(libcwd::show_objectfile | libcwd::show_function);
```

Internal path:

```text
location_ct::location_ct(addr)
  -> location_ct::M_pc_location(addr)
      -> dwarf::get_object_file(addr)
          -> global std::list<objfiles_ct*> in objfiles_ct::ST_list_instance
      -> objfile_ct::find_symbol(symbol_ct(addr))
          -> objfile_ct::M_function_symbols
      -> symbol_ct::diecu(&cu_die)
      -> dwarf_getsrc_die(&cu_die, addr - objfile_ct::M_lbase)
      -> dwarf_lineno(...)
      -> dwarf_linesrc(...)
```

`M_pc_location` stores the result in the individual `location_ct` object (`dwarf.cc:1586-1685`):

- `M_object_file` gets `object_file->get_object_file()`.
- `M_func` gets `symbol->name()`.
- `M_line` gets the result of `dwarf_lineno`.
- `M_filepath` / `M_filename` get a copied source path from `dwarf_linesrc`.
- `M_known` becomes true only after a source path was copied.

### Mangled function-name lookup

Public use:

```cpp
char const* name = libcwd::pc_mangled_function_name(function_or_pc_address);
```

Declared in `include/libcwd/pc_mangled_function_name.h:21-24`; implemented at `dwarf.cc:1533-1555`.

Internal path:

```text
pc_mangled_function_name(addr)
  -> dwarf::get_object_file(addr)
  -> objfile_ct::find_symbol(symbol_ct(addr))
  -> symbol_ct::name()
```

This uses the same object and symbol caches as `location_ct`, but it does not call `dwarf_getsrc_die`, `dwarf_lineno`, or `dwarf_linesrc`.

## What is reused and what is recomputed

Reused from cache:

- Loaded-object list: `libcwd::dwarf::objfiles_ct::ST_list_instance` as `std::list<objfiles_ct*>`.
- Per-object executable address ranges: `objfile_ct::M_start_addr`, `objfile_ct::M_end_addr`.
- Per-object load base: `objfile_ct::M_lbase`.
- Per-object libdw handle: `objfile_ct::dwarf_handle_`.
- Per-object function range/name set: `objfile_ct::M_function_symbols`.
- Per-range function metadata: `symbol_ct::{real_start_, real_end_, die_, cu_name_, func_name_, linkage_name_}`.

Recomputed on every `location_ct` source lookup:

- Containing CU lookup from the cached DIE: `symbol_ct::diecu(&cu_die)`.
- Source-line lookup: `dwarf_getsrc_die(&cu_die, relative_addr)`.
- Line extraction: `dwarf_lineno`.
- Source path extraction and copy: `dwarf_linesrc` plus allocation into `location_ct::M_filepath`.

Not present today:

- No global PC-to-location cache.
- No per-object line-table cache owned by libcwd.
- No inline-subroutine source-location cache.
- No cache keyed by address range to final `(file, line, function)` output.
