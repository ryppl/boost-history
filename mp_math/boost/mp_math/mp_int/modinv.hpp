// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


// hac 14.61, pp608
// *this = *this**-1 (mod b)
template<class A, class T>
void mp_int<A,T>::modinv(const mp_int& b)
{
  if (b.is_negative() || !b)
    throw std::domain_error("modinv: modulus is negative or zero");

  // if the modulus is odd we can use a faster routine
  if (b.is_odd())
    odd_modinv(b);
  else
    even_modinv(b);
}

/* hac 14.61, pp608 */
template<class A1, class T>
void mp_int<A1,T>::even_modinv(const mp_int& y)
{
  assert(y.is_positive() && y);

  static const char* const err_msg = "mp_int::modinv: inverse does not exist";

  const mp_int x = *this % y;
  
  if (x.is_even())
    throw std::domain_error(err_msg);

  mp_int u(x);
  mp_int v(y);
  mp_int A = digit_type(1);
  mp_int B = digit_type(0);
  mp_int C = digit_type(0);
  mp_int D = digit_type(1);

top:
  while (u.is_even())
  {
    u.divide_by_2();
    
    if (A.is_odd() || B.is_odd())
    {
      /* A = (A+y)/2, B = (B-x)/2 */
      A += y;
      B -= x;
    }
    A.divide_by_2();
    B.divide_by_2();
  }

  while (v.is_even())
  {
    v.divide_by_2();

    if (C.is_odd() || D.is_odd())
    {
      /* C = (C+y)/2, D = (D-x)/2 */
      C += y;
      D -= x;
    }
    C.divide_by_2();
    D.divide_by_2();
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

  /* now a = C, b = D, gcd == g*v */

  /* if v != 1 then there is no inverse */
  if (v != digit_type(1))
    throw std::domain_error(err_msg);

  // if it's too low
  while (C.compare_to_digit(0) == -1)
    C += y;
  
  // too big
  while (C.compare_magnitude(y) != -1)
    C -= y;
  
  swap(C);
}

/* computes the modular inverse via binary extended euclidean algorithm, 
 * that is *this = 1 / *this mod x
 *
 * Based on even modinv except this is optimized for the case where x is 
 * odd as per HAC Note 14.64 on pp. 610
 */
template<class A1, class T>
void mp_int<A1,T>::odd_modinv(const mp_int& x)
{
  assert(x.is_odd());

  // x == modulus, y == value to invert
  // we need y = |a|
  const mp_int y = *this % x;

  // 3. u=x, v=y, A=1, B=0, C=0, D=1
  mp_int u(x);
  mp_int v(y);
  mp_int A = digit_type(1);
  mp_int B = digit_type(0);
  mp_int C = digit_type(0);
  mp_int D = digit_type(1);

top:
  while (u.is_even())
  {
    u.divide_by_2();
    
    if (B.is_odd())
      B -= x;
    
    B.divide_by_2();
  }

  while (v.is_even())
  {
    v.divide_by_2();

    if (D.is_odd())
      D -= x;      

    D.divide_by_2();
  }

  if (u >= v)
  {
    /* u = u - v, B = B - D */
    u -= v;
    B -=D;
  }
  else
  {
    v -= u;
    D -= B;
  }

  if (u)
    goto top;

  /* now a = C, x = D, gcd == g*v */

  if (v != digit_type(1))
    throw std::domain_error("mp_int::modinv: inverse does not exist");

  while (D.is_negative())
    D += x;

  swap(D);
}


// returns the modular multiplicative inverse x of a (mod m) such that
//  x*a = 1 (mod m)  =>
// a^-1 = x (mod m)
// The inverse exists only if a and m are coprime (i.e. gcd(a,m) = 1).
// If no inverse exists this function will throw std::domain_error.
template<class A, class T>
mp_int<A,T> modinv(const mp_int<A,T>& a, const mp_int<A,T>& m)
{
  mp_int<A,T> x(a);
  x.modinv(m);
  return x;
}

