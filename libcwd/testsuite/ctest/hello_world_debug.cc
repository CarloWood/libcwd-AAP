#include "hello_world_debug.h"

#ifdef CWDEBUG

namespace libcwd_ctest {
namespace debug_channels {
namespace dc {

::libcwd::channel_ct hello("HELLO");

} // namespace dc
} // namespace debug_channels
} // namespace libcwd_ctest

#endif // CWDEBUG
