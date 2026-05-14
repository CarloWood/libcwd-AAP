# Eager vs. lazy loading behavior

The existing implementation is mixed, but mostly eager per discovered object file. It eagerly opens and parses debug/symbol information when an object file is registered, not when the first address lookup in that object occurs. There is also a lazy discovery fallback: if an address lookup cannot find a registered object file covering the address, the old code can scan loaded program headers and register the matching object at that point.

## Repository evidence

- `debug.cc::ST_initialize_globals()` calls `dwarf::ST_init()` during libcwd initialization.
- `dwarf::ST_init()` initializes the object-file list, obtains the executable path, and for dynamically linked builds calls `dl_iterate_phdr()`.
- The `dl_iterate_phdr` callback calls `load_object_file()` for the executable and already-loaded shared objects.
- `load_object_file()` constructs an `objfile_ct` and immediately calls `objfile_ct::initialize()`.
- `objfile_ct::initialize()` immediately opens DWARF/ELF state, processes relocation and symbol information, iterates DWARF compilation units, and fills the function-symbol cache.
- Lookup paths such as `pc_mangled_function_name()` and `location_ct::M_pc_location()` use `get_object_file()` and then query the already-populated symbol data when the object is already registered.
- Runtime loading is also eager in the normal path: the interposed `dlopen()` registers newly loaded shared objects immediately after successful load.
- The lazy fallback is in `get_object_file()`: if no registered object covers the address, it may call `dl_iterate_phdr()` and load the matching shared object then.

## Behavior to preserve or intentionally change

The current observable behavior is that `dc::bfd` loading diagnostics can occur during libcwd initialization and immediately after `dlopen()`, before any explicit source-location lookup for that object. The dwarf2 implementation should either preserve that timing or explicitly change tests and expectations to support true first-lookup loading.

If preserving current behavior, address lookups should normally be cache lookups into already-registered object data. If changing to lazy parsing, later goals must define new behavior for diagnostic timing, failure reporting, file-descriptor/DWARF-handle lifetime, and `dlopen`/`dlclose` interactions.

## Follow-up uncertainties

- Decide whether dwarf2 should preserve eager parsing for compatibility or intentionally use lazy parsing for performance.
- Clarify expected behavior for statically linked builds, where the existing initialization path skips dynamic-object enumeration.
- Confirm whether dwarf2 needs to keep DWARF handles alive for object lifetime, as the old symbol data appears to depend on DWARF-owned storage.
