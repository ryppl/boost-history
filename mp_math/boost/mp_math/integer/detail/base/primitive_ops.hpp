// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_PRIMITIVE_OPS
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_PRIMITIVE_OPS

#include <cassert>
#include <cstring>
#include <algorithm>
#include <boost/cstdint.hpp>
#include <iostream>

namespace boost {
namespace mp_math {
namespace detail {
namespace base {


// this struct contains some basic arithmetic algorithms
// which can be implemented via assembly rather easily

template<typename DigitT, typename SizeT>
struct basic_primitive_ops
{
  typedef DigitT digit_type;
  typedef SizeT  size_type;

  static const digit_type digit_bits = std::numeric_limits<digit_type>::digits;

  // ADD ------------------------------------

  // z = x + y, returns carry
  static digit_type add(digit_type& z, digit_type x, digit_type y);

  // z += x, returns carry
  static digit_type add(digit_type& z, digit_type x);

  // add y to the digits in x and store result in z
  // xlen must be > 0
  // returns: the last carry (it will not get stored in z)
  static digit_type add_single_digit(digit_type* z,
                                     const digit_type* x, size_type xlen,
                                     digit_type y);

  // z = x + y, returns last carry
  static digit_type add_digits(digit_type* z,
                               const digit_type* x,
                               const digit_type* y,
                               size_type num);

  // ripples the carry c up through n digits of x and stores results in z
  // returns the number of digits the carry rippled through and stores the last
  // carry in c. If there isn't a last carry then c will be 0.
  static size_type ripple_carry(digit_type* z,
                                const digit_type* x,
                                size_type n,
                                digit_type& c);

  // z = x + y, where xlen >= ylen
  // returns last carry
  static digit_type add_smaller_magnitude(digit_type* z,
                                          const digit_type* x, size_type xlen,
                                          const digit_type* y, size_type ylen);

  // SUB ------------------------------------

  // z = x - y, returns borrow
  static digit_type sub(digit_type& z, digit_type x, digit_type y);

  // z -= x, returns borrow
  static digit_type sub(digit_type& z, digit_type x);

  // subtracts x from the digits in y and store result in z
  static void subtract_single_digit(digit_type* z,
                                    const digit_type* x, size_type xlen,
                                    digit_type y);

  // z = x - y, returns last borrow
  static digit_type subtract_digits(digit_type* z,
                                    const digit_type* x,
                                    const digit_type* y,
                                    size_type num);

  // ripples the borrow up through n digits of x and stores results in z
  // returns the number of digits the borrow rippled through
  static size_type ripple_borrow(digit_type* z,
                                 const digit_type* x,
                                 size_type n,
                                 digit_type borrow);

  // z = x - y, where x >= y
  static void sub_smaller_magnitude(digit_type* z,
                                    const digit_type* x, size_type xlen,
                                    const digit_type* y, size_type ylen);

  // MUL ------------------------------------

  // z = x * y, returns high part of the product
  static digit_type mul(digit_type& z, digit_type x, digit_type y);

  // z *= x, returns high part of the product
  static digit_type mul(digit_type& z, digit_type x);

  // multiply y of length ylen with x and store result in z
  // returns: the last carry (it will not get stored in z)
  static digit_type multiply_by_digit(digit_type* z,
                                      const digit_type* x, size_type xlen,
                                      digit_type y);

  // z = x * 2
  static digit_type multiply_by_two(digit_type* z,
                                    const digit_type* x, size_type len);

  // z = x * y; precondition: xlen >= ylen
  static void long_mul(digit_type* z, const digit_type* x, size_type xlen,
                                      const digit_type* y, size_type ylen);

  // z = x * y; precondition: xlen >= ylen
  static void comba_mul(digit_type* z, const digit_type* x, size_type xlen,
                                       const digit_type* y, size_type ylen);

  // z = x * y; for numbers of the same size
  static void comba_mul(digit_type* z,
                        const digit_type* x,
                        const digit_type* y, size_type xylen);

