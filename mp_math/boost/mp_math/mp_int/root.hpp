// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_MP_INT_ROOT_HPP
#define BOOST_MP_MATH_MP_INT_ROOT_HPP

#include <boost/mp_math/mp_int/mp_int_fwd.hpp>


namespace boost {
namespace mp_math {

template<class A, class T>
mp_int<A,T> sqrt(const mp_int<A,T>& x)
{
  if (x.is_negative())
    throw std::domain_error("sqrt: argument must be positive");

  mp_int<A,T> t1;

  if (x.is_zero())
  {
    t1.zero();
    return t1;
  }

  t1 = x;

  // First approx. (not very bad for large arg)
  t1.shift_digits_right(t1.size()/2);

  // t1 > 0
  mp_int<A,T> t2 = x / t1;

  t1 += t2;
  t1.divide_by_2();
  // And now t1 > sqrt(arg)
  do
  {
    t2 = x / t1;
    t1 += t2;
    t1.divide_by_2();
    // t1 >= sqrt(arg) >= t2 at this point
  } while (t1.compare_magnitude(t2) == 1);

  return t1;
}


// find the n'th root of an integer 
//
// Result found such that (c)**b <= a and (c+1)**b > a 
//
// This algorithm uses Newton's approximation 
// x[i+1] = x[i] - f(x[i])/f'(x[i]) 
// which will find the root in log(N) time where 
// each step involves a fair bit.  This is not meant to 
// find huge roots [square and cube, etc].
template<class A, class T>
mp_int<A,T> nth_root(const mp_int<A,T>& x, typename mp_int<A,T>::digit_type n)
{
  if ((n & 1) == 0 && x.is_negative())
    throw std::domain_error("nth_root: argument must be positive if n is even");

  // if x is negative fudge the sign but keep track
  const int neg = x.sign();
  const_cast<mp_int<A,T>*>(&x)->set_sign(1);

  mp_int<A,T> t1, t2, t3;
  
  t2 = typename mp_int<A,T>::digit_type(2);

  do
  {
    t1 = t2;

    // t2 = t1 - ((t1**n - x) / (n * t1**(n-1)))
    
    // t3 = t1**(n-1)
    t3 = pow(t1, n-1);

    // numerator
    // t2 = t1**n
    t2 = t3 * t1;

    // t2 = t1**n - x
    t2 -= x;

    // denominator
    // t3 = t1**(n-1) * n
    t3.multiply_by_digit(n);

    // t3 = (t1**n - x)/(n * t1**(n-1))
    t3 = t2 / t3;

    t2 = t1 - t3;
  } while (t1 != t2);
  
  // result can be off by a few so check
  for (;;)
  {
    t2 = pow(t1, n);

    if (t2 > x)
      --t1;
    else
      break;
  }

  // reset the sign of x first
  const_cast<mp_int<A,T>*>(&x)->set_sign(neg);

  // set the sign of the result
  t1.set_sign(neg);

  return t1;
}

template<class A, class T>
mp_int<A,T> nth_root(const mp_int<A,T>& x, const mp_int<A,T>& n)
{
  if (n.is_odd() && x.is_negative())
    throw std::domain_error("nth_root: argument must be positive if n is even");

  if (n.size() == 1)
    return nth_root(x, n[0]);

  // if x is negative fudge the sign but keep track
  const int neg = x.sign();
  const_cast<mp_int<A,T>*>(&x)->set_sign(1);

  mp_int<A,T> t1, t2, t3;
  
  t2 = typename mp_int<A,T>::digit_type(2);

  do
  {
    t1 = t2;

    // t2 = t1 - ((t1**n - x) / (n * t1**(n-1)))
    
    // t3 = t1**(n-1)
    t3 = pow(t1, n-1);

    // numerator
    // t2 = t1**n
    t2 = t3 * t1;

    // t2 = t1**n - x
    t2 -= x;

    // denominator
    // t3 = t1**(n-1) * n
    t3 *= n;

    // t3 = (t1**n - x)/(n * t1**(n-1))
    t3 = t2 / t3;

    t2 = t1 - t3;
  } while (t1 != t2);
  
  // result can be off by a few so check
  for (;;)
  {
    t2 = pow(t1, n);

    if (t2 > x)
      --t1;
    else
      break;
  }

  // reset the sign of x first
  const_cast<mp_int<A,T>*>(&x)->set_sign(neg);

  // set the sign of the result
  t1.set_sign(neg);

  return t1;
}


} // namespace mp_math
} // namespace boost

#endif

