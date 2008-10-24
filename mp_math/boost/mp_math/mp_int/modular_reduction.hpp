// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/* pre-calculate the value required for Barrett reduction
 * For a given modulus "b" it calulates the value required in "a"
 */
template<class A, class T>
void mp_int<A,T>::reduce_setup(const mp_int& b)
{
  pow2(b.used_ * 2 * digit_bits);
  *this /= b;
}

// reduces *this mod m, assumes 0 < *this < m**2, mu is 
// precomputed via reduce_setup.
// From HAC pp.604 Algorithm 14.42
template<class A, class T>
void mp_int<A,T>::reduce(const mp_int& m, const mp_int& mu)
{
  const size_type k = m.used_;

  mp_int q(*this);

  /* q1 = x / b**(k-1)  */
  q.shift_digits_right(k - 1);

  /* according to HAC this optimization is ok */
  if (k > digit_type(1) << (valid_bits - 1))
    q *= mu;
  else
    q.fast_mul_high_digits(mu, k);

  /* q3 = q2 / b**(k+1) */
  q.shift_digits_right(k + 1);

  /* x = x mod b**(k+1), quick (no division) */
  modulo_2_to_the_power_of(valid_bits * (k + 1));

  /* q = q * m mod b**(k+1), quick (no division) */
  q.mul_digits(m, k + 1);

  /* x = x - q */
  *this -= q;

  /* If x < 0, add b**(k+1) to it */
  if (is_negative())
  {
    q = digit_type(1);
    q.shift_digits_left(k + 1);
    *this += q;
  }

  /* Back off if it's too big */
  while (compare(m) != -1)
    sub_smaller_magnitude(m);
}

/* setups the montgomery reduction stuff */
template<class A, class T>
typename mp_int<A,T>::digit_type
mp_int<A,T>::montgomery_setup() const
{
  /* fast inversion mod 2**k
   *
   * Based on the fact that
   *
   * XA = 1 (mod 2**n)  =>  (X(2-XA)) A = 1 (mod 2**2n)
   *                    =>  2*X*A - X*X*A*A = 1
   *                    =>  2*(1) - (1)     = 1
   */
  const digit_type b = digits_[0];

  if (is_even())
    throw std::domain_error("montgomery_setup: integer must be odd");

  static const size_type S = sizeof(digit_type) * CHAR_BIT;

  digit_type x = (((b + 2) & 4) << 1) + b; /* here x*a==1 mod 2**4 */
  x *= 2 - b * x;               /* here x*a==1 mod 2**8 */
  if (S != 8)
    x *= 2 - b * x;               /* here x*a==1 mod 2**16 */
  if (S == 64 || !(S == 8 || S == 16))
    x *= 2 - b * x;               /* here x*a==1 mod 2**32 */
  if (S == 64)
    x *= 2 - b * x;               /* here x*a==1 mod 2**64 */

  /* rho = -1/m mod b */
  const digit_type rho = (word_type(1) << (static_cast<word_type>(valid_bits))) - x;
  return rho;
}

/* computes xR**-1 == x (mod N) via Montgomery Reduction */
template<class A, class T>
void mp_int<A,T>::montgomery_reduce(const mp_int& n, digit_type rho)
{
  /* can the fast reduction [comba] method be used?
   *
   * Note that unlike in mul you're safely allowed *less*
   * than the available columns [255 per default] since carries
   * are fixed up in the inner loop.
   */
  const size_type digs = n.used_ * 2 + 1;
/*  const size_type ws = sizeof(word_type) * CHAR_BIT;
  if (digs < mp_warray && n.used_ < 1 << (ws - (2 * valid_bits)))
  {
    fast_montgomery_reduce(n, rho);
    return;
  }*/
  /* grow the input as required */
  grow_capacity(digs);
  std::memset(digits_ + used_, 0, (capacity_ - used_) * sizeof(digit_type));
  used_ = digs;

  for (size_type i = 0; i < n.used_; ++i)
  {
    /* mu = ai * rho mod b
     *
     * The value of rho must be precalculated via
     * montgomery_setup() such that
     * it equals -1/n0 mod b this allows the
     * following inner loop to reduce the
     * input one digit at a time
     */
    const digit_type mu = static_cast<word_type>(digits_[i]) * rho;

    /* a = a + mu * m * b**i */
  
    /* alias for digits of the modulus */
    digit_type* tmpn = n.digits_;

    /* alias for the digits of x [the input] */
    digit_type* tmpx = digits_ + i;

    digit_type carry = 0;

    /* Multiply and add in place */
    for (size_type j = 0; j < n.used_; ++j)
    {
      /* compute product and sum */
      const word_type r = static_cast<word_type>(mu)
                        * static_cast<word_type>(*tmpn++)
                        + static_cast<word_type>(carry)
                        + static_cast<word_type>(*tmpx);
      
      carry = r >> digit_bits;

      /* fix digit */
      *tmpx++ = r;
    }
    /* At this point the i'th digit of x should be zero */
    
    /* propagate carries upwards as required*/
    while (carry)
    {
      const word_type r = static_cast<word_type>(*tmpx) + carry;
      *tmpx++ = r;
      carry = r >> digit_bits;
    }
  }

  /* at this point the n.used'th least
   * significant digits of x are all zero
   * which means we can shift x to the
   * right by n.used digits and the
   * residue is unchanged.
   */

  /* x = x/b**n.used */
  clamp();
  if (is_zero())
    sign_ = 1;
  shift_digits_right(n.used_);

  /* if x >= n then x = x - n */
  if (compare_magnitude(n) != -1)
    sub_smaller_magnitude(n);
}

