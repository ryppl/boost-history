// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_DIVIDER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_DIVIDER_HPP


namespace boost {
namespace mp_math {
namespace detail {
namespace base {


template<class ApInt>
struct divider
{
  typedef typename ApInt::traits_type traits_type;
  typedef typename ApInt::digit_type  digit_type;
  typedef typename ApInt::size_type   size_type;

  // z = z % 2^n
  static void modulo_pow2(ApInt& z, size_type n);
  static void divide_by_2(ApInt& z);
  static void divide_by_3(ApInt& z);
};


template<class ApInt>
void divider<ApInt>::modulo_pow2(ApInt& z, size_type n)
{
  // if modulus >= z then return
  if (n >= z.size() * traits_type::radix_bits)
    return;

  // clear high bits
  const digit_type mask =
    (digit_type(1) << (static_cast<digit_type>(n % traits_type::radix_bits))) - 1;

  z[n / traits_type::radix_bits] &= mask;

  z.set_size(n / traits_type::radix_bits + 1);
  // TODO was z.clamp() hope it is correct now with the +1! check again!

  z.clamp();
  // TODO if (!z) z.set_sign_bit(0) is necessary here
}

template<class ApInt>
inline void divider<ApInt>::divide_by_2(ApInt& z)
{
  traits_type::ops_type::divide_by_two(z.digits(), z.digits(), z.size());

  z.clamp_high_digit();
}

// divide by three (based on routine from MPI and the GMP manual)
template<class ApInt>
inline void divider<ApInt>::divide_by_3(ApInt& z)
{
  ApInt::traits_type::ops_type::divide_by_3(z.digits(), z.size());

  z.clamp_high_digit();
}


} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif
