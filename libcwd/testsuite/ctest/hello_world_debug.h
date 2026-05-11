#ifndef LIBCWD_TESTSUITE_CTEST_HELLO_WORLD_DEBUG_H
#define LIBCWD_TESTSUITE_CTEST_HELLO_WORLD_DEBUG_H

#include "sys.h"

#ifdef CWDEBUG

#define DEBUGCHANNELS ::libcwd_ctest::debug_channels
#include "libcwd/debug.h"

namespace libcwd_ctest {
namespace debug_channels {
namespace dc {

using namespace ::libcwd::channels::dc;
extern ::libcwd::channel_ct hello;

} // namespace dc
} // namespace debug_channels
} // namespace libcwd_ctest

#endif // CWDEBUG

#endif // LIBCWD_TESTSUITE_CTEST_HELLO_WORLD_DEBUG_H
