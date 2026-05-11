#include "hello_world_debug.h"
#include "libcwd/config.h"
#include "test_support.h"

#include <cstdlib>
#include <sstream>

int main()
{
  std::stringstream captured;

  Debug(check_configuration());
  Debug(libcw_do.set_ostream(&captured));
  Debug(libcw_do.on());
  Debug(read_rcfile());
  Debug(list_channels_on(libcw_do));

  Dout(dc::hello, "Hello World!");

  char const* expected[] = {
    "BFD     : Enabled",
    "DEBUG   : Enabled",
    "HELLO   : Enabled",
    "NOTICE  : Disabled",
    "SYSTEM  : Enabled",
    "WARNING : Disabled",
    "HELLO   : Hello World!",
    nullptr
  };

  return libcwd_ctest::matches_expected_output(captured, expected) ? EXIT_SUCCESS : EXIT_FAILURE;
}
