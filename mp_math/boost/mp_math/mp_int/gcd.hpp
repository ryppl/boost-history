// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/* Greatest Common Divisor using the binary method */
template<class A, class T>
mp_int<A,T> gcd(const mp_int<A,T>& a, const mp_int<A,T>& b)
{
  typedef typename mp_int<A,T>::size_type size_type;

  /* either zero then gcd is the largest */
  if (a.is_zero())
    return abs(b);
  if (b.is_zero())
    return abs(a);

  /* get copies of a and b we can modify */
  mp_int<A,T> u = abs(a);
  mp_int<A,T> v = abs(b);

  /* B1.  Find the common power of two for u and v */
  const size_type u_lsb = u.count_lsb();
  const size_type v_lsb = v.count_lsb();
  const size_type     k = std::min(u_lsb, v_lsb);

  if (k > 0)
  {
    /* divide the power of two out */
    u.shift_right(k,0);
    v.shift_right(k,0);
  }

  /* divide any remaining factors of two out */
  if (u_lsb != k)
    u.shift_right(u_lsb - k, 0);

  if (v_lsb != k)
    v.shift_right(v_lsb - k, 0);

  while (!v.is_zero())
  {
    /* make sure v is the largest */
    if (u.compare_magnitude(v) == 1)
      /* swap u and v to make sure v is >= u */
      u.swap(v);
     
    /* subtract smallest from largest */
    v.sub_smaller_magnitude(u);

    /* Divide out all factors of two */
    v.shift_right(v.count_lsb(), 0);
  } 

  /* multiply by 2**k which we divided out at the beginning */
  u <<= k;

  return u;
}

