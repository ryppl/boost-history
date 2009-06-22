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
    (1 << (static_cast<digit_type>(n % traits_type::radix_bits))) - 1;

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
  typedef typename traits_type::word_type word_type;

  // b = 2^radix_bits / 3
  static const word_type b = (word_type(1) << traits_type::radix_bits) / 3;

  word_type w = 0;
  for (typename ApInt::reverse_iterator d = z.rbegin(); d != z.rend(); ++d)
  {
    w = (w << static_cast<word_type>(traits_type::radix_bits))
      | static_cast<word_type>(*d);

    word_type t;
    if (w >= 3U)
    {
      // multiply w by [1/3]
      t = (w * b) >> static_cast<word_type>(traits_type::radix_bits);

      // now subtract 3 * [w/3] from w, to get the remainder
      w -= t+t+t;

      // fixup the remainder as required since the optimization is not exact.
      while (w >= 3U)
      {
        t += 1;
        w -= 3;
      }
    }
    else
      t = 0;

    *d = static_cast<digit_type>(t);
  }

  z.clamp(); // TODO clamp_high_digit should suffice
}


} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif
