// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_SHIFTER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_SHIFTER_HPP

namespace boost {
namespace mp_math {
namespace detail {
namespace base {

template<class ApInt, bool IsSigned=ApInt::is_signed>
struct shifter;


template<class ApInt>
struct shifter<ApInt, false>
{
  typedef ApInt                          int_type;
  typedef typename int_type::traits_type traits_type;
  typedef typename traits_type::ops_type ops_type;
  typedef typename int_type::digit_type  digit_type;
  typedef typename int_type::size_type   size_type;

  static void shift_bits_left (ApInt& z, size_type n);
  static void shift_bits_right(ApInt& z, size_type n);

  static void shift_digits_left(ApInt& z, size_type n);
  static void shift_digits_right(ApInt& z, size_type n);
};


template<class ApInt>
void shifter<ApInt,false>::shift_bits_left(ApInt& z, size_type n)
{
  assert(z != digit_type(0));

  if (n)
  {
    // shift by as many digits in the bit count
    if (n >= traits_type::radix_bits)
      shift_digits_left(z, n / traits_type::radix_bits);

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
void shifter<ApInt,false>::shift_bits_right(ApInt& z, size_type n)
{
  if (n)
  {
    // shift by as many digits in the bit count
    if (n >= traits_type::radix_bits)
      shift_digits_right(z, n / traits_type::radix_bits);

    // shift any bit count < valid_bits
    const digit_type b = n % traits_type::radix_bits;
    if (b)
      ops_type::shift_bits_right(z.digits(), z.size(), b);

    z.clamp_high_digit();
  }
}

// {A,B,C,D,E} shifted left by 2 digits becomes
// {0,0,A,B,C,D,E}
template<class ApInt>
void shifter<ApInt,false>::shift_digits_left(ApInt& z, size_type n)
{
  assert(z != digit_type(0));

  if (n == 0)
    return;

  std::memmove(z.digits() + n, z.digits(), z.size() * sizeof(digit_type));

  // zero the lower digits
  std::memset(z.digits(), 0, n * sizeof(digit_type));

  z.set_size(z.size() + n);
}

// {A,B,C,D,E} shifted right by 2 digits becomes
// {C,D,E}
template<class ApInt>
void shifter<ApInt,false>::shift_digits_right(ApInt& z, size_type n)
{
  if (n == 0)
    return;

  if (z.size() <= n)
  {
    z = digit_type(0);
    return;
  }

  std::memmove(z.digits(), z.digits() + n, (z.size() - n) * sizeof(digit_type));

  z.set_size(z.size() - n);
}


template<class ApInt>
struct shifter<ApInt, true>
{
  typedef ApInt                          int_type;
  typedef typename int_type::size_type   size_type;

  static void shift_bits_left(ApInt& z, size_type n)
  {
    shifter<ApInt, false>::shift_bits_left(z, n);
  }

  static void shift_bits_right(ApInt& z, size_type n)
  {
    shifter<ApInt, false>::shift_bits_right(z, n);
    if (!z)
      z.set_sign_bit(0);
  }

  static void shift_digits_left(ApInt& z, size_type n)
  {
    shifter<ApInt, false>::shift_digits_left(z, n);
  }

  static void shift_digits_right(ApInt& z, size_type n)
  {
    shifter<ApInt, false>::shift_digits_right(z, n);
  }
};



} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

