# Current libcwd address-to-location lookup overview

This overview summarizes the current address-to-file:line lookup implementation and public API as it exists before the planned refactor. Paths are relative to `$REPOROOT`.

## Main source files

| File | Role |
| --- | --- |
| `include/libcwd/bfd.h` | Public location umbrella included by `debug.h` when `CWDEBUG_LOCATION` is enabled; includes `class_location.h` and `pc_mangled_function_name.h` (`include/libcwd/bfd.h:24-29`). |
| `include/libcwd/class_location.h` / `.inl` | Declares and inlines most of `libcwd::location_ct`, the public source-location object (`include/libcwd/class_location.h:77-218`). |
| `include/libcwd/pc_mangled_function_name.h` | Declares the standalone public `libcwd::pc_mangled_function_name(void const*)` API (`include/libcwd/pc_mangled_function_name.h:21-24`). |
| `include/libcwd/class_object_file.h` | Declares public-facing `libcwd::object_file_ct`, exposed through `location_ct::object_file()` (`include/libcwd/class_object_file.h:48-67`). |
| `include/cwd_dwarf.h` | Declares the internal global object-file list type and locking helpers used by `dwarf.cc` (`include/cwd_dwarf.h:82-120`). |
| `dwarf.cc` | Implements object discovery, DWARF/ELF scanning, symbol caching, `pc_mangled_function_name`, and `location_ct::M_pc_location`. |
| `debug.cc` / `threading.cc` | Drive early libcwd initialization and initialize the locks used by the DWARF/object-file cache (`debug.cc:414-473`, `threading.cc:46-61`). |

## Public API and user-visible semantics

### `libcwd::location_ct`

`libcwd::location_ct` is the source-file location object used by client code. Its documented normal use is to pass a return address adjusted by `libcwd::builtin_return_address_offset` (`include/libcwd/class_location.h:71-75`). The configured offset is declared in `include/libcwd/sys.h.in:31-39`.

Important public constructors and operations:

- `explicit location_ct(void const* addr)` constructs a lookup result for a program-counter address (`include/libcwd/class_location.h:100`; inline implementation at `include/libcwd/class_location.inl:46-50`).
- In thread-safe builds, `explicit location_ct(void const* addr LIBCWD_COMMA_TSD_PARAM)` accepts existing thread-specific data (`include/libcwd/class_location.h:102-104`; `.inl:57-61`).
- `location_ct()` creates an uninitialized/unknown object with `M_func == S_uninitialized_location_ct_c` and `M_known == false` (`include/libcwd/class_location.h:108-114`; `.inl:73-76`).
- `pc_location(void const* pc)` clears the current object and re-runs lookup (`include/libcwd/class_location.h:145-148`; `.inl:81-88`).
- `clear()` resets the object and releases owned path storage when applicable (`include/libcwd/class_location.h:153-156`; `dwarf.cc:1690-1703`).
- Copy/assignment share the stored path through `lockable_auto_ptr<char, true>` and can transfer path ownership unless `lock_ownership()` was called (`include/libcwd/class_location.h:116-143`; `dwarf.cc:1705-1736`).

Important accessors:

- `is_known()` reports whether file and line are valid (`include/libcwd/class_location.h:160-164`; `.inl:90-95`).
- `file()` returns the basename and asserts that the location is known (`include/libcwd/class_location.h:166-172`; `.inl:97-103`).
- `line()` returns the line number and asserts that the location is known (`include/libcwd/class_location.h:174-175`; `.inl:105-111`).
- `mangled_function_name()` returns `M_func`, which can be a mangled function name, `unknown_function_c`, or an internal sentinel (`include/libcwd/class_location.h:177-183`; `.inl:113-118`).
- `object_file()` returns the `object_file_ct` for the owning executable/shared object when known (`include/libcwd/class_location.h:190-195`).
- `initialization_delayed()` and `unknown_pc()` expose unresolved/delayed lookup state and the address involved (`include/libcwd/class_location.h:215-217`). No current caller retries delayed lookups automatically.

Internal state fields of `location_ct` are also part of the compatibility picture because inline printing reads them directly:

- `M_filepath`: owned full source path, allocated in `M_pc_location` (`include/libcwd/class_location.h:79`; `dwarf.cc:1677-1679`).
- `M_filename`: pointer into `M_filepath` after the last slash (`include/libcwd/class_location.h:81`; `dwarf.cc:1680-1683`).
- `M_initialization_delayed` / `M_unknown_pc`: union members storing the address when lookup was delayed or no object/function was found (`include/libcwd/class_location.h:82-84`).
- `M_line`: source line (`include/libcwd/class_location.h:85`; set at `dwarf.cc:1667-1670`).
- `M_func`: pointer to a static/sentinel string or DWARF/ELF-provided mangled name (`include/libcwd/class_location.h:86`).
- `M_object_file`: pointer to the public object-file view (`include/libcwd/class_location.h:87`).
- `M_known`: true only when source path and line are valid (`include/libcwd/class_location.h:88`).

Sentinel strings are defined in `dwarf.cc:93-96`:

- `S_uninitialized_location_ct_c`
- `S_pre_ios_initialization_c`
- `S_pre_libcwd_initialization_c`
- `S_cleared_location_ct_c`

`unknown_function_c` is the public unknown-function sentinel (`include/libcwd/class_location.h:64-65`; defined at `dwarf.cc:1525`).

### Formatting API

`location_format(location_format_t format)` sets thread-specific output formatting and returns the previous format (`include/libcwd/class_location.h:224-237`; `.inl:120-128`). The flags are declared in `include/libcwd/private_struct_TSD.h:117-126`:

- `show_path = 1`: print the full source path.
- `show_objectfile = 2`: prepend the executable/shared-object filename.
- `show_function = 4`: prepend the mangled function name.

`operator<<(std::ostream&, location_ct const&)` delegates to `_private_::print_location_on` (`include/libcwd/class_location.inl:155-172`). Current rendering behavior is:

- Known source location: optional object file, optional mangled function, then either `full/path:line` or `basename:line` (`include/libcwd/class_location.inl:135-146`).
- Unknown source line but known object/function: `objectfile:mangledfunc` (`include/libcwd/class_location.inl:147-148`).
- Unknown object/delayed lookup: `<unknown object file> (at <addr>)` using `unknown_pc()` (`include/libcwd/class_location.inl:149-150`).

`print_filepath_on()` and `print_filename_on()` assert `M_known` and write the path/basename (`dwarf.cc:1867-1877`).

### `libcwd::pc_mangled_function_name`

`pc_mangled_function_name(void const* addr)` is a public function-name-only API (`include/libcwd/pc_mangled_function_name.h:21-24`). It uses the same object-file and symbol cache as `location_ct` but does not resolve line-table information (`dwarf.cc:1533-1555`):

1. Ensure DWARF support is initialized via `dwarf::ST_init` if `dwarf::WST_initialized` is false (`dwarf.cc:1539-1543`).
2. Find the owning `dwarf::objfile_ct` with `get_object_file` (`dwarf.cc:1545-1547`).
3. Build a `symbol_ct` search key from the address (`dwarf.cc:1549`).
4. Return `symbol->name()` if `objfile_ct::find_symbol` succeeds; otherwise return `unknown_function_c` (`dwarf.cc:1550-1554`).

There is no `pc_mangled_function_name_ct` type in the current codebase; the current public API is the function above.

## Accepted address forms seen in current code and tests

The API accepts `void const*` and internally treats it as a program-counter address.

Observed call patterns:

