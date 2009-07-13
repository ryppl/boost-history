// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_MULTIPLIER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_MULTIPLIER_HPP

#include <boost/mp_math/integer/detail/shifter.hpp>
#include <boost/mp_math/integer/detail/base/adder.hpp>
#include <boost/mp_math/integer/detail/base/divider.hpp>
#include <boost/mp_math/integer/detail/base/multiplier.hpp>

namespace boost {
namespace mp_math {
namespace detail {

template<
  class ApInt,
  bool IsSigned = ApInt::is_signed
>
struct multiplier;


template<class ApInt>
struct multiplier<ApInt, false>
{
  typedef ApInt                          int_type;
  typedef typename int_type::traits_type traits_type;
  typedef typename traits_type::ops_type ops_type;
  typedef typename int_type::digit_type  digit_type;
  typedef typename int_type::size_type   size_type;
  typedef shifter<int_type>              shifter_type;
  typedef base::adder<int_type>          base_adder_type;
  typedef base::divider<int_type>        base_divider_type;
  typedef base::multiplier<int_type>     base_multiplier_type;

  static void multiply_or_square(ApInt& z, const ApInt& x);
  static void multiply_or_square(ApInt& z, const ApInt& x, const ApInt& y);

  static void mul          (ApInt& z, const ApInt& x, const ApInt& y);
  static void long_mul     (ApInt& z, const ApInt& x, const ApInt& y);
  static void comba_mul    (ApInt& z, const ApInt& x, const ApInt& y);
  static void karatsuba_mul(ApInt& z, const ApInt& x, const ApInt& y);
  static void toom3_mul    (ApInt& z, const ApInt& x, const ApInt& y);

  static void multiply_by_2(ApInt& z);

  static void sqr          (ApInt& z);
  static void sqr          (ApInt& z, const ApInt& x);
  static void comba_sqr    (ApInt& z, const ApInt& x);
  static void karatsuba_sqr(ApInt& z, const ApInt& x);
  static void toom3_sqr    (ApInt& z, const ApInt& x);

private:

/*  static void long_or_comba_mul(ApInt& z, const ApInt& x, const ApInt& y,
                                bool_<false>)
  {
    long_mul(z, x, y);
  }

  static void long_or_comba_mul(ApInt& z, const ApInt& x, const ApInt& y,
                                bool_<true>)
  {
    comba_mul(z, x, y);
  }*/
};


template<class ApInt>
void multiplier<ApInt, false>::multiply_or_square(ApInt& z, const ApInt& x)
{
  if (&z == &x)
    sqr(z);
  else
    mul(z, z, x);
}

template<class ApInt>
void
multiplier<ApInt, false>::
multiply_or_square(ApInt& z, const ApInt& x, const ApInt& y)
{
  if (&x == &y)
    sqr(z, x);
  else
    mul(z, x, y);
}

template<class ApInt>
void multiplier<ApInt, false>::mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  // always multiply larger by smaller number
  const ApInt* a = &x;
  const ApInt* b = &y;
  if (a->size() < b->size())
    std::swap(a, b);

  if (b->size() == 1U)
  {
    if ((*b)[0] == 0U)
    {
      z.reserve(1);
      z[0] = 0;
      z.set_size(1);
    }
    else if ((*b)[0] == 1U)
      z = *a;
    else
      ApInt::template integral_ops<digit_type>::multiply(z, *a, (*b)[0]);

    return;
  }

