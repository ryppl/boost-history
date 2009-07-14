// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_PRINT_DIGITS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_PRINT_DIGITS_HPP

#include <iostream>
#include <boost/cstdint.hpp>

namespace boost {
namespace mp_math {
namespace detail {
namespace base {

// Prints raw digit values in hex format to std::cout.
// If argument all is true then it also prints the digits that were allocated
// but are currently not in use.
//
// Sample output size = 4, capacity = 16:
// 4{0,0,0,1}12{0,0,0,0,0,0,0,0,0,0,0,0}

template<class ApInt>
void print_digits(const ApInt& x, bool all)
{
  typedef typename ApInt::size_type size_type;

  const std::ios::fmtflags flags = std::cout.flags();

  std::cout << x.size() << "{" << std::hex;
  for (size_type i = 0; i < x.size(); ++i)
  {
    std::cout << static_cast<uintmax_t>(x[i]);
    if (i < x.size()  - 1)
      std::cout << ",";
  }
  std::cout << "}";

  if (all)
  {
    std::cout << std::dec << x.capacity() - x.size() << "{" << std::hex;
    for (size_type i = x.size(); i < x.capacity(); ++i)
    {
      std::cout << static_cast<uintmax_t>(x[i]);
      if (i < x.capacity()  - 1)
        std::cout << ",";
    }
    std::cout << "}";
  }
  std::cout << "\n";
  std::cout.flags(flags);
}


} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