- Return address plus configured offset: `location_ct((char*)__builtin_return_address(0) + libcwd::builtin_return_address_offset)` (`include/libcwd/class_location.h:71-75`, `tests/lines.cc:7-10`, `testsuite/libcwd.tst/location.cc:93-95`, `testsuite/ctest/location_lookup.cc:21-27`).
- GCC label address: `location_ct(&&current_location)` (`tests/inlined.cc:36-40`, `tests/inlined_main.cc:57-63`).
- Function/data symbol address for name lookup: `pc_mangled_function_name(ptr)` in the CTest mangling coverage (`testsuite/ctest/mangling_symbols.cc:174-178`, `testsuite/ctest/mangling_symbols.cc:208-248`).

Internal address treatment:

- `get_object_file` converts to `uintptr_t` (`dwarf.cc:1497-1501`).
- `NEEDS_READ_LOCK_find_object_file` checks executable object ranges with strict comparisons `start < addr && addr < end` (`dwarf.cc:1488-1494`).
- `location_ct::M_pc_location` subtracts the object load base before passing the address to `dwarf_getsrc_die` (`dwarf.cc:1659-1662`).

## Internal object-file and symbol cache

### Global object list

The global loaded-object cache is `libcwd::dwarf::object_files_ct`, currently `std::list<objfiles_ct*>` (`include/cwd_dwarf.h:82`). It is stored in raw static storage `objfiles_ct::ST_list_instance` (`include/cwd_dwarf.h:85-90`; definition at `dwarf.cc:545`) and constructed with placement `new` in `dwarf::ST_init` (`dwarf.cc:506-511`).

Access to this list is through lock-checked helpers:

- `NEEDS_READ_LOCK_object_files()` returns a const reference and asserts read/write ownership in debug builds (`include/cwd_dwarf.h:100-110`).
- `NEEDS_WRITE_LOCK_object_files()` returns a mutable reference and asserts write ownership in debug builds (`include/cwd_dwarf.h:112-120`).

Threaded builds map these operations to `rwlock_tct<object_files_instance>` through macros in `include/cwd_dwarf.h:40-58`. The underlying mutex/rwlock instances are initialized by `_private_::initialize_global_mutexes()` (`threading.cc:46-61`).

### `object_file_ct` and `dwarf::objfiles_ct`

`libcwd::object_file_ct` is the public object-file view exposed by `location_ct::object_file()` (`include/libcwd/class_object_file.h:48-67`). It stores:

- `M_filepath`: full path to the executable/shared object, allocated with `malloc` in the constructor and intentionally leaked (`include/libcwd/class_object_file.h:50`; `dwarf.cc:1561-1566`).
- `M_filename`: basename pointer into `M_filepath` (`include/libcwd/class_object_file.h:51`; `dwarf.cc:1566-1568`).
- `M_no_debug_line_sections`: mutable status flag with setters/getters (`include/libcwd/class_object_file.h:52`, `:64-66`).

`dwarf::objfiles_ct` is the internal base wrapper that owns an `object_file_ct` (`include/cwd_dwarf.h:85-98`).

### `dwarf::objfile_ct`

`dwarf::objfile_ct` is the real per-object cache object (`dwarf.cc:277-311`). It derives from `objfiles_ct` and stores:

- `Dwarf* dwarf_handle_`: libdw handle (`dwarf.cc:282`).
- `int dwarf_fd_`: file descriptor for the debuginfo file (`dwarf.cc:283`).
- `uintptr_t M_lbase`: object load base (`dwarf.cc:284`).
- `uintptr_t M_start_addr`, `M_end_addr`: executable address range used for object lookup (`dwarf.cc:285-286`).
- `function_symbols_ct M_function_symbols`: per-object function-symbol/range set (`dwarf.cc:287`).

`objfile_ct::find_symbol` does lookup in `M_function_symbols` and returns a pointer to the matching `symbol_ct`, or null (`dwarf.cc:1451-1457`).

### `dwarf::symbol_ct`

`symbol_ct` represents a function range plus metadata (`dwarf.cc:222-262`). It stores:

