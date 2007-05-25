#ifndef A_SHARED_COMPILE_REQUIREMENTS
#error A_SHARED_COMPILE_REQUIREMENTS
#endif

#include "a.hpp"
#include <cassert>

int main(int argc, char**argv)
{
  assert(a_linktype() == "shared");
}

