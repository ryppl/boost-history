// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_DIVIDER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_DIVIDER_HPP

#include <iostream>
#include <boost/mp_math/integer/detail/shifter.hpp>

namespace boost {
namespace mp_math {
namespace detail {

template<class ApInt>
struct divider
{
  typedef ApInt                          int_type;
  typedef typename int_type::traits_type traits_type;
  typedef typename traits_type::ops_type ops_type;
  typedef typename int_type::digit_type  digit_type;
  typedef typename int_type::size_type   size_type;
  typedef shifter<int_type>              shifter_type;

  // divide a by b, optionally store remainder
  static void classic_divide(const ApInt& a, const ApInt& b,
                             ApInt& q, ApInt* remainder = 0);
};


// integer signed division.
// q*b + r == a
// HAC pp.598 Algorithm 14.20
//
// Note that the description in HAC is horribly incomplete. For example, it
// doesn't consider the case where digits are removed from 'x' in the inner
// loop. It also doesn't consider the case that y has fewer than three digits,
// etc..
// The overall algorithm is as described as 14.20 from HAC but fixed to treat
// these cases.
template<class ApInt>
void divider<ApInt>::classic_divide(const ApInt& a, const ApInt& b,
                                    ApInt& q, ApInt* remainder)
{
  if (!b)
    throw std::domain_error("unbounded::divide: division by zero");

  if (b.size() == 1)
  {
    q.reserve(a.size());
    q.set_size(a.size());
    const digit_type r =
      ops_type::divide_by_digit(q.digits(), a.digits(), a.size(), b[0]);
    q.clamp();
    if (remainder)
      *remainder = r;
    return;
  }

  if (ops_type::compare_magnitude(a.digits(), a.size(),
                                  b.digits(), b.size()) == -1)
  {
    if (remainder)
      *remainder = a;
    q = digit_type(0);
    return;
  }

  q.reserve(a.size() + 2);
  q.set_size(a.size() + 2);
  std::memset(q.digits(), 0, q.size() * sizeof(digit_type));

  ApInt x;
  ApInt y;

  x.reserve(a.size() + 1);
  y.reserve(b.size() + 1);

  // TODO optimize by creating shift_bits_left(digit_type* z,
  // const digit_type* x, size_type xlen, size_type n) function and using it in
  // the if below
  std::memcpy(x.digits(), a.digits(), a.size() * sizeof(digit_type));
  std::memcpy(y.digits(), b.digits(), b.size() * sizeof(digit_type));

  x.set_size(a.size());
  y.set_size(b.size());

  // normalize both x and y, ensure that y >= radix/2
  size_type norm = y.precision() % traits_type::radix_bits;
  if (norm < traits_type::radix_bits - 1)
  {
    norm = traits_type::radix_bits - 1 - norm;
    {
      const digit_type carry =
        ops_type::shift_bits_left(x.digits(), x.size(), norm);
      if (carry)
        x.push(carry);
    }
    {
      const digit_type carry =
        ops_type::shift_bits_left(y.digits(), y.size(), norm);
      if (carry)
        y.push(carry);
    }
  }
  else
    norm = 0;

  // note hac does 0 based, so if size==5 then its 0,1,2,3,4, e.g. use 4
  const size_type n = x.size() - 1;
  const size_type t = y.size() - 1;

  // find leading digit of the quotient
  // while (x >= y*radix^(n-t)) do { q[n-t] += 1; x -= y*radix^(n-t) }

  // here we pretend that we shifted y by (n-t) digits to the left
  while (ops_type::compare_magnitude(
        x.digits() + (n-t), y.size(), y.digits(), y.size()) != -1)
  {
    ++q[n - t];
    ops_type::sub_smaller_magnitude(
        x.digits() + (n-t), x.digits() + (n-t), y.size(), y.digits(), y.size());
    x.clamp();
  }

  // moved temporary construction out of the loop below
  ApInt u;
  u.reserve(y.size() + (n - t));

  // find the remainder of the digits
  for (size_type i = n; i >= (t + 1); --i)
  {
    if (i > x.size())
      continue;

    const size_type offset = i - t - 1;

    // step 3.1 if xi == yt then set q{i-t-1} to radix-1,
    // otherwise set q{i-t-1} to (xi*radix + x{i-1})/yt
    if (x[i] == y[t])
      q[offset] = traits_type::max_digit_value;
    else
    {
      ops_type::div2by1(q[offset], x[i], x[i - 1], y[t]);
      /*typedef unsigned long long word_type;
      word_type tmp  = static_cast<word_type>(x[i])
                    << static_cast<word_type>(traits_type::radix_bits);
      tmp |= x[i - 1];
      tmp /= y[t];
      q[offset] = static_cast<digit_type>(tmp);*/
    }

    // now fixup quotient estimation
    // while (q{i-t-1} * (yt * radix + y{t-1})) >
    //       xi * radix^2 + xi-1 * radix + xi-2
    //
    // do q{i-t-1} -= 1;

    digit_type t1[3];
    digit_type t2[3];

    ++q[offset];
    do
    {
      --q[offset];

      // find left hand
      t1[0] = (t == 0) ? 0 : y[t - 1];
      t1[1] = y[t];
      t1[2] = ops_type::multiply_by_digit(t1, t1, 2, q[offset]);

      // find right hand
      t2[0] = (i < 2) ? 0 : x[i - 2];
      t2[1] = (i == 0) ? 0 : x[i - 1];
      t2[2] = x[i];
    } while (ops_type::compare_magnitude(t1, 3, t2, 3) == 1);

    // step 3.3 x = x - q{i-t-1} * y * radix^{i-t-1}
    std::memset(u.digits(), 0, offset * sizeof(digit_type));
    const digit_type carry =
      ops_type::multiply_by_digit(u.digits() + offset,
                                  y.digits(), y.size(), q[offset]);
    u.set_size(y.size() + offset);
    if (carry)
      u.push(carry);
    u.clamp();

    // if x < 0 then { x = x + y*radix^{i-t-1}; q{i-t-1} -= 1; }
    // We check if x will become less than zero before we subtract, in that case
    // we adjust x before the subtraction.
    if (ops_type::compare_magnitude(x.digits(), x.size(),
                                    u.digits(), u.size()) == -1)
    {
      digit_type* x_off = x.digits() + offset;

      digit_type carry =
        ops_type::add_digits(x_off, x_off, y.digits(), y.size());

      ops_type::ripple_carry(x_off + y.size(),
                             x_off + y.size(),
                             x.size() - y.size() - offset,
                             carry);

      if (carry)
      {
        // TODO can this happen, do I need to reserve one more digit?
        x.push(carry);
      }

      --q[offset];
    }

    ops_type::sub_smaller_magnitude(x.digits(), x.digits(), x.size(),
                                                u.digits(), u.size());
    x.clamp();
  }

  // now q is the quotient and x is the remainder [which we have to normalize]

  q.clamp();

  if (remainder)
  {
    // TODO optimize by having shift_right(z, x, xlen, n) function
    x >>= norm;
    remainder->swap(x);
  }
}


} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

