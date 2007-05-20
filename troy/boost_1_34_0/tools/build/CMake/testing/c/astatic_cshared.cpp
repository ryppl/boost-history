#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif

#ifndef A_STATIC_COMPILE_REQUIREMENTS
#error A_STATIC_COMPILE_REQUIREMENTS
#endif

#ifdef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif

#ifdef B_STICKY_SHARED_COMPILE_FLAGS
#error B_STICKY_SHARED_COMPILE_FLAGS
#endif

#ifndef C_COMPILE_REQUIREMENTS
#error C_COMPILE_REQUIREMENTS
#endif

#ifndef C_SHARED_COMPILE_REQUIREMENTS
#error C_SHARED_COMPILE_REQUIREMENTS
#endif

#include "a.hpp"
#include "c.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(a_linktype() == "static");
  assert(c_linktype() == "shared");
}

