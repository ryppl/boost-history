// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// computes least common multiple as |a*b|/gcd(a,b)
template<class A, class T>
mp_int<A,T> lcm(const mp_int<A,T>& a, const mp_int<A,T>& b)
{
  mp_int<A,T> result;
    
  if (!a || !b)
  {
    result.zero();
    return result;
  }
  
  result = a / gcd(a, b) * b;

  result.set_sign(1);
  
  return result;
}

#ifdef BOOST_HAS_VARIADIC_TMPL
template<class A, class T, class... MpInts>
mp_int<A,T> lcm(const mp_int<A,T>& a, const mp_int<A,T>& b, const MpInts&... args)
{
  return lcm(lcm(a, b), args...);
}
#endif

