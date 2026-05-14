# Initialization entry point for object-file discovery

The discovery of loaded object files for `ObjectFileBase::s_object_files_` should be triggered from the location-support initializer reached by `libcwd::initialize()`: today that path is `ST_initialize_globals()` calling `libcwd::dwarf::ST_init()`.

For the new implementation, this implies adding a dwarf2 equivalent of `ST_init()` and invoking it from the same initialization point, after libcwd globals, locks, environment handling, and built-in debug channels have been initialized.

## Repository evidence

- `include/libcwd/libraries_debug.h` documents `libcwd::initialize()` as the public early-initialization entry point for user global constructors.
- `debug.cc` implements `libcwd::initialize()` as a call to `ST_initialize_globals(LIBCWD_TSD)`.
- `ST_initialize_globals()` initializes global mutexes, environment variables, built-in channels such as `dc::bfd`, `dc::warning`, and `dc::notice`, and then calls `dwarf::ST_init(LIBCWD_TSD)` when location support is enabled.
- `dwarf::ST_init()` initializes the existing object-file list, obtains the executable path, calls `dl_iterate_phdr()`, loads discovered executable/shared objects, sorts the list, and marks location initialization complete.
- `dwarf::load_object_file()` has a guard for pre-main `dlopen()` by calling `ST_init()` if the location subsystem has not been initialized yet.
- `ObjectFileBase::s_object_files_` and `dwarf2::ObjectFile` exist, but there is not yet a dwarf2 discovery/init function that populates the list.

## Initialization implications

`libcwd::initialize()` should remain the public trigger because it is intended to be idempotent and safe before `main()`. Global construction paths that touch libcwd also enter `ST_initialize_globals()` through `debug_ct::NS_init()`, so using that path preserves existing early-initialization behavior.

Object-file discovery should not run before debug-channel initialization. The old code uses `dc::bfd` diagnostics during `dwarf::ST_init()`, and `dc::bfd` is initialized immediately before that call. The dwarf2 initializer should preserve that ordering.

## Follow-up uncertainties

- Decide whether dwarf2 discovery temporarily coexists with the old `dwarf::ST_init()` or eventually replaces it.
- Define the exact dwarf2 initializer API, idempotence guard, and `dl_iterate_phdr()` callback behavior.
- Confirm the locking model for discovery and later `dlopen`/`dlclose` updates, since the old code and `ObjectFileBase::s_object_files_` use different locking abstractions.