- `real_start_`, `real_end_`: relocated absolute address range keys (`dwarf.cc:225-226`).
- `objfile_`: owning `objfile_ct` (`dwarf.cc:228`).
- `die_`: copied `Dwarf_Die` for the function DIE when available (`dwarf.cc:229`).
- `cu_name_`, `func_name_`, `linkage_name_`: CU/function/linkage strings (`dwarf.cc:230-232`).

Accessors include `diecu(Dwarf_Die*)`, which calls `dwarf_diecu` unless the symbol came only from `.symtab` and has no DIE (`dwarf.cc:245-250`), and `name()`, which prefers `linkage_name_` over `func_name_` (`dwarf.cc:251-254`).

`function_symbols_ct` is `std::set<symbol_ct, symbol_key_greater>` (`dwarf.cc:273`). The comparator returns true only when one range starts at or beyond the other range's end (`dwarf.cc:264-271`). This makes overlapping ranges equivalent for lookup, allowing a dummy `[addr, addr+1)` `symbol_ct` search key to find the containing function range.

### `dwarf::Elf`

`Elf` is an internal helper around libelf/gelf (`dwarf.cc:619-712`). It stores:

- `GElf_Addr lbase_`, `int fd_`, `::Elf* elf_`, and `size_t shstrndx_` (`dwarf.cc:621-625`).
- `relocation_map_`, a `std::map<char const*, Range, SymbolNameCompare>` keyed by ELF symbol name (`dwarf.cc:630-639`).

Its role is to extract relocated function ranges from relocation records and `.symtab`, then supply those ranges to DWARF symbol construction:

- `process_relocs()` walks relocation sections and `.symtab` (`dwarf.cc:817-916`).
- `add_symbol()` stores `STT_FUNC` symbols with nonzero `st_size` into `relocation_map_` (`dwarf.cc:714-741`).
- `apply_relocation()` maps a linkage name to a start/end range when DWARF gives low_pc zero (`dwarf.cc:928-938`).
- `add_symtab()` invokes a callback for all collected ranges (`dwarf.cc:778-783`).
- `add_backup()` records a DWARF-derived range as a fallback for later zero-low-pc DIEs (`dwarf.cc:918-926`).

## Initialization and loaded-object discovery

### Normal early initialization path

The normal early path starts from the global debug object:

1. `libcwd::libcw_do` is a global `debug_ct` object defined in `debug.cc:287-294`.
2. `debug_ct::debug_ct()` calls `NS_init(LIBCWD_TSD)` during global construction (`include/libcwd/class_debug.inl:197-215`).
3. `debug_ct::NS_init()` calls `ST_initialize_globals(LIBCWD_TSD)` before object-specific initialization (`debug.cc:1196-1204`).
4. `ST_initialize_globals()` is guarded by static `ST_already_called` (`debug.cc:414-419`), initializes global mutexes and debug channels (`debug.cc:420-439`), initializes the default debug object (`debug.cc:441-442`), and then calls `dwarf::ST_init(LIBCWD_TSD)` when `CWDEBUG_LOCATION` is enabled (`debug.cc:472-473`).

The code comments explicitly expect this path to happen before `main()` (`dwarf.cc:487-488`). Channel classes also document that global channel objects can force initialization from global-object constructors (`include/libcwd/class_channel.h:110-118`, `include/libcwd/class_continued_channel.h:48-57`, `include/libcwd/class_fatal_channel.h:54-63`).

### `dwarf::ST_init`

`dwarf::ST_init(LIBCWD_TSD_PARAM)` is the core DWARF initialization function (`dwarf.cc:475-543`). It:

