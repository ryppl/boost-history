// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_MODINV_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_MODINV_HPP

#include <boost/mp_math/integer/detail/base/divider.hpp>

namespace boost {
namespace mp_math {
namespace detail {


template<class ApInt>
struct modular_inverter
{
  typedef typename ApInt::digit_type digit_type;
  typedef base::divider<ApInt>       base_divider_type;

  static void modinv     (ApInt& z, const ApInt& x, const ApInt& m);
  static void even_modinv(ApInt& z, const ApInt& x, const ApInt& m);
  static void odd_modinv (ApInt& z, const ApInt& x, const ApInt& m);
};


// returns the modular multiplicative inverse z of x (mod m) such that
// z * x = 1 (mod m)  =>
// x^-1  = z (mod m)
// The inverse exists only if a and m are coprime (i.e. gcd(a,m) = 1).
// If no inverse exists this function will throw std::domain_error.
template<class ApInt>
void
modular_inverter<ApInt>::modinv(ApInt& z, const ApInt& x, const ApInt& m)
{
  if (m.is_negative())
    throw std::domain_error("modinv: modulus is negative");
  if (!m)
    throw std::domain_error("modinv: modulus is zero");

  // if the modulus is odd we can use a faster routine
  if (m.is_odd())
    odd_modinv(z, x, m);
  else
    even_modinv(z, x, m);
}

// HAC 14.61, pp608
template<class ApInt>
void
modular_inverter<ApInt>::even_modinv(ApInt& z, const ApInt& x, const ApInt& m)
{
  assert(m.is_even());

  static const char* const err_msg = "modinv: inverse does not exist";

  const ApInt y = x % m;

  if (y.is_even())
    throw std::domain_error(err_msg);

  ApInt u(y);
  ApInt v(m);
  ApInt A = digit_type(1);
  ApInt B = digit_type(0);
  ApInt C = digit_type(0);
  ApInt D = digit_type(1);

top:
  while (u.is_even())
  {
    base_divider_type::divide_by_2(u);

    if (A.is_odd() || B.is_odd())
    {
      A += m;
      B -= y;
    }
    base_divider_type::divide_by_2(A);
    base_divider_type::divide_by_2(B);
  }

  while (v.is_even())
  {
    base_divider_type::divide_by_2(v);

    if (C.is_odd() || D.is_odd())
    {
      C += m;
      D -= y;
    }
    base_divider_type::divide_by_2(C);
    base_divider_type::divide_by_2(D);
  }

  if (u >= v)
  {
    u -= v;
    A -= C;
    B -= D;
  }
  else
  {
    v -= u;
    C -= A;
    D -= B;
  }

  if (u)
    goto top;

  // now a = C, b = D, gcd == g*v

  // if v != 1 then there is no inverse
  if (v != digit_type(1))
    throw std::domain_error(err_msg);

  // if it's too low
  while (C.is_negative())
    C += m;

  // too big
  while (compare_magnitude(C, m) != -1)
    C -= m;

  swap(z, C);
}

// computes the modular inverse via binary extended euclidean algorithm,
// that is z = 1 / z mod m
//
// Based on even modinv except this is optimized for the case where m is
// odd as per HAC Note 14.64 on pp. 610
template<class ApInt>
void
modular_inverter<ApInt>::odd_modinv(ApInt& z, const ApInt& x, const ApInt& m)
{
  assert(m.is_odd());

  // m == modulus, y == value to invert
  // we need y = |a|
  const ApInt y = x % m;

  ApInt u(m);
  ApInt v(y);
  ApInt A = digit_type(1);
  ApInt B = digit_type(0);
  ApInt C = digit_type(0);
  ApInt D = digit_type(1);

top:
  while (u.is_even())
  {
    base_divider_type::divide_by_2(u);

    if (B.is_odd())
      B -= m;

    base_divider_type::divide_by_2(B);
  }

  while (v.is_even())
  {
    base_divider_type::divide_by_2(v);

    if (D.is_odd())
      D -= m;

    base_divider_type::divide_by_2(D);
  }

  if (u >= v)
  {
    // u = u - v, B = B - D
    u -= v;
    B -= D;
  }
  else
  {
    v -= u;
    D -= B;
  }

  if (u)
    goto top;

  // now a = C, m = D, gcd == g*v

  if (v != digit_type(1))
    throw std::domain_error("modinv: inverse does not exist");

  while (D.is_negative())
    D += m;

  swap(z, D);
}


} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

