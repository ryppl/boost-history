// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_BITMANIPULATOR_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_BITMANIPULATOR_HPP

namespace boost {
namespace mp_math {
namespace detail {
namespace base {


template<
  class ApInt1,
  class ApInt2,
  int Cmp = ApInt1::traits_type::radix_bits < ApInt2::traits_type::radix_bits ? -1
          : ApInt1::traits_type::radix_bits > ApInt2::traits_type::radix_bits ? : 1
          : 0
>
struct bit_copier;


template<class ApInt1, class ApInt2>
struct bit_copier<ApInt1, ApInt2, -1>
{
  typedef typename ApInt1::size_type size_type1;
  typedef typename ApInt2::size_type size_type2;
  typedef typename ApInt1::digit_type digit_type1;
  typedef typename ApInt2::digit_type digit_type2;

  static const unsigned radix_bits1 = ApInt1::traits_type::radix_bits;
  static const unsigned radix_bits2 = ApInt2::traits_type::radix_bits;

  static void copy_bits(ApInt1&       dst, size_type1 dst_offset,
                        const ApInt2& src, size_type2 src_offset, size_type1 n);
};


template<class ApInt1, class ApInt2>
struct bit_copier<ApInt1, ApInt2, 1>
{
  typedef typename ApInt1::size_type size_type1;
  typedef typename ApInt2::size_type size_type2;
  typedef typename ApInt1::digit_type digit_type1;
  typedef typename ApInt2::digit_type digit_type2;

  static const unsigned radix_bits1 = ApInt1::traits_type::radix_bits;
  static const unsigned radix_bits2 = ApInt2::traits_type::radix_bits;

  static void copy_bits(ApInt1&       dst, size_type1 dst_offset,
                        const ApInt2& src, size_type2 src_offset, size_type1 n);
};


template<class ApInt1, class ApInt2>
struct bit_copier<ApInt1, ApInt2, 0>
{
  typedef typename ApInt1::size_type size_type1;
  typedef typename ApInt2::size_type size_type2;
  typedef typename ApInt1::digit_type digit_type1;
  typedef typename ApInt2::digit_type digit_type2;

  static const digit_type1 z1 = ~digit_type1(0);
  static const digit_type2 z2 = ~digit_type2(0);
  static const unsigned radix_bits = ApInt1::traits_type::radix_bits;

  static void copy_bits(ApInt1&       dst, size_type1 dst_offset,
                        const ApInt2& src, size_type2 src_offset, size_type1 n);
};


template<class ApInt1, class ApInt2>
void bit_copier<ApInt1, ApInt2, 0>::copy_bits(
    ApInt1&       dst, size_type1 dst_offset,
    const ApInt2& src, size_type2 src_offset, size_type1 num)
{
  if (!num)
    return;

  const size_type1 q1 = dst_offset / radix_bits;
  const size_type1 r1 = dst_offset % radix_bits;
  const size_type2 q2 = src_offset / radix_bits;
  const size_type2 r2 = src_offset % radix_bits;

  const digit_type1 r1_inv = radix_bits - r1;
  const digit_type2 r2_inv = radix_bits - r2;

  digit_type1* d       = dst.digits() + q1;
  const digit_type2* s = src.digits() + q2;

  // If the offset into the first destination digit is not zero, i.e. r1 ==
  // true, then we fill it up manually (normalize it) and then start the
  // normalized copy loop.
  if (!r1)
  {
    if (!r2)
    {
      const size_type1 num_digits = (num + (radix_bits - 1)) / radix_bits;
      std::memcpy(d, s, num_digits * sizeof(digit_type2));
      return;
    }
    else
      goto normalized_bitcopy;
  }
  else
  {
    if (num < r1_inv)
    {
      *d &= (z1 >> r1_inv) | (z1 << (r1_inv + num));
      *d |= ((*s >> r2) & ~(z2 << num)) << r1;
      return;
    }
    else
    {
      *d &= ~(z1 << r1);

      if (num < r2_inv)
      {
        *d |= ((*s >> r2) & ~(z2 << num)) << r1;
        return;
      }
      else
      {
        *d |= (*s >> r2) << r1;
        ++s;
        num -= r2_inv;
        *d |= (*s & ~(z2 << (radix_bits - num))) << (r1 + r2_inv);
        goto normalized_bitcopy;
      }
    }
  }

normalized_bitcopy:

  // fill one whole destination digit per iteration
  while (num >= radix_bits)
  {
    *d = *s >> r2;
    ++s;
    *d |= *s << r2_inv;
    ++d;
    num -= radix_bits;
  }

  // copy remaining bits manually
  if (num)
  {
    *d &= ~(z1 >> (radix_bits - num));

    if (num < r2_inv)
      *d |= (*s >> r2) & ~(z2 << num);
    else
    {
      *d |= (*s >> r2);
      ++s;
      num -= r2;
      *d |= (*s & (z2 >> (radix_bits - num))) << r2;
    }
  }
}



} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif
