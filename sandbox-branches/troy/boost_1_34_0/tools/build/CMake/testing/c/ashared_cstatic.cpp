#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif
#ifndef A_SHARED_COMPILE_REQUIREMENTS
#error A_SHARED_COMPILE_REQUIREMENTS
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
  assert(a_linktype() == "shared");
  assert(c_linktype() == "static");
}

