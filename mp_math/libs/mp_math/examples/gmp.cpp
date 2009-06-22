// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mp_math/integer.hpp>
#include <boost/mp_math/gmp.hpp>

typedef boost::mp_math::integer<boost::mp_math::gmp_integer<> > int_type;

int main(int, char**)
{
  int_type x, y;

  x = 0x1223;
  y = "0x2984389248923984ababab";

  const int_type z = pow(y, x);

  std::cout << z << std::endl;

  return 0;
}

