// do NOT see
#ifdef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif
#ifdef A_STATIC_COMPILE_FLAGS
#error A_STATIC_COMPILE_FLAGS
#endif

#ifdef B_SHARED_COMPILE_FLAGS
#error B_SHARED_COMPILE_FLAGS
#endif
#ifdef C_SHARED_COMPILE_FLAGS
#error C_SHARED_COMPILE_FLAGS
#endif
#ifdef D_SHARED_COMPILE_FLAGS
#error D_SHARED_COMPILE_FLAGS
#endif

// DO see
#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif
#ifndef A_STATIC_COMPILE_REQUIREMENTS
#error A_STATIC_COMPILE_REQUIREMENTS
#endif
#ifndef B_COMPILE_REQUIREMENTS
#error B_COMPILE_REQUIREMENTS
#endif
#ifndef B_STATIC_COMPILE_REQUIREMENTS
#error B_STATIC_COMPILE_REQUIREMENTS
#endif
#ifndef C_COMPILE_REQUIREMENTS
#error C_COMPILE_REQUIREMENTS
#endif
#ifndef C_STATIC_COMPILE_REQUIREMENTS
#error C_STATIC_COMPILE_REQUIREMENTS
#endif
#ifndef D_COMPILE_REQUIREMENTS
#error D_COMPILE_REQUIREMENTS
#endif
#ifndef D_STATIC_COMPILE_REQUIREMENTS
#error D_STATIC_COMPILE_REQUIREMENTS
#endif

#include "a.hpp"
#include "b.hpp"
#include "c.hpp"
#include "d.hpp"
#include <cassert>
int main(int argc, char**argv)
{
  assert(a_linktype() == "static");
  assert(b_linktype() == "static");
  assert(c_linktype() == "static");
  assert(d_linktype() == "static");
}