  if (b->size() < traits_type::karatsuba_mul_threshold)
    comba_mul(z, *a, *b);
  else if (b->size() < traits_type::toom_mul_threshold)
    karatsuba_mul(z, *a, *b);
  else
    toom3_mul(z, *a, *b);
}

template<class ApInt>
inline
void multiplier<ApInt, false>::
long_mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  if ((&z != &x) && (&z != &y))
  {
    z.reserve(x.size() + y.size());
    base_multiplier_type::long_mul(z, x, y);
  }
  else
  {
    ApInt tmp;
    tmp.reserve(x.size() + y.size());
    base_multiplier_type::long_mul(tmp, x, y);
    swap(tmp, z);
  }
}

template<class ApInt>
inline
void multiplier<ApInt, false>::
comba_mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  if ((&z != &x) && (&z != &y))
  {
    z.reserve(x.size() + y.size());
    base_multiplier_type::comba_mul(z, x, y);
  }
  else
  {
    ApInt tmp;
    tmp.reserve(x.size() + y.size());
    base_multiplier_type::comba_mul(tmp, x, y);
    swap(tmp, z);
  }
}

// Divide and conquer multiplication using the Karatsuba algorithm.
//
// Let B represent the radix [e.g. 2^radix_bits] and let n represent half of
// the number of digits in min(a,b).
//
// a = x1 * B^n + x0
// b = y1 * B^n + y0
//
// Then, a * b => x1y1 * B^2n + ((x1 + x0)(y1 + y0) - (x0y0 + x1y1)) * B + x0y0
//
// Note that x1y1 and x0y0 are used twice and only need to be computed once.  So
// in total three half size (half # of digits) multiplications are performed,
// x0y0, x1y1 and (x1+y1)(x0+y0).
//
// Note that a multiplication of half the digits requires 1/4th the number of
// single precision multiplications so in total after one call 25% of the single
// precision multiplications are saved. Note also that the call to operator *
// can end up back in this function if x0, x1, y0, or y1 are above the
// threshold. This leads to O(N^lg(3)) or O(N^1.584) complexity. Generally
// though the overhead of this method doesn't pay off until a certain size
// (N ~ 80) is reached.
template<class ApInt>
void multiplier<ApInt, false>::
karatsuba_mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  assert(x.size() >= y.size());

  ApInt x0, x1, y0, y1, x0y0, x1y1;

  // B is the point at which we split both numbers
  const size_type B = y.size() / 2;

  // allocate memory
  x0.reserve(B);
  x1.reserve(x.size() + y.size());
  y0.reserve(B);
  y1.reserve(y.size() - B + 1);

  x0.set_size(B);
  y0.set_size(B);
  x1.set_size(x.size() - B);
  y1.set_size(y.size() - B);

  // copy digits over
  static const size_type s = sizeof(digit_type);
  std::memcpy(x0.digits(), x.digits(), s * B);
  std::memcpy(y0.digits(), y.digits(), s * B);
  std::memcpy(x1.digits(), x.digits() + B, s * (x.size() - B));
  std::memcpy(y1.digits(), y.digits() + B, s * (y.size() - B));

  x0.clamp();
  y0.clamp();

  // now evaluate the term
  // x1y1 * B**2n + ((x1 + x0)(y1 + y0) - (x0y0 + x1y1)) * B + x0y0

  mul(x0y0, x0, y0);
  mul(x1y1, x1, y1);

  // tmp = (x1 + x0) * (y1 + y0)
  x1 += x0;
  y1 += y0;
  // we don't need a tmp just reuse x1
  mul(x1, x1, y1);

  // tmp -= (x0y0 + x1y1);
  base_adder_type::subtract_smaller_magnitude(x1, x0y0);
  base_adder_type::subtract_smaller_magnitude(x1, x1y1);

  // shift by B
  if (x1 != digit_type(0))
    shifter_type::shift_digits_left(x1, B);
  if (x1 != digit_type(0))
    shifter_type::shift_digits_left(x1y1, B * 2);

  x1y1 += x1;
  x1y1 += x0y0;
  z.swap(x1y1);
}

