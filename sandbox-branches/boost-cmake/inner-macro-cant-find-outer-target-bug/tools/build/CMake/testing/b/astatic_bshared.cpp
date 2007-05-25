#ifndef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif

#ifndef A_STICKY_STATIC_COMPILE_FLAGS
#error A_STICKY_STATIC_COMPILE_FLAGS
#endif

#ifndef B_STICKY_COMPILE_FLAGS
#error static flags not here
#endif

#ifndef B_STICKY_SHARED_COMPILE_FLAGS
#error static flags not here
#endif

#include "a.hpp"
#include "b.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(b_linktype() == "shared");
  assert(a_linktype() == "static");
}