- Uses static `WST_being_initialized` to reject recursive initialization (`dwarf.cc:477-481`).
- Calls `libcw_do.NS_init` and aborts on recursive debug initialization (`dwarf.cc:483-485`).
- Assumes single-threaded startup and treats multithreaded entry here as fatal in debug/threaded builds (`dwarf.cc:487-492`).
- Constructs the global object-files list (`dwarf.cc:506-511`).
- Computes the full executable path through `ST_get_full_path_to_executable` (`dwarf.cc:514-520`; helper at `dwarf.cc:153-217`).
- Initializes the object-file lock with `DWARF_INITIALIZE_LOCK` (`dwarf.cc:522-523`).
- If not `statically_linked`, calls `dl_iterate_phdr(dl_iterate_phdr_callback, fullpath.data())` to load all currently known executable/shared objects, then sorts the object list (`dwarf.cc:525-529`).
- Sets namespace-scope `dwarf::WST_initialized = true` (`dwarf.cc:538`; declaration at `dwarf.cc:219`).

`dwarf::statically_linked` is inferred from `__PIC__` (`dwarf.cc:100-105`). In the current code, initial full-object scanning is skipped when `statically_linked == true` (`dwarf.cc:525-530`).

### `load_object_file` and phdr callbacks

`load_object_file(char const* name, uintptr_t base_addr, uintptr_t start_addr, uintptr_t end_addr, bool initialized = false)` creates and initializes one `objfile_ct` (`dwarf.cc:333-363`). It has a separate local static `WST_initialized` used to handle a `dlopen` call that reaches libcwd before normal initialization: if not already locally initialized, it either trusts the `initialized` argument or calls `ST_init` (`dwarf.cc:335-345`). It then allocates an `objfile_ct`, calls `objfile_ct::initialize`, and returns null on failure or duplicate (`dwarf.cc:346-362`).

Program-header callbacks discover objects by executable `PT_LOAD` ranges:

- `dl_iterate_phdr_callback` scans every object during `ST_init`, computes executable range, and calls `load_object_file`; the main executable uses the previously resolved full path when `dlpi_name` is empty (`dwarf.cc:365-393`).
- `dl_iterate_phdr_callback2` is used lazily by `get_object_file` when an address was not found in the current cache; it loads the matching shared object only (`dwarf.cc:402-433`).
- `dl_iterate_phdr_callback3` is used after `dlopen` to load the object whose base address matches the new `link_map` (`dwarf.cc:436-463`).
- `resort_object_files` sorts the global object list by descending load base using `object_file_greater` (`dwarf.cc:313-318`, `:465-473`).

### `dlopen` / `dlclose` interception

When `HAVE_DLOPEN` is enabled, `dwarf.cc` defines interposing `extern "C"` `dlopen` and `dlclose` (`dwarf.cc:1770-1861`).

`dlopen`:

- Lazily resolves the real function with `dlsym(RTLD_NEXT, "dlopen")` (`dwarf.cc:1772-1778`).
- Calls the real `dlopen`, ignores null handles and `RTLD_NOLOAD`, and warns for statically linked applications (`dwarf.cc:1777-1789`).
- Uses `dlinfo(..., RTLD_DI_LINKMAP, ...)` and `dl_iterate_phdr_callback3` to load the new object into the cache (`dwarf.cc:1805-1814`).
- Tracks handles in `_private_::dlopen_map`, a `std::map<void*, dlloaded_st>` (`dwarf.cc:1743-1754`, `:1790-1819`).

`dlclose`:

- Serializes around the real `dlclose` with `dlclose_instance` (`dwarf.cc:1823-1836`).
- Decrements the tracked refcount and, unless `RTLD_NODELETE` applies, calls `objfile_ct::deinitialize` to erase the object from the global list and close its DWARF handle (`dwarf.cc:1840-1854`; deinitialize at `dwarf.cc:1474-1486`).
- Intentionally does not delete `M_object_file` because existing `location_ct` objects may still point to it (`dwarf.cc:1849-1850`).

## Building the current symbol cache

`objfile_ct::initialize` is responsible for constructing the per-object symbol cache (`dwarf.cc:1157-1256`). Its current behavior:

