#ifndef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif
#ifndef A_STICKY_STATIC_COMPILE_FLAGS
#error A_STICKY_STATIC_COMPILE_FLAGS
#endif

#ifdef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif
#ifdef B_STICKY_STATIC_COMPILE_FLAGS
#error B_STICKY_STATIC_COMPILE_FLAGS
#endif

#ifndef C_STICKY_COMPILE_FLAGS
#error C_STICKY_COMPILE_FLAGS
#endif
#ifndef C_STICKY_STATIC_COMPILE_FLAGS
#error C_STICKY_STATIC_COMPILE_FLAGS
#endif

#include "a.hpp"
#include "c.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(c_linktype() == "static");
  assert(a_linktype() == "static");
}

