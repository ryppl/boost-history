#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif
#ifndef A_STATIC_COMPILE_REQUIREMENTS
#error A_STATIC_COMPILE_REQUIREMENTS
#endif

#ifdef B_COMPILE_REQUIREMENTS
#error B_COMPILE_REQUIREMENTS
#endif
#ifdef B_STATIC_COMPILE_REQUIREMENTS
#error B_STATIC_COMPILE_REQUIREMENTS
#endif

#ifndef C_COMPILE_REQUIREMENTS
#error C_COMPILE_REQUIREMENTS
#endif
#ifndef C_STATIC_COMPILE_REQUIREMENTS
#error C_STATIC_COMPILE_REQUIREMENTS
#endif

#include "a.hpp"
#include "c.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(c_linktype() == "static");
  assert(a_linktype() == "static");
}

