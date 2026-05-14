# Parallel old and dwarf2 loading

The old implementation and the new `dwarf2` implementation can coexist during development, but the existing `dwarf.cc` code should remain the sole owner of exported `dlopen`/`dlclose` interposition and the active `location_ct` lookup path until an explicit switch-over point.

The new `dwarf2.cc` can be built and populated in parallel, but it should not define independent exported `dlopen`/`dlclose` wrappers or silently take over lookup behavior while the old code is still active.

## Repository evidence

- The build currently compiles both `dwarf.cc` and `dwarf2.cc` into the same `cwd_r` library.
- `dwarf.cc` owns the exported `extern "C" dlopen` and `extern "C" dlclose` wrappers.
- `dwarf.cc` stores real loader function pointers resolved through `dlsym(RTLD_NEXT, ...)`.
- `dwarf.cc` maintains a `dlopen_map` that associates handles with flags, reference counts, and old `dwarf::objfile_ct*` state.
- The current address-to-object lookup and source-location lookup paths are still in `dwarf.cc`, including `location_ct::M_pc_location()` and `pc_mangled_function_name()`.
- `dwarf2.cc` defines separate object-file and DWARF-opening logic, and `include/cwd_dwarf2.h` defines `ObjectFileBase::s_object_files_`, but this list is not yet wired into loader events or the active lookup path.
- Existing CTest coverage includes both current location lookup and shared-library loading/unloading behavior.

## Coexistence risks and constraints

- There must not be a second exported `dlopen` or `dlclose` wrapper. Duplicate interposition risks duplicate symbols, recursion, incorrect `RTLD_NEXT` behavior, and inconsistent handle bookkeeping.
- The old lookup path has lifetime assumptions around old object-file data; unload handling must not invalidate data still referenced by existing `location_ct` objects.
- Any parallel dwarf2 bookkeeping must avoid lock-order inversions with the old object-file, `dlopen_map`, and `dlclose` locks.
- `dwarf2` should be treated as passive until explicitly connected to loader events and lookup APIs.

## Plan implications

- Keep `dwarf.cc` authoritative for loader interposition and current lookup while `dwarf2` is developed.
- Add dwarf2 object tracking through internal hooks or controlled calls from the existing loader path, not by adding another wrapper layer.
- Switch `location_ct` and `pc_mangled_function_name()` to dwarf2 only after equivalent object tracking, unload semantics, locking, and regression tests exist.