// multiplication using the Toom-Cook 3-way algorithm
//
// Much more complicated than Karatsuba but has a lower
// asymptotic running time of O(N**1.464). This algorithm is
// only particularly useful on VERY large inputs
// (we're talking 1000s of digits here...).
template<class ApInt>
void multiplier<ApInt, false>::
toom3_mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  assert(x.size() >= y.size());

  const size_type B = y.size() / 3;

  // a = a2 * B**2 + a1 * B + a0
  ApInt a0(x);
  base_divider_type::modulo_pow2(a0, traits_type::radix_bits * B);
  ApInt a1(x);
  shifter_type::shift_digits_right(a1, B);
  base_divider_type::modulo_pow2(a1, traits_type::radix_bits * B);
  ApInt a2(x);
  shifter_type::shift_digits_right(a2, B*2);

  // b = b2 * B**2 + b1 * B + b0
  ApInt b0(y);
  base_divider_type::modulo_pow2(b0, traits_type::radix_bits * B);
  ApInt b1(y);
  shifter_type::shift_digits_right(b1, B);
  base_divider_type::modulo_pow2(b1, traits_type::radix_bits * B);
  ApInt b2(y);
  shifter_type::shift_digits_right(b2, B*2);

  // w0 = a0*b0
  const ApInt w0(a0 * b0);

  // w4 = a2 * b2
  ApInt w4 = a2 * b2;

  // w1 = (a2 + 2(a1 + 2a0))(b2 + 2(b1 + 2b0))
  ApInt tmp1 = a0;
  multiply_by_2(tmp1);
  tmp1 += a1;
  multiply_by_2(tmp1);
  tmp1 += a2;

  ApInt tmp2 = b0;
  multiply_by_2(tmp2);
  tmp2 += b1;
  multiply_by_2(tmp2);
  tmp2 += b2;

  ApInt w1 = tmp1 * tmp2;

  // w3 = (a0 + 2(a1 + 2a2))(b0 + 2(b1 + 2b2))
  tmp1 = a2;
  multiply_by_2(tmp1);
  tmp1 += a1;
  multiply_by_2(tmp1);
  tmp1 += a0;

  tmp2 = b2;
  multiply_by_2(tmp2);
  tmp2 += b1;
  multiply_by_2(tmp2);
  tmp2 += b0;

  ApInt w3 = tmp1 * tmp2;

  // w2 = (a2 + a1 + a0)(b2 + b1 + b0)
  tmp1 = a2 + a1;
  tmp1 += a0;
  tmp2 = b2 + b1;
  tmp2 += b0;
  ApInt w2 = tmp1 * tmp2;

  // now solve the matrix
  //
  // 0  0  0  0  1
  // 1  2  4  8  16
  // 1  1  1  1  1
  // 16 8  4  2  1
  // 1  0  0  0  0
  //
  // using 12 subtractions, 4 shifts,
  // 2 small divisions and 1 small multiplication

  // r1 - r4
  w1 -= w4;
  // r3 - r0
  w3 -= w0;
  // r1/2
  base_divider_type::divide_by_2(w1);
  // r3/2
  base_divider_type::divide_by_2(w3);
  // r2 - r0 - r4
  w2 -= w0;
  w2 -= w4;
  // r1 - r2
  w1 -= w2;
  // r3 - r2
  w3 -= w2;
  // r1 - 8r0
  tmp1 = w0 << 3;
  w1 -= tmp1;
  // r3 - 8r4
  tmp1 = w4 << 3;
  w3 -= tmp1;
  // 3r2 - r1 - r3
  w2 *= digit_type(3);
  w2 -= w1;
  w2 -= w3;
  // r1 - r2
  w1 -= w2;
  // r3 - r2
  w3 -= w2;
  // r1/3
  base_divider_type::divide_by_3(w1);
  // r3/3
  base_divider_type::divide_by_3(w3);

  // at this point shift W[n] by B*n
  if (w1 != digit_type(0))
    shifter_type::shift_digits_left(w1, 1 * B);
  if (w2 != digit_type(0))
    shifter_type::shift_digits_left(w2, 2 * B);
  if (w3 != digit_type(0))
    shifter_type::shift_digits_left(w3, 3 * B);
  if (w4 != digit_type(0))
    shifter_type::shift_digits_left(w4, 4 * B);

  z = w0 + w1;
  tmp1 = w2 + w3;
  tmp1 += w4;
  z += tmp1;
}