1. Checks for an already loaded object with the same load base in the global list (`dwarf.cc:1159-1172`).
2. Opens DWARF/debug info with `open_dwarf` (`dwarf.cc:1174-1177`).
3. Opens the object with `Elf`, processes relocations and `.symtab` (`dwarf.cc:1181-1185`).
4. If a DWARF handle exists, iterates CUs with `dwarf_nextcu`, obtains each CU DIE with `dwarf_offdie`, gets the CU name with `dwarf_diename`, and calls `extract_and_add_function_symbols` (`dwarf.cc:1186-1215`).
5. Adds `.symtab` ranges that do not overlap already stored DWARF ranges, mostly as a fallback for objects without `.debug_info` (`dwarf.cc:1227-1237`).
6. Pushes the initialized object into the global object list (`dwarf.cc:1248-1253`).

`open_dwarf` finds separate debug info via `get_debug_info_path`, opens the selected path, and creates a `Dwarf*` with `dwarf_begin` (`dwarf.cc:1419-1449`). `get_debug_info_path` tries build-id paths under `/usr/lib/debug/.build-id`, `/usr/lib/debug<path>.debug`, and debuginfod cache locations such as `DEBUGINFOD_CACHE_PATH`, `XDG_CACHE_HOME`, and `$HOME` (`dwarf.cc:1346-1417`).

`extract_and_add_function_symbols(char const* cu_name, Elf& elf, Dwarf_Die* die_ptr, ...)` recursively walks DIE children (`dwarf.cc:940-1155`):

- It recurses into any child with children before processing that child (`dwarf.cc:946-954`).
- It only creates symbols for `DW_TAG_subprogram` DIEs (`dwarf.cc:959-960`).
- It skips declarations and deleted functions (`dwarf.cc:965-973`).
- It prefers mangled names by looking for `DW_AT_MIPS_linkage_name` or `DW_AT_linkage_name`, following `DW_AT_specification` or `DW_AT_abstract_origin` references if needed (`dwarf.cc:975-991`).
- It falls back to `dwarf_diename` for `func_name` and ignores nameless functions (`dwarf.cc:993-997`).
- It iterates address ranges with `dwarf_ranges` (`dwarf.cc:999-1010`).
- If a range starts at zero, it tries `Elf::apply_relocation` using the preferred name (`dwarf.cc:1013-1043`).
- It skips empty ranges and requires at least one non-empty range if any empty range was seen (`dwarf.cc:1047-1058`, `:1146`).
- It inserts `symbol_ct` entries into `M_function_symbols` under the object-file write lock (`dwarf.cc:1060-1062`).
- It handles duplicate/alias/overlapping ranges by updating stored names or, for relocated duplicates with different end addresses, adjusting `real_end_` (`dwarf.cc:1074-1142`).

Important limitation for inlining: this function does not create entries for `DW_TAG_inlined_subroutine`, and the codebase does not reference `DW_AT_call_file`, `DW_AT_call_line`, `dwarf_getscopes`, or similar inline-stack APIs. The only source-line lookup currently uses `dwarf_getsrc_die` on the containing CU (`dwarf.cc:1661`).

## Lookup flow

### Object lookup

`get_object_file(void const* addr, ...)` is the common object lookup path (`dwarf.cc:1497-1518`):

1. Convert `addr` to `uintptr_t` (`dwarf.cc:1499-1501`).
2. Acquire the object-files read lock and linearly scan the global `object_files_ct` list via `NEEDS_READ_LOCK_find_object_file` (`dwarf.cc:1502-1505`).
3. If no object was found and the application is not statically linked, use `dl_iterate_phdr_callback2` to lazily load the shared object that contains the address, then resort the object list (`dwarf.cc:1507-1513`).
4. Return the `objfile_ct*` or null (`dwarf.cc:1515-1518`).

### Function-name lookup

`pc_mangled_function_name` is described above. It does not require a known source line; a symbol table fallback from `.symtab` is enough to return a name (`dwarf.cc:1533-1555`).

### Source-location lookup

