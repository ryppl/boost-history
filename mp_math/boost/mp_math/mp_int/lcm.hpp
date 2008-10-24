// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/* computes least common multiple as |a*b|/(a, b) */
template<class A, class T>
mp_int<A,T> lcm(const mp_int<A,T>& a, const mp_int<A,T>& b)
{
  /* t1 = get the GCD of the two inputs */
  const mp_int<A,T> t1 = gcd(a,b);

  /* divide the smallest by the GCD */
  const mp_int<A,T>* smallest = a.compare_magnitude(b) == -1 ? &a : &b;

  mp_int<A,T> t2 = *smallest / t1;
  
  t2.sign_ = 1;
  
  return t2;
}

