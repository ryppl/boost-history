#include "a.hpp"
#include <cassert>

#ifndef A_STATIC_COMPILE_REQUIREMENTS
#error A_STATIC_COMPILE_REQUIREMENTS
#endif

int main(int argc, char**argv)
{
  assert(a_linktype() == "static");
}