  // z = x * y; precondition: xlen >= ylen and workspace must have at least ylen
  // digits this function is only used by the integral ops interaction code, it
  // allows x and z to be the same pointer.
  static void comba_mul(digit_type* z, const digit_type* x, size_type xlen,
                                       const digit_type* y, size_type ylen,
                                       digit_type* workspace);

  // computes the lower num digits of the product of x and y
  static void comba_mul_lo(digit_type* z, const digit_type* x, size_type xlen,
                                          const digit_type* y, size_type ylen,
                                          size_type num);

  // computes the high product of x and y without num least significant digits
  // basically the result is: z = (x*y) >> radix^num
  static void comba_mul_hi(digit_type* z, const digit_type* x, size_type xlen,
                                          const digit_type* y, size_type ylen,
                                          size_type num);

  // SQR ------------------------------------

  // z = x * x;
  static void comba_sqr(digit_type* z, const digit_type* x, size_type xlen);

  // MADD ------------------------------------

  // z = z + x * y, returns carry
  static digit_type multiply_add(digit_type& z,
                                 digit_type x, digit_type y);

  // z = z + x * y, returns carry
  static digit_type multiply_add(digit_type& z_hi, digit_type& z_lo,
                                 digit_type x, digit_type y);

  // z = z + x * y
  static digit_type multiply_add_digits(digit_type* z,
                                        const digit_type* x,
                                        digit_type y,
                                        size_type n);

  // DIV -------------------------------------

  // q = x / y
  static void div2by1(digit_type& q,
                      digit_type x_hi, digit_type x_lo, digit_type y);

  // z = x / 2
  static void divide_by_two(digit_type* z, const digit_type* x, size_type len);

  // z = x / y
  // returns remainder
  static digit_type divide_by_digit(digit_type* z,
                                    const digit_type* x, size_type xlen,
                                    digit_type y);

  static void divide_by_3(digit_type* z, size_type len);

  // SHIFT -----------------------------------

  // shifts x by n bits to the left, where n > 0 && n < digit_bits
  // returns carry
  static digit_type shift_bits_left(digit_type* x, size_type xlen, size_type n);

  // shifts x by n bits to the right, where n > 0 && n < digit_bits
  static void shift_bits_right(digit_type* x, size_type xlen, size_type n);

  // CMP -------------------------------------

  // returns  1 if x  > y
  // returns  0 if x == y
  // returns -1 if x  < y
  static int compare_magnitude(const digit_type* x, size_type xlen,
                               const digit_type* y, size_type ylen);

private:

