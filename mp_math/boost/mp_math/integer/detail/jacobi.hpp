// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_JACOBI_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_JACOBI_HPP


namespace boost {
namespace mp_math {
namespace detail {

// computes the jacobi c = (a | p) (or Legendre if p is prime)
// HAC pp. 73 Algorithm 2.149
template<class ApInt>
int jacobi(const ApInt& a, const ApInt& p)
{
  typedef typename ApInt::digit_type digit_type;
  typedef typename ApInt::size_type  size_type;

  if (p <= digit_type(0))
    throw std::domain_error("jacobi: p must be greater than 0");

  if (!a)
    return 0;

  if (a == digit_type(1))
    return 1;

  // default
  int s = 0;

  // write a = a1 * 2**k
  ApInt a1(a);

  // find largest power of two that divides a1
  const size_type k = a1.count_trailing_zero_bits();
  // now divide by it
  a1 >>= k;

  // if k is even set s=1
  if ((k & 1) == 0)
    s = 1;
  else
  {
    // calculate p[0] mod 8
    const digit_type residue = p[0] & 7;

    if (residue == 1 || residue == 7)
      s = 1;
    else if (residue == 3 || residue == 5)
      s = -1;
  }

  // if p == 3 (mod 4) *and* a1 == 3 (mod 4) then s = -s
  if (((p[0] & 3) == 3) && ((a1[0] & 3) == 3))
    s = -s;

  if (a1 == digit_type(1))
    return s;
  else
    return s * jacobi(p % a1, a1);
}


} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

