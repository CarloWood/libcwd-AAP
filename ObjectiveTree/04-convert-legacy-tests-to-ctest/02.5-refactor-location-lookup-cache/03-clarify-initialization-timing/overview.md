# libcwd initialization timing essentials

## Current initialization entry points

- `libcwd::debug_ct libcw_do` is the global default debug object (`debug.cc:294`).
- `debug_ct::debug_ct()` calls `debug_ct::NS_init(LIBCWD_TSD)` during global construction (`include/libcwd/class_debug.inl:206-214`).
- `debug_ct::NS_init` immediately calls `libcwd::ST_initialize_globals(LIBCWD_TSD)` (`debug.cc:1196-1204`).
- `ST_initialize_globals` is guarded by static `ST_already_called` and is therefore idempotent (`debug.cc:414-419`). It initializes mutexes, environment flags, core/fatal/default channels, `libcw_do`, and then `dwarf::ST_init` when `CWDEBUG_LOCATION` is enabled (`debug.cc:420-473`).
- `libcwd::dwarf::ST_init(LIBCWD_TSD)` initializes the DWARF/object-file cache and currently assumes it is called before `main()` while still single-threaded (`dwarf.cc:475-543`, especially `dwarf.cc:487-488`).

Most likely, initialization happens from the global constructor of `libcw_do` before `main()`. Other global `channel_ct` constructors can also force channel/debug initialization early because `channel_ct::channel_ct` calls `NS_initialize` (`include/libcwd/class_channel.inl:35-39`).

## Explicit public initialization shape

If users need to initialize libcwd from their own global constructor before `libcw_do`'s constructor happens to run, add a small public wrapper around the existing guarded initializer, for example:

```cpp
namespace libcwd {
  void initialize()
  {
    LIBCWD_TSD_DECLARATION;
    ST_initialize_globals(LIBCWD_TSD);
  }
}
```

This should be declared in a public header and implemented where `ST_initialize_globals` is visible. It can be void because `ST_initialize_globals` is already idempotent; if a success/failure result is desired, the implementation would need to expose the failure path currently hidden inside `debug_ct::NS_init` / `dwarf::ST_init`.

Example use from a user global constructor:

```cpp
struct EarlyLibcwdInit {
  EarlyLibcwdInit() { libcwd::initialize(); }
};
static EarlyLibcwdInit early_libcwd_init;
```

## Channel state after initialization

Initialization creates/registers the default channels:

- `dc::core`, `dc::fatal`, `dc::debug`, `dc::continued`, `dc::finish`, `dc::bfd`, `dc::warning`, `dc::notice`, `dc::system` (`debug.cc:425-439`).

But channels are **not all on** by default:

- `channel_ct::NS_initialize` initializes normal channels off (`off_cnt = 0`) and only turns `WARNING` on by default (`debug.cc:1487-1522`).
- `dc::notice` is initialized but off until `Debug(dc::notice.on())`.
- There is no `dc::error` channel in the current code; errors are represented with `error_cf` or fatal/warning channels.
- `libcw_do` itself is initialized off by default (`debug.cc:1244-1249`); after initialization, `Debug(libcw_do.on())` turns the debug object on. With only that call, `dc::warning` output can print; notices still need `dc::notice.on()`.

## `LIBCWD_PRINT_LOADING`

`_private_::process_environment_variables()` sets `_private_::always_print_loading` when `LIBCWD_PRINT_LOADING` is present, and `_private_::suppress_startup_msgs` when `LIBCWD_NO_STARTUP_MSGS` is present (`environ.cc:21-28`). `LIBCWD_NO_STARTUP_MSGS` suppresses startup output and overrides loading output (`include/libcwd/private_environ.h:26-36`).

During `dwarf::ST_init`, when `always_print_loading && !suppress_startup_msgs`, libcwd temporarily forces both `libcw_do` and `dc::bfd` on, loads debug info, then restores their previous states (`dwarf.cc:497-535`). The debug-info loading messages are currently written through `dc::bfd`.