  // divides half digits [x1, x2, x3] by [y1, y2] and returns remainder
  // this function is used by divide_by_digit
  static digit_type divide_half_digits(digit_type& q,
                                       digit_type x12, digit_type x3,
                                       digit_type y1, digit_type y2);
};



template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::add(digit_type& z, digit_type x, digit_type y)
{
  z = x + y;
  return z < x;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::add(digit_type& z, digit_type x)
{
  z += x;
  return z < x;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::add_single_digit(digit_type* z,
                                             const digit_type* x,
                                             size_type xlen,
                                             digit_type y)
{
  digit_type carry = add(*z++, *x++, y);

  while (carry && --xlen)
    carry = add(*z++, *x++, carry);

  return carry;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::add_digits(digit_type* z,
                                       const digit_type* x,
                                       const digit_type* y, size_type n)
{
  digit_type carry = 0;
  while (n--)
  {
    const digit_type c0 = add(*z, *x++, *y++);
    const digit_type c1 = add(*z++, carry);
    carry = c0 | c1;
  }

  return carry;
}

template<typename D, typename S>
inline
S basic_primitive_ops<D,S>::ripple_carry(digit_type* z,
                                         const digit_type* x,
                                         size_type n,
                                         digit_type& carry)
{
  size_type i = 0;

  for (; carry && (i < n); ++i)
    carry = add(*z++, *x++, carry);

  return i;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::add_smaller_magnitude(digit_type* z,
                                                  const digit_type* x,
                                                  size_type xlen,
                                                  const digit_type* y,
                                                  size_type ylen)
{
  digit_type carry = add_digits(z, x, y, ylen);

  size_type n = ripple_carry(z + ylen, x + ylen, xlen - ylen, carry);

  n += ylen;

  if (n < xlen && z != x)
    std::memcpy(z + n, x + n, sizeof(digit_type) * (xlen - n));

  return carry;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::sub(digit_type& z, digit_type x, digit_type y)
{
  z = x - y;
  return z > x;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::sub(digit_type& z, digit_type x)
{
  const digit_type tmp = z;
  z -= x;
  return z > tmp;
}

template<typename D, typename S>
inline
void
basic_primitive_ops<D,S>::subtract_single_digit(digit_type* z,
                                                const digit_type* y,
                                                size_type ylen,
                                                digit_type x)
{
  digit_type borrow = sub(*z++, *y++, x);

  while (borrow && --ylen)
    borrow = sub(*z++, *y++, borrow);
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::subtract_digits(digit_type* z,
                                            const digit_type* x,
                                            const digit_type* y,
                                            size_type n)
{
  digit_type borrow = 0;

  while (n--)
  {
    const digit_type b0 = sub(*z, *x++, *y++);
    const digit_type b1 = sub(*z++, borrow);
    borrow = b0 | b1;
  }

  return borrow;
}

template<typename D, typename S>
inline
S basic_primitive_ops<D,S>::ripple_borrow(digit_type* z,
                                          const digit_type* x,
                                          size_type n,
                                          digit_type borrow)
{
  size_type i = 0;

  for (; borrow && (i < n); ++i)
    borrow = sub(*z++, *x++, borrow);

  return i;
}

template<typename D, typename S>
inline
void basic_primitive_ops<D,S>::sub_smaller_magnitude(
    digit_type* z,
    const digit_type* x, size_type xlen,
    const digit_type* y, size_type ylen)
{
  const digit_type borrow = subtract_digits(z, x, y, ylen);

  size_type n = ripple_borrow(z + ylen, x + ylen, xlen - ylen, borrow);

  if (z != x)
  {
    n += ylen;
    std::memcpy(z + n, x + n, (xlen - n) * sizeof(digit_type));
  }
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::mul(digit_type& z_lo, digit_type x, digit_type y)
{
  static const digit_type lo_mask = (digit_type(1) << digit_bits / 2) - 1;

  const digit_type x_lo = x & lo_mask;
  const digit_type x_hi = x >> digit_bits/2;
  const digit_type y_lo = y & lo_mask;
  const digit_type y_hi = y >> digit_bits/2;

  const digit_type z0 = x_lo * y_lo;
  const digit_type z1 = x_lo * y_hi;
  const digit_type z2 = x_hi * y_lo;
  const digit_type z3 = x_hi * y_hi;

  digit_type z12 = z1 + (z0 >> digit_bits/2);
  const digit_type carry = add(z12, z2);

  z_lo = (z12 << digit_bits/2) + (z0 & lo_mask);

  const digit_type z_hi = z3 + (carry << digit_bits/2) + (z12 >> digit_bits/2);

  return z_hi;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::mul(digit_type& z, digit_type x)
{
  return mul(z, z, x);
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::multiply_by_digit(digit_type* z,
                                              const digit_type* y,
                                              size_type ylen,
                                              digit_type x)
{
  digit_type carry = 0;

  while (ylen--)
  {
    const digit_type tmp = mul(*z, *y++, x);
    carry = tmp + add(*z++, carry);
  }

  return carry;
}


template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::multiply_by_two(digit_type* z,
                                            const digit_type* x, size_type n)
{
  digit_type carry = 0;

  while (n--)
  {
    // get carry bit for next iteration
    const digit_type r = *x >> (digit_bits - 1);

    *z++ = (*x++ << 1) | carry;

    carry = r;
  }

  return carry;
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::long_mul(digit_type* z,
                                   const digit_type* x, size_type xlen,
                                   const digit_type* y, size_type ylen)
{
  assert(xlen >= ylen);
  z[xlen] = multiply_by_digit(z++, x, xlen, *(y++));
  --ylen;
  while(--ylen)
    z[xlen] = multiply_add_digits(z++, x, xlen, *(y++));
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::comba_mul(digit_type* z,
                                    const digit_type* x, size_type xlen,
                                    const digit_type* y, size_type ylen)
{
  assert(xlen >= ylen);

  digit_type acc_hi = 0;  // accumulator for each column
  digit_type acc_lo = 0;
  digit_type carry_hi = 0;
  digit_type carry_lo = 0;

  // phase 1
  for (size_type i = 0; i < ylen; ++i)
  {
    const digit_type* a = x;
    const digit_type* b = y + i;

    for (size_type j = 0; j <= i; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  // phase 2
  for (size_type i = 0; i < xlen - ylen; ++i)
  {
    const digit_type* a = x + 1 + i;
    const digit_type* b = y + ylen - 1;

    for (size_type j = 0; j < ylen; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  // phase 3
  for (size_type i = 0; i < ylen - 1; ++i)
  {
    const digit_type* a = x + xlen - (ylen - 1) + i;
    const digit_type* b = y + ylen - 1;

    for (size_type j = i + 1; j < ylen; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  *z = acc_lo;
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::comba_mul(digit_type* z,
                                    const digit_type* x,
                                    const digit_type* y, size_type xylen)
{
  digit_type acc_hi = 0;  // accumulator for each column
  digit_type acc_lo = 0;
  digit_type carry_hi = 0;
  digit_type carry_lo = 0;

  // phase 1
  for (size_type i = 0; i < xylen; ++i)
  {
    const digit_type* a = x;
    const digit_type* b = y + i;

    for (size_type j = 0; j <= i; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  // phase 2
  for (size_type i = 1; i < xylen; ++i)
  {
    const digit_type* a = y + xylen - 1;
    const digit_type* b = x + i;

    for (size_type j = 0; j < xylen - i; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a--, *b++);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  *z = acc_lo;
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::comba_mul(digit_type* z,
                                    const digit_type* x, size_type xlen,
                                    const digit_type* y, size_type ylen,
                                    digit_type* workspace)
{
  assert(xlen >= ylen);

  digit_type* w = workspace;
  digit_type acc_hi = 0;  // accumulator for each column
  digit_type acc_lo = 0;
  digit_type carry_hi = 0;
  digit_type carry_lo = 0;

  // phase 1
  for (size_type i = 0; i < ylen; ++i)
  {
    const digit_type* a = x;
    const digit_type* b = y + i;

    for (size_type j = 0; j <= i; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *w++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  w -= ylen;

  // phase 2
  for (size_type i = 0; i < xlen - ylen; ++i)
  {
    const digit_type* a = x + 1 + i;
    const digit_type* b = y + ylen - 1;

    for (size_type j = 0; j < ylen; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++ = *w;
    *w++ = acc_lo;

    if (static_cast<size_type>(w - workspace) == ylen)
      w -= ylen;

    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  // phase 3
  for (size_type i = 0; i < ylen - 1; ++i)
  {
    const digit_type* a = x + xlen - (ylen - 1) + i;
    const digit_type* b = y + ylen - 1;

    for (size_type j = i + 1; j < ylen; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++ = *w;
    *w++ = acc_lo;

    if (static_cast<size_type>(w - workspace) == ylen)
      w -= ylen;

    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  const digit_type* ws_index = w;

  while (w < (workspace + ylen))
    *z++ = *w++;

  w = workspace;

  while (w < ws_index)
    *z++ = *w++;

  *z = acc_lo;
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::comba_mul_lo(digit_type* z,
                                       const digit_type* x, size_type xlen,
                                       const digit_type* y, size_type ylen,
                                       size_type num)
{
  assert(xlen >= ylen);
  assert(num <= (xlen + ylen));

  digit_type acc_hi = 0;  // accumulator for each column
  digit_type acc_lo = 0;
  digit_type carry_hi = 0;
  digit_type carry_lo = 0;

  // phase 1
  if (num)
  {
    const size_type m = ylen < num ? ylen : num;
    for (size_type i = 0; i < m; ++i)
    {
      const digit_type* a = x;
      const digit_type* b = y + i;

      for (size_type j = 0; j <= i; ++j)
      {
        const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
        carry_hi += add(carry_lo, carry);
        carry_hi += add(carry_lo, acc_hi);
        acc_hi = 0;
      }

      *z++     = acc_lo;
      acc_lo   = carry_lo;
      carry_lo = carry_hi;
      carry_hi = 0;
    }
  }

  // phase 2
  if (num >= ylen)
  {
    const size_type m = xlen - ylen < num ? xlen - ylen : num;
    for (size_type i = 0; i < m; ++i)
    {
      const digit_type* a = x + 1 + i;
      const digit_type* b = y + ylen - 1;

      for (size_type j = 0; j < ylen; ++j)
      {
        const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
        carry_hi += add(carry_lo, carry);
        carry_hi += add(carry_lo, acc_hi);
        acc_hi = 0;
      }

      *z++     = acc_lo;
      acc_lo   = carry_lo;
      carry_lo = carry_hi;
      carry_hi = 0;
    }
  }

  // phase 3
  if (num >= xlen + ylen)
  {
    const size_type m = ylen - 1 < num ? ylen - 1 : num;
    for (size_type i = 0; i < m; ++i)
    {
      const digit_type* a = x + xlen - (ylen - 1);
      const digit_type* b = y + ylen - 1;

      for (size_type j = i + 1; j < ylen; ++j)
      {
        const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
        carry_hi += add(carry_lo, carry);
        carry_hi += add(carry_lo, acc_hi);
        acc_hi = 0;
      }

      *z++     = acc_lo;
      acc_lo   = carry_lo;
      carry_lo = carry_hi;
      carry_hi = 0;
    }
  }

  *z = acc_lo;
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::comba_mul_hi(digit_type* z,
                                       const digit_type* x, size_type xlen,
                                       const digit_type* y, size_type ylen,
                                       size_type num)
{
  //assert(xlen >= ylen);
  assert(num > 0);
  assert(num < (xlen + ylen));

  digit_type acc_hi = 0;  // accumulator for each column
  digit_type acc_lo = 0;
  digit_type carry_hi = 0;
  digit_type carry_lo = 0;

  // phase 1
  if (num < ylen)
  {
    for (size_type i = num; i < ylen; ++i)
    {
      const digit_type* a = x;
      const digit_type* b = y + i;

      for (size_type j = 0; j <= i; ++j)
      {
        const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
        carry_hi += add(carry_lo, carry);
        carry_hi += add(carry_lo, acc_hi);
        acc_hi = 0;
      }

      *z++     = acc_lo;
      acc_lo   = carry_lo;
      carry_lo = carry_hi;
      carry_hi = 0;
    }
  }

  // phase 2
  if (num < xlen)
  {
    for (size_type i = num - ylen; i < xlen - ylen; ++i)
    {
      const digit_type* a = x + 1 + i;
      const digit_type* b = y + ylen - 1;

      for (size_type j = 0; j < ylen; ++j)
      {
        const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
        carry_hi += add(carry_lo, carry);
        carry_hi += add(carry_lo, acc_hi);
        acc_hi = 0;
      }

      *z++     = acc_lo;
      acc_lo   = carry_lo;
      carry_lo = carry_hi;
      carry_hi = 0;
    }
  }

  // phase 3
  if (num < (xlen + ylen))
  {
    for (size_type i = num - (xlen + ylen); i < ylen - 1; ++i)
    {
      const digit_type* a = x + xlen - (ylen - 1);
      const digit_type* b = y + ylen - 1;

      for (size_type j = i + 1; j < ylen; ++j)
      {
        const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
        carry_hi += add(carry_lo, carry);
        carry_hi += add(carry_lo, acc_hi);
        acc_hi = 0;
      }

      *z++     = acc_lo;
      acc_lo   = carry_lo;
      carry_lo = carry_hi;
      carry_hi = 0;
    }
  }

  *z = acc_lo;
}

template<typename D, typename S>
void
basic_primitive_ops<D,S>::comba_sqr(digit_type* z,
                                    const digit_type* x,
                                    size_type xlen)
{
  digit_type acc_hi = 0;  // accumulator for each column
  digit_type acc_lo = 0;
  digit_type carry_hi = 0;
  digit_type carry_lo = 0;

  // phase 1
  for (size_type i = 0; i < xlen; ++i)
  {
    const digit_type* a = x;
    const digit_type* b = x + i;

    for (size_type j = 0; j <= i; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  // phase 2
  for (size_type i = 1; i < xlen; ++i)
  {
    const digit_type* a = x + i;
    const digit_type* b = x + xlen - 1;

    for (size_type j = 0; j < xlen - i; ++j)
    {
      const digit_type carry = multiply_add(acc_hi, acc_lo, *a++, *b--);
      carry_hi += add(carry_lo, carry);
      carry_hi += add(carry_lo, acc_hi);
      acc_hi = 0;
    }

    *z++     = acc_lo;
    acc_lo   = carry_lo;
    carry_lo = carry_hi;
    carry_hi = 0;
  }

  *z = acc_lo;
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::multiply_add(digit_type& z_hi, digit_type& z_lo,
                                         digit_type x, digit_type y)
{
  static const digit_type lo_mask = (digit_type(1) << digit_bits / 2) - 1;

  const digit_type x_lo = x & lo_mask;
  const digit_type x_hi = x >> digit_bits/2;
  const digit_type y_lo = y & lo_mask;
  const digit_type y_hi = y >> digit_bits/2;

  const digit_type z0 = x_lo * y_lo;
  const digit_type z1 = x_lo * y_hi;
  const digit_type z2 = x_hi * y_lo;
  const digit_type z3 = x_hi * y_hi;

  digit_type z12 = z1 + (z0 >> digit_bits/2);
  const digit_type carry = add(z12, z2);

  const digit_type w_lo = (z12 << digit_bits/2) + (z0 & lo_mask);
  const digit_type w_hi = z3 + (carry << digit_bits/2) + (z12 >> digit_bits/2);

  digit_type u = add(z_lo, w_lo);
  digit_type v = add(z_hi, u);

  return v + add(z_hi, w_hi);
}

template<typename D, typename S>
inline
D basic_primitive_ops<D,S>::multiply_add(digit_type& z,
                                         digit_type x, digit_type y)
{
  static const digit_type lo_mask = (digit_type(1) << digit_bits / 2) - 1;

  const digit_type x_lo = x & lo_mask;
  const digit_type x_hi = x >> digit_bits/2;
  const digit_type y_lo = y & lo_mask;
  const digit_type y_hi = y >> digit_bits/2;

  const digit_type z0 = x_lo * y_lo;
  const digit_type z1 = x_lo * y_hi;
  const digit_type z2 = x_hi * y_lo;
  const digit_type z3 = x_hi * y_hi;

  digit_type z12 = z1 + (z0 >> digit_bits/2);
  const digit_type carry = add(z12, z2);

  const digit_type w_lo = (z12 << digit_bits/2) + (z0 & lo_mask);
  const digit_type w_hi = z3 + (carry << digit_bits/2) + (z12 >> digit_bits/2);

  const digit_type u = add(z, w_lo);

  return u + w_hi;
}

template<typename D, typename S>
D basic_primitive_ops<D,S>::multiply_add_digits(digit_type* z,
                                                const digit_type* x,
                                                digit_type y,
                                                size_type n)
{
  digit_type carry = 0;
  while (n--)
  {
    carry = add(*z, carry);
    carry += multiply_add(*z, *x, y);
    ++z; ++x;
  }

  return carry;
}

template<typename D, typename S>
void basic_primitive_ops<D,S>::div2by1(digit_type& q,
                                       digit_type x_hi, digit_type x_lo,
                                       digit_type y)
{
  static const digit_type lo_mask = (digit_type(1) << digit_bits / 2) - 1;
  static const digit_type hi_mask = lo_mask << (digit_bits / 2);
  static const digit_type hi_bit = digit_type(1) << (digit_bits - 1);
  static const digit_type d2 = digit_bits / 2;

  if (x_hi || x_lo >= y) // if x >= y
  {
    if (y & hi_bit) // if y is normalized
    {
      digit_type q_hi, q_lo;
      const digit_type rem = divide_half_digits(
          q_hi,
          x_hi, (x_lo & hi_mask) >> d2,
          (y & hi_mask) >> d2, y & lo_mask);

      divide_half_digits(
          q_lo,
          rem, x_lo & lo_mask,
          (y & hi_mask) >> d2, y & lo_mask);

      if (x_hi >= y)
        ++q_lo;

      q = (q_hi << d2) | q_lo;
    }
    else
    {
      // count leading zeros
      unsigned norm = 0;
      while (!(y & hi_bit))
      {
        ++norm;
        y <<= 1;
      }

      // normalize the number
      const digit_type shift = digit_bits - norm;

      digit_type w_hi, w_lo;

      w_hi = x_hi >> shift;
      if (w_hi)
        w_lo = (x_hi << norm) | (x_lo >> shift);
      else
      {
        w_hi = (x_hi << norm) | (x_lo >> shift);
        w_lo = x_lo << norm;
      }

      // Do a 2 digit by 1 digit division -> [w_hi, w_lo] / y
      digit_type q_hi, q_lo;
      const digit_type rem = divide_half_digits(
          q_hi,
          w_hi, (w_lo & hi_mask) >> d2,
          (y & hi_mask) >> d2, y & lo_mask);

      divide_half_digits(
          q_lo,
          rem, w_lo & lo_mask,
          (y & hi_mask) >> d2, y & lo_mask);

      if (w_hi >= y)
        ++q_lo;

      q = (q_hi << d2) | q_lo;
    }
  }
  else
    q = 0;
}

template<typename D, typename S>
inline
void basic_primitive_ops<D,S>::divide_by_two(digit_type* z,
                                             const digit_type* x, size_type n)
{
  z += n - 1;
  x += n - 1;

  digit_type carry = 0;

  while (n--)
  {
    // get carry for next iteration
    const digit_type r = *x & 1;

    *z-- = (*x-- >> 1) | (carry << (digit_bits - 1));

    carry = r;
  }
}

template<typename D, typename S>
D basic_primitive_ops<D,S>::divide_by_digit(digit_type* z,
                                            const digit_type* x, size_type n,
                                            digit_type y)
{
  /*if (n == 1)
  {
    *z = *x / y;
    return *x % y;
  }*/
  static const digit_type lo_mask = (digit_type(1) << digit_bits / 2) - 1;
  static const digit_type hi_mask = lo_mask << (digit_bits / 2);
  static const digit_type hi_bit = digit_type(1) << (digit_bits - 1);
  static const digit_type d2 = digit_bits / 2;

  z += n - 1;
  x += n - 1;

  digit_type w_hi, w_lo = 0;

  if (y & hi_bit) // if y is normalized
  {
    while (n--)
    {
      w_hi = w_lo;
      w_lo = *x--;
      // Do a 2 digit by 1 digit division -> [w_hi, w_lo] / y
      if (w_hi || w_lo >= y) // if w >= y
      {
        digit_type q_hi, q_lo;
        const digit_type rem = divide_half_digits(
            q_hi,
            w_hi, (w_lo & hi_mask) >> d2,
            (y & hi_mask) >> d2, y & lo_mask);

        w_lo = divide_half_digits(
            q_lo,
            rem, w_lo & lo_mask,
            (y & hi_mask) >> d2, y & lo_mask);

        if (w_hi >= y)
          ++q_lo;

        *z-- = (q_hi << d2) | q_lo;
      }
      else
        *z-- = 0;

      // q is never larger than one digit because w_hi is always set to the
      // remainder and thus w_hi can never be greater than y which is the
      // precondition for producing a quotient carry.
      // TODO we could do a 2 by 1 division before the loop, that could produce
      // a q carry. And then continue on as usual.
    }

    return w_lo;
  }
  else
  {
    // count leading zeros
    unsigned norm = 0;
    while (!(y & hi_bit))
    {
      ++norm;
      y <<= 1;
    }

    // we're going to normalize the number in place
    const digit_type shift = digit_bits - norm;

    w_hi = *x >> shift;

    while (n--)
    {
      w_lo = (*x << norm);
      if (n)
      {
        --x;
        w_lo |= *x >> shift;
      }

      // Do a 2 digit by 1 digit division -> [w_hi, w_lo] / y
      if (w_hi || w_lo >= y) // if w >= y
      {
        digit_type q_hi, q_lo;
        const digit_type rem = divide_half_digits(
            q_hi,
            w_hi, (w_lo & hi_mask) >> d2,
            (y & hi_mask) >> d2, y & lo_mask);

        w_lo = divide_half_digits(
            q_lo,
            rem, w_lo & lo_mask,
            (y & hi_mask) >> d2, y & lo_mask);

        *z-- = (q_hi << d2) | q_lo;
      }
      else
        *z-- = 0;

      w_hi = w_lo;
    }

    return w_lo >> norm;
  }
}

template<typename D, typename S>
D basic_primitive_ops<D,S>::divide_half_digits(digit_type& q,
                                               digit_type x12, digit_type x3,
                                               digit_type y1, digit_type y2)
{
  const digit_type y = (y1 << digit_bits / 2) | y2;
  //if (x12 >= y)
  //  x12 -= y;

  q = x12 / y1;

  const digit_type c = x12 - q * y1;
  const digit_type D_ = q * y2;

  digit_type R = (c << digit_bits / 2) | x3;

  if (R < D_) // R is too large by at most 2
  {
    static const digit_type z = ~digit_type(0);
    --q;
    R += y;
    if (R < y) // overflow
      return z - D_ + R + 1;
    if (R < D_)
    {
      --q;
      R += y;
      if (R < y) // overflow
        return z - D_ + R + 1;
    }
  }

  return R - D_;
}

template<typename D, typename S>
void basic_primitive_ops<D,S>::divide_by_3(digit_type* z, size_type len)
{
  divide_by_digit(z, z, len, 3);
}

template<typename D, typename S>
D basic_primitive_ops<D,S>::shift_bits_left(digit_type* x,
                                            size_type xlen,
                                            size_type n)
{
  assert(n > 0 && n < digit_bits);

  // shift for msbs
  const digit_type shift = digit_bits - n;

  digit_type carry = 0;
  while (xlen--)
  {
    const digit_type c = (*x >> shift);
    *x = (*x << n) | carry;
    carry = c;
    ++x;
  }

  return carry;
}

template<typename D, typename S>
void basic_primitive_ops<D,S>::shift_bits_right(digit_type* x,
                                                size_type xlen,
                                                size_type n)
{
  assert(n > 0 && n < digit_bits);

  const digit_type mask = (digit_type(1) << n) - 1;

  // shift for lsb
  const digit_type shift = digit_bits - n;

  x += xlen;

  digit_type carry = 0;
  while (xlen--)
  {
    --x;
    const digit_type c = *x & mask;
    *x = (*x >> n) | (carry << shift);
    carry = c;
  }
}

template<typename D, typename S>
int basic_primitive_ops<D,S>::compare_magnitude(const digit_type* x,
                                                size_type xlen,
                                                const digit_type* y,
                                                size_type ylen)
{
  if (xlen > ylen)
    return 1;

  if (xlen < ylen)
    return -1;

  // compare all digits
  x += xlen;
  y += ylen;
  while (xlen--)
  {
    --x; --y;
    if (*x > *y)
      return 1;
    if (*x < *y)
      return -1;
  }

  return 0;
}

template<typename D, typename S>
struct primitive_ops : basic_primitive_ops<D,S>
{};




/*
// Here we include primitive_ops specializations that use assembler

#if defined(BOOST_MP_MATH_INTEGER_USE_ASM)

  #if defined(__GNU__)
    #if defined(__386__)
      #include <boost/mp_math/integer/detail/base/asm/x86/gnu_386_primitive_ops.hpp>
    #endif
  #endif

#endif
*/


} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