/*
 * shifts with subtractions when the result is greater than b.
 *
 * The method is slightly modified to shift B unconditionally upto just under
 * the leading bit of b. This saves alot of multiple precision shifting.
 */
template<class A, class T>
void mp_int<A,T>::montgomery_calc_normalization(const mp_int& b)
{
  /* how many bits of last digit does b use */
  size_type bits = b.precision() % valid_bits;

  if (b.used_ > 1)
    pow2((b.used_ - 1) * valid_bits + bits - 1);
  else
  {
    *this = digit_type(1);
    bits = 1;
  }

  /* now compute C = A * B mod b */
  for (int x = bits - 1; x < valid_bits; ++x)
  {
    multiply_by_2();
    if (compare_magnitude(b) != -1)
      sub_smaller_magnitude(b);
  }
}

/* computes xR**-1 == x (mod N) via Montgomery Reduction
 *
 * This is an optimized implementation of montgomery_reduce
 * which uses the comba method to quickly calculate the columns of the
 * reduction.
 *
 * Based on Algorithm 14.32 on pp.601 of HAC.
*/
template<class A, class T>
void mp_int<A,T>::fast_montgomery_reduce(const mp_int& n, digit_type rho)
{
  word_type W[mp_warray];

  /* grow a as required */
  grow_capacity(n.used_ + 1);

  /* first we have to get the digits of the input into
   * an array of double precision words W[...]
   */

  /* copy the digits of a into W[0..a->used-1] */
  for (size_type i = 0; i < used_; ++i)
    W[i] = digits_[i];

  /* zero the high words of W[a->used..m->used*2] */
  std::memset(W + used_, 0, (n.used_ * 2 + 1) * sizeof(word_type));

  /* now we proceed to zero successive digits
   * from the least significant upwards
   */
  for (size_type i = 0; i < n.used_; ++i)
  {
    /* mu = ai * m' mod b
     *
     * We avoid a double precision multiplication (which isn't required)
     * by casting the value down to a mp_digit.  Note this requires
     * that W[ix-1] have  the carry cleared (see after the inner loop)
     */
    const digit_type mu = ((W[i] & mp_mask) * rho) & mp_mask;

    /* a = a + mu * m * b**i
     *
     * This is computed in place and on the fly.  The multiplication
     * by b**i is handled by offseting which columns the results
     * are added to.
     *
     * Note the comba method normally doesn't handle carries in the
     * inner loop In this case we fix the carry from the previous
     * column since the Montgomery reduction requires digits of the
     * result (so far) [see above] to work.  This is
     * handled by fixing up one carry after the inner loop. The
     * carry fixups are done in order so after these loops the
     * first m->used words of W[] have the carries fixed
     */
    /* inner loop */
    for (size_type j = 0; j < n.used_; ++j)
      W[i+j] += static_cast<word_type>(mu) * static_cast<word_type>(n.digits_[j]);

    /* now fix carry for next digit, W[ix+1] */
    W[i + 1] += W[i] >> static_cast<word_type>(valid_bits);
  }

  /* now we have to propagate the carries and
   * shift the words downward [all those least
   * significant digits we zeroed].
   */

  /* now fix rest of carries */

  for (size_type i = n.used_ + 1; i <= n.used_ * 2 + 1; ++i)
    W[i] += W[i-1] >> static_cast<word_type>(valid_bits);

  /* copy out, A = A/b**n
   *
   * The result is A/b**n but instead of converting from an
   * array of mp_word to mp_digit than calling mp_rshd
   * we just copy them in the right order
   */

  for (size_type i = 0; i < n.used_ + 1; ++i)
    digits_[i] = W[n.used_ + i] & static_cast<word_type>(mp_mask);

  /* set the max used and clamp */
  used_ = n.used_ + 1;
  clamp();
  if (is_zero())
    sign_ = 1;

  /* if A >= m then A = A - m */
  if (compare_magnitude(n) != -1)
    sub_smaller_magnitude(n);
}

/* determines the setup value */
template<class A, class T>
typename mp_int<A,T>::digit_type
mp_int<A,T>::dr_setup() const
{
   /* the casts are required if valid_bits is one less than
    * the number of bits in a valid_bits [e.g. valid_bits==31]
    */
   const digit_type d = (word_type(1) << static_cast<word_type>(valid_bits))
                      - static_cast<word_type>(digits_[0]);
   return d;
}