template<class ApInt>
void multiplier<ApInt, false>::multiply_by_2(ApInt& z)
{
  z.reserve(z.size() + 1);

  const digit_type carry =
    ops_type::multiply_by_two(z.digits(), z.digits(), z.size());

  if (carry)
    z.push(carry);
}


template<class ApInt>
void multiplier<ApInt, false>::sqr(ApInt& z, const ApInt& x)
{
  if (x.size() < ApInt::traits_type::karatsuba_sqr_threshold)
    comba_sqr(z, x);
  else if (x.size() < ApInt::traits_type::toom_sqr_threshold)
    karatsuba_sqr(z, x);
  else
    toom3_sqr(z, x);
}

template<class ApInt>
void multiplier<ApInt, false>::sqr(ApInt& z)
{
  //TODO resurrect optimization here from mp_math_v04
  // if (z.size() < 16)
  //   comba_sqr to digit array on stack then memcpy over to z;
  // else
  //   ApInt tmp;
  //   sqr(tmp, z);
  ApInt tmp;
  sqr(tmp, z);
  z.swap(tmp);
}

template<class ApInt>
inline
void multiplier<ApInt, false>::comba_sqr(ApInt& z, const ApInt& x)
{
  z.reserve(x.size() + x.size());
  base_multiplier_type::comba_sqr(z, x);
}

// Karatsuba squaring, computes b = a*a using three half size squarings
//
// See comments of karatsuba_mul for details. It is essentially the same
// algorithm but merely tuned to perform recursive squarings.
//
// a    = x1 * B**n + x0
// a**2 = x1x1 * B**2n + 2*x0x1 * B**n + x0x0
// where
// 2*x0x1 = 1) x1x1 + x0x0 - (x1 - x0)**2   or
//          2) (x0 + x1)**2 - (x0x0 + x1x1)
// we use version 1)
// version 2) may use one less temporary?
// a**2 = x1x1 * B**2n + (x1x1 + x0x0 - (x1 - x0)**2) * B**n + x0x0
// TODO revert!
template<class ApInt>
void multiplier<ApInt, false>::karatsuba_sqr(ApInt& z, const ApInt& x)
{
  typedef typename ApInt::digit_type digit_type;

  ApInt x0, x1, tmp, tmp2, x0x0, x1x1;

  const typename ApInt::size_type B = x.size() / 2;

  x0.reserve(B);
  x1.reserve(x.size() - B);

  x0x0.reserve(B * 2);
  x1x1.reserve((x.size() - B) * 2);

  // now shift the digits
  std::memcpy(x0.digits(), x.digits(), B * sizeof(digit_type));
  std::memcpy(x1.digits(), x.digits() + B, (x.size() - B) * sizeof(digit_type));

  x0.set_size(B);
  x1.set_size(x.size() - B);

  x0.clamp();

  sqr(x0x0, x0);
  sqr(x1x1, x1);

  tmp = x1x1;
  tmp += x0x0;

  tmp2 = x1;
  tmp2 -= x0;
  sqr(tmp2);

  base_adder_type::subtract_smaller_magnitude(tmp, tmp2);

  if (x1x1 != digit_type(0))
    shifter_type::shift_digits_left(x1x1, B * 2);
  if (tmp != digit_type(0))
    shifter_type::shift_digits_left(tmp, B);

  x1x1 += tmp;
  x1x1 += x0x0;

  z.swap(x1x1);
}

