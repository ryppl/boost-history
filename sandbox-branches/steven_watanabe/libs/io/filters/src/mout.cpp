#include <boost/io/filters/mout.hpp>
#include <iostream>
  marg_ostream&
mout(void)
  { static marg_ostream x(std::cout.rdbuf())
  ; return x
  ;}
