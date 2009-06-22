// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mp_math/integer.hpp>

typedef boost::mp_math::integer<> int_type;


int main(int, char**)
{
  int_type x(2), y(0x257);

  x = pow(x, 101);

  x = modpow(x, y, int_type(257));

  while (y > 2)
  {
    x *= y;
    --y;
  }

  x = y + y + y * x % (int_type(12));

  for (int i = 0; i < 15; ++i)
  {
    x -= x * y;
    y += 15;
    x += i;
  };

  x *= -1;

  x = sqrt(x);

  // now x should be 0x66d74da264f9
  std::cout << std::hex << std::showbase << "The answer is " << x << "." << std::endl;


  return 0;
}