// squaring using Toom-Cook 3-way algorithm
template<class ApInt>
void multiplier<ApInt, false>::toom3_sqr(ApInt& z, const ApInt& x)
{
  ApInt w0, w1, w2, w3, w4, tmp1, a0, a1, a2;

  const typename ApInt::size_type B = x.size() / 3;

  /* a = a2 * B**2 + a1 * B + a0 */
  a0 = x;
  base_divider_type::modulo_pow2(a0, traits_type::radix_bits * B);

  a1 = x;
  shifter_type::shift_digits_right(a1, B);
  base_divider_type::modulo_pow2(a1, traits_type::radix_bits * B);

  a2 = x;
  shifter_type::shift_digits_right(a2, B * 2);

  /* w0 = a0*a0 */
  w0 = a0;
  sqr(w0);

  /* w4 = a2 * a2 */
  w4 = a2;
  sqr(w4);

  /* w1 = (a2 + 2(a1 + 2a0))**2 */
  w1 = a0;
  multiply_by_2(w1);
  w1 += a1;
  multiply_by_2(w1);
  w1 += a2;
  sqr(w1);

  /* w3 = (a0 + 2(a1 + 2a2))**2 */
  w3 = a2;
  multiply_by_2(w3);
  w3 += a1;
  multiply_by_2(w3);
  w3 += a0;
  sqr(w3);

  /* w2 = (a2 + a1 + a0)**2 */
  w2 = a1 + a2;
  w2 += a0;
  sqr(w2);

  /* now solve the matrix

     0  0  0  0  1
     1  2  4  8  16
     1  1  1  1  1
     16 8  4  2  1
     1  0  0  0  0

     using 12 subtractions, 4 shifts, 2 small divisions and 1 small
     multiplication.
   */

  /* r1 - r4 */
  w1 -= w4;
  /* r3 - r0 */
  w3 -= w0;
  /* r1/2 */
  base_divider_type::divide_by_2(w1);
  /* r3/2 */
  base_divider_type::divide_by_2(w3);
  /* r2 - r0 - r4 */
  w2 -= w0;
  w2 -= w4;
  /* r1 - r2 */
  w1 -= w2;
  /* r3 - r2 */
  w3 -= w2;
  /* r1 - 8r0 */
  tmp1 = w0;
  tmp1 <<= 3;
  w1 -= tmp1;
  /* r3 - 8r4 */
  tmp1 = w4;
  tmp1 <<= 3;
  w3 -= tmp1;
  /* 3r2 - r1 - r3 */
  w2 *= digit_type(3);
  w2 -= w1;
  w2 -= w3;
  /* r1 - r2 */
  w1 -= w2;
  /* r3 - r2 */
  w3 -= w2;
  /* r1/3 */
  base_divider_type::divide_by_3(w1);
  /* r3/3 */
  base_divider_type::divide_by_3(w3);
  /* at this point shift W[n] by B*n */
  if (w1 != digit_type(0))
    shifter_type::shift_digits_left(w1, 1 * B);
  if (w2 != digit_type(0))
    shifter_type::shift_digits_left(w2, 2 * B);
  if (w3 != digit_type(0))
    shifter_type::shift_digits_left(w3, 3 * B);
  if (w4 != digit_type(0))
    shifter_type::shift_digits_left(w4, 4 * B);

  z = w0 + w1;
  tmp1 = w2 + w3;
  tmp1 += w4;
  z += tmp1;
}

