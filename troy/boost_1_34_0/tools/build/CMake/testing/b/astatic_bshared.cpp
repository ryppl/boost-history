#ifndef A_STATIC_COMPILE_REQUIREMENTS
#error A_STATIC_COMPILE_REQUIREMENTS
#endif

#ifndef B_COMPILE_REQUIREMENTS
#error B_COMPILE_REQUIREMENTS
#endif

#ifndef B_SHARED_COMPILE_REQUIREMENTS
#error B_SHARED_COMPILE_REQUIREMENTS
#endif

#include "a.hpp"
#include "b.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(b_linktype() == "shared");
  assert(a_linktype() == "static");
}

