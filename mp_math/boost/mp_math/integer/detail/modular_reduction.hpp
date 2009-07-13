// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_MODULAR_REDUCTION_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_MODULAR_REDUCTION_HPP

#include <boost/mp_math/integer/detail/adder.hpp>
#include <boost/mp_math/integer/detail/multiplier.hpp>
#include <boost/mp_math/integer/detail/power.hpp>
#include <boost/mp_math/integer/detail/shifter.hpp>

namespace boost {
namespace mp_math {
namespace detail {

template<class ApInt>
int compare_magnitude(const ApInt& lhs, const ApInt& rhs)
{
  return ApInt::traits_type::ops_type::compare_magnitude(
      lhs.digits(), lhs.size(), rhs.digits(), rhs.size());
}


template<class ApInt>
struct modular_reducer
{
  typedef typename ApInt::digit_type     digit_type;
  typedef typename ApInt::size_type      size_type;
  typedef typename ApInt::traits_type    traits_type;
  typedef typename traits_type::ops_type ops_type;

  static void barrett_reduce        (ApInt& z, const ApInt& m, const ApInt& mu);
  static void montgomery_reduce     (ApInt& z, const ApInt& m, digit_type rho);
  static void montgomery_normalize  (ApInt& z, const ApInt& n);
  static void restricted_dr_reduce  (ApInt& z, const ApInt& n, digit_type k);
  static void unrestricted_dr_reduce(ApInt& z, const ApInt& n, digit_type d);
  static void unrestricted_dr_slow_reduce(ApInt& z, const ApInt& n, const ApInt& d);
};


// reduces x mod m, assumes 0 < x < m^2, mu is precomputed.
// From HAC pp.604 Algorithm 14.42
template<class ApInt>
void modular_reducer<ApInt>::
barrett_reduce(ApInt& x, const ApInt& m, const ApInt& mu)
{
  size_type k = m.size();

  ApInt q(x);

  // q = x / radix^(k-1)
  shifter<ApInt>::shift_digits_right(q, k-1);

  // according to HAC this optimization is ok
  if (k > digit_type(1) << (traits_type::radix_bits - 1))
    q *= mu;
  else
  {
    //q.fast_mul_high_digits(mu, k);
    ApInt tmp; tmp.reserve(k);
    ops_type::comba_mul_hi(tmp.digits(), q.digits(), q.size(),
                                         m.digits(), m.size(), k);
    swap(tmp, q);
  }

  // q = q / radix^(k+1)
  shifter<ApInt>::shift_digits_right(q, k+1);

  // r = x mod radix^(k+1)
  base::divider<ApInt>::modulo_pow2(x, traits_type::radix_bits * (k + 1));

  // q = q * m mod radix^(k+1)
  //q.mul_digits(m, k + 1);

  ApInt tmp; tmp.reserve(k + 1);
  ops_type::comba_mul_lo(tmp.digits(), m.digits(), m.size(),
                                       q.digits(), q.size(), k + 1);
  swap(tmp, q);

  x -= q;

  // if x < 0, add radix^(k+1) to it
  if (x.is_negative())
  {
    q = digit_type(1);
    shifter<ApInt>::shift_digits_left(q, k+1);
    x += q;
  }

  while (x >= m)
    base::adder<ApInt>::subtract_smaller_magnitude(x, m);
}

// computes xR^-1 == x (mod m) via Montgomery Reduction
template<class ApInt>
void modular_reducer<ApInt>::
montgomery_reduce(ApInt& x, const ApInt& m, digit_type rho)
{
  const size_type num = m.size() * 2 + 1;

  x.reserve(num);
  std::memset(x.digits() + x.size(), 0,
              (x.capacity() - x.size()) * sizeof(digit_type));
  x.set_size(num);

  for (size_type i = 0; i < m.size(); ++i)
  {
    // mu = x[i] * rho (mod base)
    // The value of rho must be precalculated such that it equals -1/n0 mod b
    // this allows multiply_add_digits to reduce the input one digit at a time.
    const digit_type mu = x[i] * rho;

    // x = x + mu * m * base^i

    digit_type carry =
      ops_type::multiply_add_digits(x.digits() + i,
                                    m.digits(),
                                    mu,
                                    m.size());

    // At this point the i'th digit of x should be zero

    ops_type::ripple_carry(x.digits() + i + m.size(),
                           x.digits() + i + m.size(), num, carry);
  }

  x.clamp();

  if (!x)
    x.set_sign_bit(0);

  // at this point the m.size least significant digits of x are all zero which
  // means we can shift x to the right by m.size digits and the residue is
  // unchanged.

  // x = x/radix^m.size()
  shifter<ApInt>::shift_digits_right(x, m.size());

  if (compare_magnitude(x, m) != -1)
    base::adder<ApInt>::subtract_smaller_magnitude(x, m);
}

// shifts with subtractions when the result is greater than n.
// The method is slightly modified to shift B unconditionally upto just under
// the leading bit of n. This saves alot of multiple precision shifting.
template<class ApInt>
void modular_reducer<ApInt>::montgomery_normalize(ApInt& x, const ApInt& n)
{
  // how many bits of last digit does n use
  size_type bits = n.precision() % traits_type::radix_bits;

  if (n.size() > 1)
    power<ApInt>::pow2(x, (n.size() - 1) * traits_type::radix_bits + bits - 1);
  else
  {
    x = digit_type(1);
    bits = 1;
  }

  // now compute C = A * B mod n
  for (size_type i = bits; i <= traits_type::radix_bits; ++i)
  {
    multiplier<ApInt>::multiply_by_2(x);
    if (compare_magnitude(x, n) != -1)
      base::adder<ApInt>::subtract_smaller_magnitude(x, n);
  }
}

// reduce "x" modulo "n" using the Diminished Radix algorithm.
// Based on algorithm from the paper
//
// "Generating Efficient Primes for Discrete Log Cryptosystems"
//                 Chae Hoon Lim, Pil Joong Lee,
//          POSTECH Information Research Laboratories
//
// The modulus must be of a special format [see manual]
//
// Has been modified to use algorithm 7.10 from the LTM book instead
//
// Input x must be in the range 0 <= x <= (n-1)**2
template<class ApInt>
void modular_reducer<ApInt>::
restricted_dr_reduce(ApInt& x, const ApInt& n, digit_type k)
{
  typedef typename ApInt::digit_type digit_type;
  typedef typename ApInt::size_type  size_type;
  typedef typename ApInt::iterator   iterator;

  const size_type m = n.size();

  x.reserve(m + m);
  std::memset(x.digits() + x.size(), 0, (m + m - x.size()) * sizeof(digit_type));

top:

  // compute (r mod B**m) + k * [r/B**m]
  x[m] = ops_type::multiply_add_digits(x.digits(), x.digits() + m, k, m);

  // zero words above m
  if (x.size() > m + 1) // guard against overflow
    std::memset(x.digits() + m + 1, 0, (x.size() - (m + 1)) * sizeof(digit_type));

  x.clamp();

  if (!x)
    x.set_sign_bit(0);

  if (compare_magnitude(x, n) != -1)
  {
    base::adder<ApInt>::subtract_smaller_magnitude(x, n);
    goto top;
  }
}

// reduces x modulo n where n is of the form 2^p - d
template<class ApInt>
void modular_reducer<ApInt>::
unrestricted_dr_reduce(ApInt& x, const ApInt& n, digit_type d)
{
  const size_type p = n.precision();

top:

  ApInt q(x);

  // q = x/2^p, x = x % 2^p
  shifter<ApInt>::shift_bits_right(q, x, p);

  q *= d;

  adder<ApInt>::add_magnitude(x, q);

  if (compare_magnitude(x, n) != -1)
  {
    base::adder<ApInt>::subtract_smaller_magnitude(x, n);
    goto top;
  }
}

// reduces x modulo n where n is of the form 2^p - d. This differs from
// unrestricted_dr_reduce since "d" can be larger than a single digit.
template<class ApInt>
void modular_reducer<ApInt>::
unrestricted_dr_slow_reduce(ApInt& x, const ApInt& n, const ApInt& d)
{
  const size_type p = n.precision();

top:

  ApInt q(x);

  // q = x/2^p, x = x % 2^p
  shifter<ApInt>::shift_bits_right(q, x, p);

  q *= d;

  adder<ApInt>::add_magnitude(x, q);

  if (compare_magnitude(x, n) != -1)
  {
    base::adder<ApInt>::subtract_smaller_magnitude(x, n);
    goto top;
  }
}




} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

