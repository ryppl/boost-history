// DO see
#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif
#ifndef B_COMPILE_REQUIREMENTS
#error B_COMPILE_REQUIREMENTS
#endif
#ifndef C_COMPILE_REQUIREMENTS
#error C_COMPILE_REQUIREMENTS
#endif
#ifndef D_COMPILE_REQUIREMENTS
#error D_COMPILE_REQUIREMENTS
#endif

#ifdef D_COMPILE_FLAGS
#error D_COMPILE_FLAGS
#endif
#ifdef D_SHARED_COMPILE_FLAGS
#error D_SHARED_COMPILE_FLAGS
#endif

#include "a.hpp"
#include "b.hpp"
#include "c.hpp"
#include "d.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(a_linktype() == "shared");
  assert(b_linktype() == "shared");
  assert(c_linktype() == "shared");
  assert(d_linktype() == "shared");
}

