#include "a.hpp"
#include <cassert>

#ifndef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif
#ifndef A_STICKY_STATIC_COMPILE_FLAGS
#error A_STICKY_STATIC_COMPILE_FLAGS
#endif

int main(int argc, char**argv)
{
  assert(a_linktype() == "static");
}

