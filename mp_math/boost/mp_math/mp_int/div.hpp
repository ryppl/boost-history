// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

template<class A, class T>
typename mp_int<A,T>::digit_type
mp_int<A,T>::divide_by_digit(digit_type b)
{
  if (b == 0)
    throw std::domain_error("mp_int::divide_by_digit: division by zero");

  if (b == 1 || is_zero())
    return 0;

  const bool is_power_of_two = (b & (b-1)) == 0;
  if (is_power_of_two)
  {
    for (int i = 0; i < valid_bits; ++i)
    {
      if (b == digit_type(1) << i)
      {
        const digit_type remainder = digits_[0] & ((digit_type(1) << i) - 1);
        *this >>= i;
        return remainder;
      }
    }
  }

  word_type w = 0;
  for (reverse_iterator d = rbegin(); d != rend(); ++d)
  {
    w = (w << static_cast<word_type>(valid_bits)) | static_cast<word_type>(*d);
    digit_type tmp;
    if (w >= b)
    {
      tmp = static_cast<digit_type>(w / b);
      w -= tmp * b;
    }
    else
      tmp = 0;
    *d = tmp;
  }

  clamp();
  if (is_zero())
    sign_ = 1;
 
  return static_cast<digit_type>(w);
}

/* b = a/2 */
template<class A, class T>
void mp_int<A,T>::divide_by_2()
{
  digit_type carry = 0;
  for (reverse_iterator d = rbegin(); d != rend(); ++d)
  {
    /* get the carry for the next iteration */
    const digit_type rr = *d & 1;
    /* shift the current digit, add in carry and store */
    *d = (*d >> 1) | (carry << (valid_bits - 1));
    /* forward carry to next iteration */
    carry = rr;
  }
  clamp();
  if (is_zero())
    sign_ = 1;
}

/* divide by three (based on routine from MPI and the GMP manual) */
template<class A, class T>
typename mp_int<A,T>::digit_type
mp_int<A,T>::divide_by_3()
{
  /* b = 2**valid_bits / 3 */
  const word_type b = (word_type(1) << static_cast<word_type>(valid_bits)) / word_type(3);

  word_type w = 0;
  for (reverse_iterator d = rbegin(); d != rend(); ++d)
  {
    w = (w << static_cast<word_type>(valid_bits)) | static_cast<word_type>(*d);
    
    word_type t;
    if (w >= 3)
    {
      /* multiply w by [1/3] */
      t = (w * b) >> static_cast<word_type>(valid_bits);

      /* now subtract 3 * [w/3] from w, to get the remainder */
      w -= t+t+t;

      /* fixup the remainder as required since
       * the optimization is not exact.
       */
      while (w >= 3)
      {
        t += 1;
        w -= 3;
      }
    }
    else
      t = 0;

    *d = static_cast<digit_type>(t);
  }  

  // *this is now the quotient
  // return remainder
  return static_cast<digit_type>(w);
}

/* shift right by a certain bit count */
template<class A, class T>
void mp_int<A,T>::shift_right(size_type b, mp_int* remainder)
{
  if (b == 0)
  {
    if (remainder)
      remainder->zero();
    return;
  }

  /* get the remainder */
  mp_int t;
  if (remainder)
  {
    *remainder = *this;
    remainder->modulo_2_to_the_power_of(b);
  }

  /* shift by as many digits in the bit count */
  if (b >= static_cast<size_type>(valid_bits))
    shift_digits_right(b / valid_bits);

  /* shift any bit count < valid_bits */
  const digit_type D = b % valid_bits;
  if (D)
  {
    const digit_type mask = (digit_type(1) << D) - 1;

    /* shift for lsb */
    const digit_type shift = valid_bits - D;

    digit_type carry = 0;
    for (reverse_iterator d = rbegin(); d != rend(); ++d)
    {
      /* get the lower bits of this word in a temp */
      const digit_type rr = *d & mask;

      /* shift the current word and mix in the carry bits from the previous word */
      *d = (*d >> D) | (carry << shift);

      /* set the carry to the carry bits of the current word found above */
      carry = rr;
    }
  }
  clamp();
  if (is_zero())
    sign_ = 1;
}

