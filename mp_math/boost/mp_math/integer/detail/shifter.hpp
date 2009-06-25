// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_SHIFTER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_SHIFTER_HPP

#include <boost/mp_math/integer/detail/base/divider.hpp>
#include <boost/mp_math/integer/detail/base/shifter.hpp>

namespace boost {
namespace mp_math {
namespace detail {

template<class ApInt>
struct shifter
{
  typedef ApInt                          int_type;
  typedef base::shifter<int_type>        base_shifter_type;
  typedef typename int_type::traits_type traits_type;
  typedef typename traits_type::ops_type ops_type;
  typedef typename int_type::digit_type  digit_type;
  typedef typename int_type::size_type   size_type;

  static void shift_bits_left (ApInt& z, size_type n);
  static void shift_bits_right(ApInt& z, size_type n)
  {
    base_shifter_type::shift_bits_right(z, n);
  }

  // stores remainder in r
  static void shift_bits_right(ApInt& z, ApInt& r, size_type n);

  static void shift_digits_left(ApInt& z, size_type n)
  {
    z.reserve(z.size() + n);
    base_shifter_type::shift_digits_left(z, n);
  }

  static void shift_digits_right(ApInt& z, size_type n)
  {
    base_shifter_type::shift_digits_right(z, n);
  }
};


template<class ApInt>
void shifter<ApInt>::shift_bits_left(ApInt& z, size_type n)
{
  assert(z != digit_type(0));

  if (n)
  {
    z.reserve(z.size() + n/traits_type::radix_bits + 1);

    // shift by as many digits in the bit count
    if (n >= traits_type::radix_bits)
      base_shifter_type::shift_digits_left(z, n / traits_type::radix_bits);

    // shift any bit count < radix_bits
    const digit_type b = static_cast<digit_type>(n % traits_type::radix_bits);

    if (b)
    {
      const digit_type carry =
        ops_type::shift_bits_left(z.digits(), z.size(), b);

      if (carry)
        z.push(carry);
    }
  }
}

template<class ApInt>
void shifter<ApInt>::shift_bits_right(ApInt& z, ApInt& remainder, size_type n)
{
  if (!n)
  {
    remainder = digit_type(0);
    return;
  }

  remainder = z;
  base::divider<ApInt>::modulo_pow2(remainder, n);

  base_shifter_type::shift_bits_right(z, n);
}


} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