`location_ct::M_pc_location(void const* addr, ...)` is the current address-to-file:line implementation (`dwarf.cc:1586-1685`). Its behavior:

1. Assert the object is not already known (`dwarf.cc:1588`).
2. If `dwarf::WST_initialized` is false, call `dwarf::ST_init`. If that fails, store delayed state: `M_object_file = nullptr`, `M_func = S_pre_libcwd_initialization_c`, `M_initialization_delayed = addr`, then return (`dwarf.cc:1592-1604`).
3. In thread-safe builds, if `__libcwd_tsd.pthread_lock_interface_is_locked`, avoid possible deadlock by storing delayed state and returning (`dwarf.cc:1606-1630`). The comment says this can occur while emitting debug output and recursively allocating/looking up locations (`dwarf.cc:1609-1625`).
4. Find the owning object with `get_object_file` (`dwarf.cc:1633`). If none, set `M_func = unknown_function_c`, store `M_unknown_pc`, and return (`dwarf.cc:1635-1642`).
5. Store the public `object_file_ct` pointer in `M_object_file` (`dwarf.cc:1645`).
6. Look up a containing `symbol_ct` in `objfile_ct::M_function_symbols` (`dwarf.cc:1647-1655`). If none, set `M_func = unknown_function_c`, store `M_unknown_pc`, and return.
7. Store `M_func = symbol->name()` (`dwarf.cc:1657`).
8. Obtain the containing CU DIE with `symbol->diecu(&cu_die)` and ask libdw for the nearest source line using `dwarf_getsrc_die(&cu_die, int_addr - object_file->get_lbase())` (`dwarf.cc:1659-1664`). If either fails, leave `M_known = false` and return.
9. Read the line number with `dwarf_lineno` into `M_line` (`dwarf.cc:1667-1670`).
10. Read the source path with `dwarf_linesrc`; allocate and copy it into `M_filepath`; set `M_known = true`; set `M_filename` to the basename pointer (`dwarf.cc:1672-1684`).

## What is cached vs recomputed today

Cached during initialization/object load:

- List of loaded executable/shared objects and their executable address ranges (`object_files_ct`, `objfile_ct::M_start_addr`, `M_end_addr`).
- Per-object libdw handle and file descriptor (`objfile_ct::dwarf_handle_`, `dwarf_fd_`).
- Per-object function ranges and names (`objfile_ct::M_function_symbols`, `symbol_ct`).
- Copied `Dwarf_Die` for each DWARF-derived function symbol (`symbol_ct::die_`).
- ELF/symtab fallback ranges are folded into `M_function_symbols` during `objfile_ct::initialize`; the temporary `Elf::relocation_map_` itself is destroyed when initialization returns (`dwarf.cc:669-678`).

Not cached by libcwd today:

- No explicit program-counter-to-`location_ct` or address-range-to-file:line cache exists.
- Each `location_ct::M_pc_location` repeats `symbol->diecu`, `dwarf_getsrc_die`, `dwarf_lineno`, and `dwarf_linesrc` (`dwarf.cc:1659-1674`).
- Resolved paths are copied only into the individual `location_ct` instance (`dwarf.cc:1677-1684`).
- There is a historical comment referring to a “location cache” in the deadlock discussion (`dwarf.cc:1615-1621`) and another in `dlclose` serialization (`dwarf.cc:1829-1830`), but no corresponding cache structure is present in the current code.

Lifetime observations:

- `object_file_ct` path strings are intentionally leaked and stable for process lifetime (`dwarf.cc:1561-1568`).
- `location_ct` copies its source path into owned storage, so `file()`/printing are independent of libdw line-table storage once lookup succeeds (`dwarf.cc:1677-1684`).
- `M_func` points to either a static sentinel or a string returned from DWARF/ELF processing; the code does not explicitly copy mangled names into libcwd-owned storage (`dwarf.cc:975-995`, `:1657`).
- On `dlclose`, `objfile_ct::deinitialize` closes the DWARF handle but does not delete the object because `location_ct` may still hold its `object_file_ct` pointer (`dwarf.cc:1474-1486`, `:1849-1850`). The current code does not document equivalent lifetime protection for DWARF-backed function-name pointers stored in existing `location_ct` objects.

