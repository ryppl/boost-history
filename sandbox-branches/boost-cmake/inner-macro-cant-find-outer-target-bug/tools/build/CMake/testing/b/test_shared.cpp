#ifndef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif
#ifndef A_STICKY_SHARED_COMPILE_FLAGS
#error A_STICKY_SHARED_COMPILE_FLAGS
#endif

#ifndef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif
#ifndef B_STICKY_SHARED_COMPILE_FLAGS
#error B_STICKY_SHARED_COMPILE_FLAGS
#endif


#include "a.hpp"
#include "b.hpp"
#include <cassert>
int main(int argc, char**argv)
{
  assert(b_linktype() == "shared");
  assert(a_linktype() == "shared");
}