/* integer signed division. 
 * c*b + d == a [e.g. a/b, c=quotient, d=remainder]
 * HAC pp.598 Algorithm 14.20
 *
 * Note that the description in HAC is horribly 
 * incomplete.  For example, it doesn't consider 
 * the case where digits are removed from 'x' in 
 * the inner loop.  It also doesn't consider the 
 * case that y has fewer than three digits, etc..
 *
 * The overall algorithm is as described as 
 * 14.20 from HAC but fixed to treat these cases.
*/
// divide *this by rhs, optionally store remainder
template<class A, class T>
void mp_int<A,T>::divide(const mp_int& rhs, mp_int* remainder)
{
  if (rhs.is_zero())
    throw std::domain_error("mp_int::divide: division by zero");

  /* if *this < rhs then q=0, r = *this */
  if (compare_magnitude(rhs) == -1)
  {
    if (remainder)
      *remainder = *this;
    zero();
    return;
  }

  mp_int q;
  q.grow_capacity(used_ + 2);
  q.used_ = used_ + 2;
  std::memset(q.digits_, 0, q.used_ * sizeof(digit_type));

  mp_int x(*this);
  mp_int y(rhs);

  /* fix the sign */
  const int neg = (sign_ == rhs.sign_) ? 1 : -1;
  x.sign_ = y.sign_ = 1;

  /* normalize both x and y, ensure that y >= beta/2, [beta == 2**valid_bits] */
  size_type norm = y.precision() % valid_bits;
  if (norm < valid_bits-1)
  {
    norm = valid_bits - 1 - norm;
    x <<= norm;
    y <<= norm;
  }
  else
    norm = 0;

  /* note hac does 0 based, so if used==5 then its 0,1,2,3,4, e.g. use 4 */
  const size_type n = x.used_ - 1;
  const size_type t = y.used_ - 1;

  // find leading digit of the quotient
  /* while (x >= y*beta**(n-t)) do { q[n-t] += 1; x -= y*beta**(n-t) } */
  y.shift_digits_left(n - t); /* y = y*beta**(n-t) */

  while (x.compare(y) != -1)
  {
    ++q[n - t];
    x -= y;
  }

  /* reset y by shifting it back down */
  y.shift_digits_right(n - t);

  // find the remainder of the digits
  /* step 3. for i from n down to (t + 1) */
  for (size_type i = n; i >= (t + 1); i--)
  {
    if (i > x.used_)
      continue;

    /* step 3.1 if xi == yt then set q{i-t-1} to beta-1, 
     * otherwise set q{i-t-1} to (xi*beta + x{i-1})/yt */
    if (x[i] == y[t])
      q[i - t - 1] = std::numeric_limits<digit_type>::max();
    else
    {
      word_type tmp = static_cast<word_type>(x[i])
                   << static_cast<word_type>(valid_bits);
      tmp |= x[i - 1];
      tmp /= y[t];
      q[i - t - 1] = static_cast<digit_type>(tmp);
    }

    // now fixup quotient estimation
    /* while (q{i-t-1} * (yt * beta + y{t-1})) > 
             xi * beta**2 + xi-1 * beta + xi-2 
     
       do q{i-t-1} -= 1; 
    */

    mp_int t1, t2;
    t1.grow_capacity(3);
    t2.grow_capacity(3);

    ++q[i - t - 1];
    do
    {
      --q[i - t - 1];

      /* find left hand */
      t1.zero();
      t1[0] = (t == 0) ? 0 : y[t - 1];
      t1[1] = y[t];
      t1.used_ = 2;
      t1.multiply_by_digit(q[i - t - 1]);

      /* find right hand */
      t2[0] = (i < 2) ? 0 : x[i - 2];
      t2[1] = (i == 0) ? 0 : x[i - 1];
      t2[2] = x[i];
      t2.used_ = 3;
    } while (t1.compare_magnitude(t2) == 1);

    /* step 3.3 x = x - q{i-t-1} * y * beta**{i-t-1} */
    t1 = y;
    t1.multiply_by_digit(q[i - t -1]);
    t1.shift_digits_left(i - t - 1);
    x -= t1;

    /* if x < 0 then { x = x + y*beta**{i-t-1}; q{i-t-1} -= 1; } */
    if (x.sign_ == -1)
    {
      t1 = y;
      t1.shift_digits_left(i - t -1);
      x += t1;

      --q[i - t - 1] = q[i - t - 1];
    }
  }

  /* now q is the quotient and x is the remainder 
   * [which we have to normalize] 
   */
  
  /* get sign before writing to c */
  x.sign_ = x.is_zero() ? 1 : sign_;

  q.clamp();
  swap(q);
  sign_ = neg;

  if (remainder)
  {
    x >>= norm;
    remainder->swap(x);
  }
}

/*
template<class A, class T>
void divide(const mp_int<A,T>& x, const mp_int<A,T>& y, mp_int<A,T>& q, mp_int<A,T>& r)
{
  divide(x, y, &q, &r);
}
*/