/*template<typename D, typename W, typename S>
void karatsuba_mul(mp_int_base<D,W,S>& z,
                   const mp_int_base<D,W,S>& x,
                   const mp_int_base<D,W,S>& y)
{
  typedef D digit_type;
  typedef S size_type;

  // B is the point at which we split both numbers
  const size_type B = std::min(x.size(), y.size()) / 2;

  mp_int_base_type x0(x.digits(), B, B);
  mp_int_base_type y0(y.digits(), B, B);
  mp_int_base_type x1(x.digits() + B, x.size() - B, x.size() - B);
  mp_int_base_type y1(y.digits() + B, y.size() - B, y.size() - B);

  x0.clamp();
  y0.clamp();

  // now evaluate the term
  // x1y1 * B**2n + ((x1 + x0)(y1 + y0) - (x0y0 + x1y1)) * B + x0y0

  mp_int_base x0y0(memory, B + 1);
  multiply(x0y0, x0, y0);

  mp_int_base x1y1(memory, B*2 + B + 1);
  multiply(x1y1, x1, y1);

  // c = (x1 + x0) * (y1 + y0)
  add_magnitude(a, x1, x0);
  add_magnitude(b, y1, y0);
  multiply(c, a, b);

  // c -= (x0y0 + x1y1);
  sub_smaller_magnitude(c, x0y0);
  sub_smaller_magnitude(c, x1y1);

  // shift by B
  shift_digits_left(x1, B);
  shift_digits_left(x1y1, B * 2);

  x1y1.add_magnitude(x1y1, x1);
  x1y1.add_magnitude(x1y1, x0y0);
  z.swap(x1y1);

  //////////////////////////////////////////////////////////

  size_type smallest_B = B;
  while (smallest_B > karatsuba_mul_cutoff)
    smallest_B /= 2;

  for (size_type i = 0; i < std::min(x.size(), y.size()); i += smallest_B)
  {
    mp_int_base_type x0(x.digits() + B*i, B, B);
    mp_int_base_type y0(y.digits() + B*i, B, B);
    mp_int_base_type x1(x.digits() + B, x.size() - B, x.size() - B);
    mp_int_base_type y1(y.digits() + B, y.size() - B, y.size() - B);

  }
}*/



template<class ApInt>
struct multiplier<ApInt, true>
{
  typedef ApInt                          int_type;
  typedef typename int_type::traits_type traits_type;
  typedef typename traits_type::ops_type ops_type;
  typedef typename int_type::digit_type  digit_type;
  typedef typename int_type::size_type   size_type;

  static void multiply_or_square(ApInt& z, const ApInt& x);
  static void multiply_or_square(ApInt& z, const ApInt& x, const ApInt& y);

  static void mul(ApInt& z, const ApInt& x, const ApInt& y);

  static void multiply_by_2(ApInt& z)
  {
    multiplier<ApInt, false>::multiply_by_2(z);
  }

  static void sqr(ApInt& z)
  {
    multiplier<ApInt, false>::sqr(z);
    z.set_sign_bit(0);
  }

  static void sqr(ApInt& z, const ApInt& x)
  {
    multiplier<ApInt, false>::sqr(z, x);
    z.set_sign_bit(0);
  }
};


template<class ApInt>
void multiplier<ApInt, true>::multiply_or_square(ApInt& z, const ApInt& x)
{
  if (&z == &x)
    sqr(z);
  else
    mul(z, z, x);
}

template<class ApInt>
void
multiplier<ApInt, true>::
multiply_or_square(ApInt& z, const ApInt& x, const ApInt& y)
{
  if (&x == &y)
    sqr(z, x);
  else
    mul(z, x, y);
}

template<class ApInt>
void multiplier<ApInt, true>::mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  // always multiply larger by smaller number
  const ApInt* a = &x;
  const ApInt* b = &y;
  if (a->size() < b->size())
    std::swap(a, b);

  if (b->size() == 1U)
  {
    if ((*b)[0] == 0U)
    {
      z.reserve(1);
      z[0] = 0;
      z.set_size(1);
      z.set_sign_bit(0);
    }
    else
    {
      if ((*b)[0] == 1U)
        z = *a;
      else
        ApInt::template integral_ops<digit_type>::multiply(z, *a, (*b)[0]);
      z.set_sign_bit(x.sign_bit() ^ y.sign_bit());
    }
  }
  else
  {
    const bool s = x.sign_bit() ^ y.sign_bit();

    if (b->size() < traits_type::karatsuba_mul_threshold)
      multiplier<ApInt, false>::comba_mul(z, *a, *b);
    else if (b->size() < traits_type::toom_mul_threshold)
      multiplier<ApInt, false>::karatsuba_mul(z, *a, *b);
    else
      multiplier<ApInt, false>::toom3_mul(z, *a, *b);

    z.set_sign_bit(s);
  }
}



} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

