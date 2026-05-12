# Existing libcwd location-cache storage overview

Paths are relative to `$REPOROOT`.

## Cache shape

The current cache root is `libcwd::dwarf::objfiles_ct::ST_list_instance`, raw static storage for a placement-constructed `std::list<objfiles_ct*>`:

```cpp
// include/cwd_dwarf.h:82-97
using object_files_ct = std::list<objfiles_ct*>;

class objfiles_ct {
  static char ST_list_instance[sizeof(object_files_ct)];
  libcwd::object_file_ct M_object_file;
public:
  libcwd::object_file_ct const* get_object_file() const;
};

// dwarf.cc:545
char objfiles_ct::ST_list_instance[sizeof(object_files_ct)] __attribute__((__aligned__));
```

`dwarf::ST_init` constructs the list with placement `new` under `LIBCWD_DEFER_CANCEL` and `DWARF_ACQUIRE_WRITE_LOCK` (`dwarf.cc:506-512`). Access is through `NEEDS_READ_LOCK_object_files()` / `NEEDS_WRITE_LOCK_object_files()` (`include/cwd_dwarf.h:100-120`).

Each list entry is actually a `dwarf::objfile_ct*`.

## Per-object storage

`dwarf::objfile_ct` is the real object-cache node (`dwarf.cc:277-311`):

```cpp
using function_symbols_ct = std::set<symbol_ct, symbol_key_greater>; // dwarf.cc:273

class objfile_ct : public objfiles_ct {
  Dwarf* dwarf_handle_;
  int dwarf_fd_;
  uintptr_t M_lbase;
  uintptr_t M_start_addr;
  uintptr_t M_end_addr;
  function_symbols_ct M_function_symbols;

public:
  uintptr_t get_lbase() const;
  uintptr_t get_start_addr() const;
  uintptr_t get_end_addr() const;
  symbol_ct const* find_symbol(symbol_ct const& search_key) const;
};
```

`M_start_addr`/`M_end_addr` identify the loaded object's executable address range. `M_lbase` is subtracted from runtime addresses before libdw source lookup. `M_function_symbols` caches the function ranges for that object.

## Per-range storage

`dwarf::symbol_ct` stores one function range plus enough metadata for name and CU lookup (`dwarf.cc:222-262`):

```cpp
class symbol_ct {
  Dwarf_Addr real_start_;
  Dwarf_Addr real_end_;
  mutable objfile_ct* objfile_;
  mutable Dwarf_Die die_;
  mutable char const* cu_name_;
  mutable char const* func_name_;
  mutable char const* linkage_name_;

public:
  symbol_ct(Dwarf_Addr start, Dwarf_Addr end, objfile_ct*, Dwarf_Die const*,
            char const* cu_name, char const* func_name, char const* linkage_name);
  symbol_ct(Dwarf_Addr start, Dwarf_Addr end, objfile_ct*, char const* linkage_name);
  symbol_ct(Dwarf_Addr start);                  // search key: [start, start + 1)
  bool diecu(Dwarf_Die* cu_die_out) const;
  char const* name() const;                      // linkage_name_ if present, else func_name_
};
```

`symbol_key_greater` orders ranges by non-overlap (`dwarf.cc:264-271`), allowing `M_function_symbols.find(symbol_ct(addr))` to find the function range containing `addr`.

## Lookup entry points

- Object lookup: `dwarf::get_object_file(void const* addr)` (`dwarf.cc:1497-1518`). It scans `NEEDS_READ_LOCK_object_files()` via `NEEDS_READ_LOCK_find_object_file` (`dwarf.cc:1488-1494`) and may lazily load a shared object with `dl_iterate_phdr`.
- Function-range lookup: `objfile_ct::find_symbol(symbol_ct const&)` (`dwarf.cc:1451-1457`) searches `M_function_symbols`.
- Public source lookup: `location_ct::M_pc_location` (`dwarf.cc:1586-1685`) calls both of the above, then calls libdw for file/line.
- Public function-name lookup: `pc_mangled_function_name(void const*)` (`dwarf.cc:1533-1555`) calls the same object/symbol lookup path and returns `symbol_ct::name()`.

## Population path

- `dwarf::ST_init` initializes the global list and runs `dl_iterate_phdr` for loaded objects (`dwarf.cc:475-543`).
- `load_object_file` allocates an `objfile_ct` and calls `objfile_ct::initialize` (`dwarf.cc:333-363`).
- `objfile_ct::initialize` opens DWARF/debug info, processes ELF relocation/symtab data, iterates CUs, and fills `M_function_symbols` (`dwarf.cc:1157-1256`).
- `extract_and_add_function_symbols` walks `DW_TAG_subprogram` DIEs and inserts `symbol_ct` ranges (`dwarf.cc:940-1155`).
- Later `dlopen`/`dlclose` interception adds/removes object cache entries (`dwarf.cc:1772-1859`).

## Public use

```cpp
libcwd::location_ct loc((char*)__builtin_return_address(0) +
    libcwd::builtin_return_address_offset);

if (loc.is_known())
  std::cerr << loc.file() << ':' << loc.line() << '\n';

char const* mangled = libcwd::pc_mangled_function_name(addr);
```

Optional rendering flags are set with `libcwd::location_format(...)` using `show_path`, `show_objectfile`, and `show_function`.

## Reused vs recomputed

Reused: global object list, object address ranges, load base, open DWARF handle, and per-object `M_function_symbols` containing `symbol_ct` ranges/names/DIEs.

Recomputed for every successful `location_ct` source lookup: `symbol_ct::diecu`, `dwarf_getsrc_die`, `dwarf_lineno`, `dwarf_linesrc`, and the copy into `location_ct::M_filepath`.

Not present today: a cached address-to-file:line table, a libcwd-owned line-table cache, or inline-subroutine location cache.
