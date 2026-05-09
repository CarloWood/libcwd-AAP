# CMake configuration options

## Project options

| CMake variable | Default | Meaning |
|---|---:|---|
| `-DEnableGlibCxxDebug=ON/OFF` | OFF | Compile with `-D_GLIBCXX_DEBUG`. |
| `-DEnableLibcwdAlloc=ON/OFF` | OFF | Enable allocation debugging support. This is planned for removal. |
| `-DEnableLibcwdMagic=ON/OFF` | ON | Enable magic numbers in allocation debugging. This becomes obsolete when allocation debugging is removed. |
| `-DEnableLibcwdMarker=ON/OFF` | ON | Enable allocation markers. This becomes obsolete when allocation debugging is removed. |
| `-DEnableLibcwdLocation=ON/OFF` | ON | Enable source file/line lookup. |
| `-DEnableLibcwdDebug=ON/OFF` | OFF | Enable debugging of libcwd itself. |
| `-DEnableLibcwdDebugOutput=ON/OFF` | ON, gated by `EnableLibcwdDebug` | Enable debug output from libcwd internal debugging. |
| `-DEnableLibcwdDebugm=ON/OFF` | OFF, gated by `EnableLibcwdDebug` | Debug allocation-debugging internals. This is planned for removal with allocation debugging. |
| `-DEnableLibcwdDebugt=ON/OFF` | OFF, gated by `EnableLibcwdDebug` | Debug threading-debugging internals. |
| `-DTHREADSMAX=N` | `1024` | Maximum number of simultaneously supported threads. |

## Shared CMake infrastructure

| CMake variable | Values |
|---|---|
| `-DCMAKE_BUILD_TYPE=` | `Release`, `Debug`, `RelWithDebInfo`, `BetaTest`, `RelWithDebug`, `Perf`, `Tracy`, `None` |

## Autotools-only options that are intentionally not preserved

| Autotools option | Reason |
|---|---|
| `--with-redzone[=N]` | Redzones belong to malloc debugging support, which is planned for removal. |
| `--disable-threading` / `--disable-nonthreading` | Non-threaded library support is planned for removal; CMake already builds the threaded library. |
| `--enable-profile` | First-class profiling of libcwd itself is not needed. |
| `--enable-optimize` | CMake build types and compiler flags cover optimization policy. |