/* reduce "x" in place modulo "n" using the Diminished Radix algorithm.
 *
 * Based on algorithm from the paper
 *
 * "Generating Efficient Primes for Discrete Log Cryptosystems"
 *                 Chae Hoon Lim, Pil Joong Lee,
 *          POSTECH Information Research Laboratories
 *
 * The modulus must be of a special format [see manual]
 *
 * Has been modified to use algorithm 7.10 from the LTM book instead
 *
 * Input *this must be in the range 0 <= *this <= (n-1)**2
 */
template<class A, class T>
void mp_int<A,T>::dr_reduce(const mp_int& n, digit_type k)
{
  /* m = digits in modulus */
  const size_type m = n.used_;

  /* ensure that *this has at least 2m digits */
  grow_capacity(m + m);
  std::memset(digits_ + used_, 0, (m + m - used_) * sizeof(digit_type));

/* top of loop, this is where the code resumes if
 * another reduction pass is required.
 */
top:
  /* set carry to zero */
  digit_type mu = 0;

  /* compute (*this mod B**m) + k * [*this/B**m] inline and inplace */
  for (size_type i = 0; i < m; ++i)
  {
    const word_type r = static_cast<word_type>(digits_[m+i])
                      * static_cast<word_type>(k) + digits_[i] + mu;
    digits_[i] = static_cast<digit_type>(r);
    mu = static_cast<digit_type>(r >> static_cast<word_type>(digit_bits));
  }

  /* set final carry */
  digits_[m] = mu;

  /* zero words above m */
  if (used_ > m + 1) // guard against overflow
    std::memset(digits_ + m + 1, 0, (used_ - (m + 1)) * sizeof(digit_type));

  /* clamp, sub and return */
  clamp();
  if (is_zero())
    sign_ = 1;

  /* if *this >= n then subtract and reduce again
   * Each successive "recursion" makes the input smaller and smaller.
   */
  if (compare_magnitude(n) != -1)
  {
    sub_smaller_magnitude(n);
    goto top;
  }
}

/* determines if a number is a valid DR modulus */
template<class A, class T>
bool mp_int<A,T>::is_dr_modulus() const
{
  /* must be at least two digits */
  if (used_ < 2)
    return false;

  /* must be of the form b**k - a [a <= b] so all
   * but the first digit must be equal to -1 (mod b).
   */
  for (size_type i = 1; i < used_; ++i)
  {
    if (digits_[i] != digit_max)
      return false;
  }
  return true;
}

/* determines the setup value */
template<class A, class T>
typename mp_int<A,T>::digit_type
mp_int<A,T>::reduce_2k_setup() const
{
  mp_int tmp;
  const size_type p = precision();

  tmp.pow2(p);
  tmp.sub_smaller_magnitude(*this);

  return tmp.digits_[0];
}

/* reduces *this modulo n where n is of the form 2**p - d */
template<class A, class T>
void mp_int<A,T>::reduce_2k(const mp_int& n, digit_type d)
{
  const size_type p = n.precision();

top:

  mp_int q(*this);
  
  /* q = a/2**p, a = a mod 2**p */
  q.shift_right(p, this);

  if (d != 1)
    /* q = q * d */
    q.multiply_by_digit(d);
  /* a = a + q */
  add_magnitude(q);

  if (compare_magnitude(n) != -1)
  {
    sub_smaller_magnitude(n);
    goto top;
  }
}

/* determines if mp_reduce_2k can be used */
template<class A, class T>
bool mp_int<A,T>::reduce_is_2k() const
{
  if (used_ == 1)
    return true;
  else if (used_ > 1)
  {
    const size_type bits = precision();
    size_type j = 1;
    digit_type k = 1;
    
    /* Test every bit from the second digit up, must be 1 */
    for (size_type i = valid_bits; i < bits; ++i)
    {
      if ((digits_[j] & k) == 0)
        return false;
      k <<= 1;
      if (k > mp_mask) // FIXME this can never happen with the new code
      {
        ++j;
        k = 1;
      }
    }
  }
  return true;
}

/* determines the setup value */
template<class A, class T>
mp_int<A,T> mp_int<A,T>::reduce_2k_l_setup()
{
  mp_int tmp;

  tmp.pow2(precision());
  return tmp - *this;
}

/* reduces *this modulo n where n is of the form 2**p - d 
   This differs from reduce_2k since "d" can be larger
   than a single digit.
*/
template<class A, class T>
void mp_int<A,T>::reduce_2k_l(const mp_int& n, const mp_int& d)
{
  const size_type p = n.precision();

top:

  mp_int q(*this);

  /* q = a/2**p, a = a mod 2**p */
  q.shift_right(p, this);

  /* q = q * d */
  q *= d;

  /* a = a + q */
  add_magnitude(q);

  if (compare_magnitude(n) != -1)
  {
    sub_smaller_magnitude(n);
    goto top;
  }
}

/* determines if reduce_2k_l can be used */
template<class A, class T>
bool mp_int<A,T>::reduce_is_2k_l() const
{
  if (used_ == 1)
    return true;
  else if (used_ > 1)
  {
    size_type count = 0;
    // if more than half of the digits are -1 we're sold
    for (size_type i = 0; i < used_; ++i)
      if (digits_[i] == digit_max)
        ++count;
    return count >= used_ / 2 ? true : false;
  }
  return false;
}

