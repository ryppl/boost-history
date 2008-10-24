// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/* calc a value mod 2**b */
template<class A, class T>
void mp_int<A,T>::modulo_2_to_the_power_of(int b)
{
  /* if b is <= 0 then zero the int */
  if (b <= 0)
  {
    zero();
    return;
  }

  /* if the modulus is larger than the value then return */
  if (b >= static_cast<int>(used_ * valid_bits))
    return;

  /* zero digits above the last digit of the modulus */
  for (size_type x = (b / valid_bits) + ((b % valid_bits) == 0 ? 0 : 1); x < used_; ++x)
    digits_[x] = 0;

  /* clear the digit that is not completely outside/inside the modulus */
  digits_[b / valid_bits] &= static_cast<digit_type>(
      ((digit_type(1)) << ((static_cast<digit_type>(b)) % valid_bits)) - (digit_type(1)));

  clamp();
  if (is_zero())
    sign_ = 1;
}

// hac 14.61, pp608
// *this = *this**-1 (mod b)
template<class A, class T>
void mp_int<A,T>::modinv(const mp_int& b)
{
  /* b cannot be negative */
  if (b.is_negative() || b.is_zero())
    throw std::domain_error("modinv: b is negative or zero");

  /* if the modulus is odd we can use a faster routine instead */
  if (b.is_odd())
    fast_modinv(b);
  else
    slow_modinv(b);
}

/* hac 14.61, pp608 */
template<class Al, class T>
void mp_int<Al,T>::slow_modinv(const mp_int& b)
{
  /* b cannot be negative */
  if (b.is_negative() || b.is_zero())
    throw std::domain_error("mp_int::slow_modinv: b is negative or zero"); // XXX: this is already tested in modinv

  const mp_int x = *this % b;
  const mp_int y(b); // TODO no need to copy b here since b or y is never changed

  /* [modified] if x,y are both even then return an error! */
  if (x.is_even() && y.is_even())
    throw std::domain_error("mp_int::slow_modinv: no inverse exists");// TODO: different text?

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

  if (!u.is_zero())
    goto top;

  /* now a = C, b = D, gcd == g*v */

  /* if v != 1 then there is no inverse */
  if (v != digit_type(1))
    throw std::domain_error("mp_int::slow_modinv: no inverse exists"); // TODO return false if no inverse exists?

  /* if its too low */
  while (C.compare_to_digit(0) == -1)
    C += b;
  
  /* too big */
  while (C.compare_magnitude(b) != -1)
    C -= b;
  
  /* C is now the inverse */
  swap(C);
}

/* computes the modular inverse via binary extended euclidean algorithm, 
 * that is *this = 1 / *this mod b 
 *
 * Based on slow modinv except this is optimized for the case where b is 
 * odd as per HAC Note 14.64 on pp. 610
 */
template<class Al, class T>
void mp_int<Al,T>::fast_modinv(const mp_int& b)
{
  if (b.is_even())
    throw std::domain_error("mp_int::fast_modinv: b must be odd");

  /* x == modulus, y == value to invert */
  mp_int x = b;

  /* we need y = |a| */
  mp_int y = *this % b;

  /* 3. u=x, v=y, A=1, B=0, C=0,D=1 */
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

  if (!u.is_zero())
    goto top;

  /* now a = C, b = D, gcd == g*v */

  /* if v != 1 then there is no inverse */
  if (v != digit_type(1))
    throw std::domain_error("mp_int::fast_modinv: no inverse exists");

  /* D is now the inverse */
  while (D.sign_ == -1)
    D += b;

  swap(D);
}