## Accuracy and inline-source behavior today

The current lookup is line-table based after finding a containing `DW_TAG_subprogram` symbol. It does not inspect inline-subroutine DIEs or inline call stacks.

Consequences:

- Address-to-function lookup is based on `M_function_symbols`, which is populated only from `DW_TAG_subprogram` DIEs and `.symtab` function ranges (`dwarf.cc:959-1062`, `:1227-1237`). It ignores `DW_TAG_inlined_subroutine` entries.
- Source line lookup calls `dwarf_getsrc_die` for the containing CU and address (`dwarf.cc:1661`). That reports a line-table row; it does not by itself choose between an inline function's abstract/original source line and an expansion/call-site line.
- No current code reads `DW_AT_call_file`, `DW_AT_call_line`, `DW_AT_abstract_origin` for inline call-site resolution except when following references to find a subprogram linkage name (`dwarf.cc:975-991`).
- Therefore the current implementation cannot guarantee the desired behavior “return the exact source location inside the original inline function body rather than where that function was inlined.” A new implementation will need explicit inline-stack-aware resolution and a cache data model that represents those ranges.

## Related tests and expected behavior

Current CTest coverage:

- `testsuite/ctest/location_lookup.cc` constructs `location_ct` from a return address in `A::A()` and checks both the rendered `location_lookup.cc:<line>` and that `mangled_function_name()` demangles to `A::A()` for a caller (`testsuite/ctest/location_lookup.cc:1-7`, `:21-27`, `:35-39`, `:64-117`).
- `testsuite/ctest/mangling_symbols.cc` calls `pc_mangled_function_name` for many function addresses and one data address, then compares libcwd demangling with the platform ABI demangler (`testsuite/ctest/mangling_symbols.cc:174-178`, `:208-248`).

Legacy/manual tests relevant to the planned port:

- `testsuite/libcwd.tst/location.cc` walks multiple caller return addresses through `libcwd_bfd_test3`, formats locations with `show_objectfile | show_function`, and expects file/function/line output (`testsuite/libcwd.tst/location.cc:50-111`, `:166-175`). Expected regexes include `location.cc:120`, `:129`, `:138`, and `:175` (`testsuite/libcwd.tst/location.re:6-17`).
- `tests/lines.cc` checks ordinary line resolution and `#line` remapping to `foo.c` and `bar.c` (`tests/lines.cc:7-23`).
- `tests/inlined.cc` and `tests/inlined_main.cc` exercise label-address and return-address lookup around inline/default-argument calls: `g` has default arguments that call `h29()` and `h30()`, `f()` calls `g(h40())`, and each `h*` prints the resolved caller location (`tests/inlined.cc:20-40`, `tests/inlined_main.cc:27-45`, `:57-63`). These are the most relevant existing examples for the desired inline-source accuracy.

## Compatibility targets for the refactor

The refactor should preserve these current public contracts unless deliberately changed:

- Keep `location_ct(void const*)`, `pc_location(void const*)`, `is_known()`, `file()`, `line()`, `mangled_function_name()`, `object_file()`, `operator<<`, and `location_format` behavior and names.
- Keep `pc_mangled_function_name(void const*)` returning a pointer to a mangled name or `unknown_function_c`.
- Preserve accepted address patterns: return addresses plus `builtin_return_address_offset`, label addresses, and symbol/function addresses.
- Preserve rendering formats, including `show_objectfile`, `show_function`, and `show_path`.
- Preserve useful unknown/delayed states or replace them with an equivalent that still prints deterministically.
- Preserve fast lookup from an initialization-time object/symbol cache, while adding a real reusable source-location cache and inline-aware source resolution.
