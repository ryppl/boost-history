// do NOT see
#ifdef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif
#ifdef A_STICKY_SHARED_COMPILE_FLAGS
#error A_STICKY_SHARED_COMPILE_FLAGS
#endif
#ifdef B_STICKY_STATIC_COMPILE_FLAGS
#error B_STICKY_STATIC_COMPILE_FLAGS
#endif
#ifdef C_STICKY_STATIC_COMPILE_FLAGS
#error C_STICKY_STATIC_COMPILE_FLAGS
#endif
#ifdef D_STICKY_STATIC_COMPILE_FLAGS
#error D_STICKY_STATIC_COMPILE_FLAGS
#endif

// DO see
#ifndef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif
#ifndef B_STICKY_SHARED_COMPILE_FLAGS
#error B_STICKY_SHARED_COMPILE_FLAGS
#endif
#ifndef C_STICKY_COMPILE_FLAGS
#error C_STICKY_COMPILE_FLAGS
#endif
#ifndef C_STICKY_SHARED_COMPILE_FLAGS
#error C_STICKY_SHARED_COMPILE_FLAGS
#endif

#ifndef D_STICKY_COMPILE_FLAGS
#error D_STICKY_COMPILE_FLAGS
#endif
#ifndef D_STICKY_SHARED_COMPILE_FLAGS
#error D_STICKY_SHARED_COMPILE_FLAGS
#endif

#include "b.hpp"
#include "c.hpp"
#include "d.hpp"
#include <cassert>
int main(int argc, char**argv)
{
  assert(b_linktype() == "shared");
  assert(c_linktype() == "shared");
  assert(d_linktype() == "shared");
}

