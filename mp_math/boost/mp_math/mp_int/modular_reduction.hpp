// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


// reduces x mod m, assumes 0 < x < m**2, mu is precomputed.
// From HAC pp.604 Algorithm 14.42
template<class A, class T>
void barrett_reduce(mp_int<A,T>& x, const mp_int<A,T>& m, const mp_int<A,T>& mu)
{
  typedef typename mp_int<A,T>::digit_type digit_type;

  const typename mp_int<A,T>::size_type k = m.size();

  mp_int<A,T> q(x);

  // q = x / base**(k-1)
  q.shift_digits_right(k - 1);

  // according to HAC this optimization is ok
  if (k > digit_type(1) << (mp_int<A,T>::valid_bits - 1))
    q *= mu;
  else
    q.fast_mul_high_digits(mu, k);

  // q = q / base**(k+1)
  q.shift_digits_right(k + 1);

  // r = x mod base**(k+1)
  x.modulo_2_to_the_power_of(mp_int<A,T>::valid_bits * (k + 1));

  // q = q * m mod base**(k+1)
  q.mul_digits(m, k + 1);

  x -= q;

  // If x < 0, add base**(k+1) to it
  if (x.is_negative())
  {
    q = digit_type(1);
    q.shift_digits_left(k + 1);
    x += q;
  }

  while (x >= m)
    x.sub_smaller_magnitude(m);
}

/* computes xR**-1 == x (mod N) via Montgomery Reduction */
template<class A, class T>
void montgomery_reduce(mp_int<A,T>& x,
                       const mp_int<A,T>& n,
                       typename mp_int<A,T>::digit_type rho)
{
  typedef typename mp_int<A,T>::digit_type digit_type;
  typedef typename mp_int<A,T>::word_type  word_type;
  typedef typename mp_int<A,T>::size_type  size_type;

  const size_type digs = n.size() * 2 + 1;

  x.grow_capacity(digs);
  std::memset(x.digits() + x.size(), 0, (x.capacity() - x.size()) * sizeof(digit_type));
  x.set_size(digs);

  // TODO rewrite both for loops in terms of const_iterator nd = n.begin();...
  for (size_type i = 0; i < n.size(); ++i)
  {
    // mu = x[i] * rho (mod base)
    // The value of rho must be precalculated such that it equals -1/n0 mod b
    // this allows the following inner loop to reduce the input one digit at a
    // time.
    const digit_type mu = static_cast<word_type>(x[i]) * rho;

    // x = x + mu * m * base**i
  
    // alias for digits of the modulus
    const digit_type* tmpn = n.digits();

    // alias for the digits of x
    digit_type* tmpx = x.digits() + i;

    digit_type carry = 0;

    // Multiply and add in place
    for (size_type j = 0; j < n.size(); ++j)
    {
      // compute product and sum
      const word_type r = static_cast<word_type>(mu)
                        * static_cast<word_type>(*tmpn++)
                        + static_cast<word_type>(carry)
                        + static_cast<word_type>(*tmpx);
      
      carry = static_cast<digit_type>(r >> mp_int<A,T>::digit_bits);

      *tmpx++ = r;
    }
    // At this point the i'th digit of x should be zero
    
    // propagate carries upwards as required
    while (carry)
    {
      const word_type r = static_cast<word_type>(*tmpx) + carry;
      *tmpx++ = r;
      carry = r >> mp_int<A,T>::digit_bits;
    }
  }

  // at this point the n.used'th least significant digits of x are all zero
  // which means we can shift x to the right by n.used digits and the residue is
  // unchanged.

  // x = x/base**n.size()
  x.clamp();
  if (x.is_zero())
    x.set_sign(1);

  x.shift_digits_right(n.size());

  if (x.compare_magnitude(n) != -1)
    x.sub_smaller_magnitude(n);
}

// shifts with subtractions when the result is greater than n.
// The method is slightly modified to shift B unconditionally upto just under
// the leading bit of n. This saves alot of multiple precision shifting.
template<class A, class T>
void montgomery_normalize(mp_int<A,T>& x, const mp_int<A,T>& n)
{
  // how many bits of last digit does n use
  typename mp_int<A,T>::size_type bits = n.precision() % mp_int<A,T>::valid_bits;

  if (n.size() > 1)
    x.pow2((n.size() - 1) * mp_int<A,T>::valid_bits + bits - 1);
  else
  {
    x = typename mp_int<A,T>::digit_type(1);
    bits = 1;
  }

  // now compute C = A * B mod n
  for (int i = bits - 1; i < mp_int<A,T>::valid_bits; ++i)
  {
    x.multiply_by_2();
    if (x.compare_magnitude(n) != -1)
      x.sub_smaller_magnitude(n);
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
void fast_montgomery_reduce(mp_int<A,T>& x,
                            const mp_int<A,T>& n,
                            typename mp_int<A,T>::digit_type rho)
{
  typedef typename mp_int<A,T>::digit_type digit_type;
  typedef typename mp_int<A,T>::word_type  word_type;
  typedef typename mp_int<A,T>::size_type  size_type;

  word_type W[512];

  x.grow_capacity(n.used_ + 1);

  for (size_type i = 0; i < x.size(); ++i)
    W[i] = x[i];

  // zero the high words of W[a->used..m->used*2]
  std::memset(W + x.size(), 0, (n.size() * 2 + 1) * sizeof(word_type));

  // now we proceed to zero successive digits
  // from the least significant upwards
  for (size_type i = 0; i < n.size(); ++i)
  {
    // mu = ai * m' mod b
    //
    // We avoid a double precision multiplication (which isn't required) by
    // casting the value down to a mp_digit.  Note this requires that W[ix-1]
    // have  the carry cleared (see after the inner loop)
    const digit_type mu = static_cast<digit_type>(W[i])
                        * static_cast<word_type>(rho);

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
    for (size_type j = 0; j < n.size(); ++j)
      W[i+j] += static_cast<word_type>(mu) * static_cast<word_type>(n[j]);

    // now fix carry for next digit, W[ix+1]
    W[i + 1] += W[i] >> static_cast<word_type>(mp_int<A,T>::valid_bits);
  }

  // now we have to propagate the carries and shift the words downward [all
  // those least significant digits we zeroed].

  // now fix rest of carries
  for (size_type i = n.size() + 1; i <= n.size() * 2 + 1; ++i)
    W[i] += W[i-1] >> static_cast<word_type>(mp_int<A,T>::valid_bits);

  // copy out, A = A/b**n
  //
  // The result is A/b**n but instead of converting from an array of word_type
  // to digit_type than calling mp_rshd we just copy them in the right order
  for (size_type i = 0; i < n.size() + 1; ++i)
    x[i] = static_cast<digit_type>(W[n.size() + i]);

  // set the max used and clamp
  x.set_size(n.size() + 1);
  x.clamp();
  if (x.is_zero())
    x.set_sign(1);

  // if A >= m then A = A - m
  if (x.compare_magnitude(n) != -1)
    x.sub_smaller_magnitude(n);
}


// reduce "x" modulo "n" using the Diminished Radix algorithm.
// Based on algorithm from the paper
//
// "Generating Efficient Primes for Discrete Log Cryptosystems"
//                 Chae Hoon Lim, Pil Joong Lee,
//          POSTECH Information Research Laboratories
//
// The modulus must be of a special format [see manual]
//
// Has been modified to use algorithm 7.10 from the LTM book instead
//
// Input x must be in the range 0 <= x <= (n-1)**2
template<class A, class T>
void restricted_dr_reduce(mp_int<A,T>& x,
                          const mp_int<A,T>& n,
                          typename mp_int<A,T>::digit_type k)
{
  typedef typename mp_int<A,T>::digit_type digit_type;
  typedef typename mp_int<A,T>::word_type  word_type;
  typedef typename mp_int<A,T>::size_type  size_type;

  const size_type m = n.size();

  x.grow_capacity(m + m);
  std::memset(x.digits() + x.size(), 0, (m + m - x.size()) * sizeof(digit_type));

top:
  // set carry to zero
  digit_type mu = 0;

  // compute (r mod B**m) + k * [r/B**m] inline and inplace
  for (size_type i = 0; i < m; ++i)
  {
    const word_type r = static_cast<word_type>(x[m+i])
                      * static_cast<word_type>(k) + x[i] + mu;
    x[i] = static_cast<digit_type>(r);
    mu = static_cast<digit_type>(r >> static_cast<word_type>(mp_int<A,T>::digit_bits));
  }

  // set final carry
  x[m] = mu;

  // zero words above m
  if (x.size() > m + 1) // guard against overflow
    std::memset(x.digits() + m + 1, 0, (x.size() - (m + 1)) * sizeof(digit_type));

  x.clamp();
  if (x.is_zero())
    x.set_sign(1);

  if (x.compare_magnitude(n) != -1)
  {
    x.sub_smaller_magnitude(n);
    goto top;
  }
}

// reduces x modulo n where n is of the form 2**p - d
template<class A, class T>
void unrestricted_dr_reduce(mp_int<A,T>& x,
                            const mp_int<A,T>& n,
                            typename mp_int<A,T>::digit_type d)
{
  const typename mp_int<A,T>::size_type p = n.precision();

top:

  mp_int<A,T> q(x);
  
  /* q = a/2**p, r = r mod 2**p */
  q.shift_right(p, &x);

  if (d != 1)
    q.multiply_by_digit(d);
  
  x.add_magnitude(q);

  if (x.compare_magnitude(n) != -1)
  {
    x.sub_smaller_magnitude(n);
    goto top;
  }
}

// reduces x modulo n where n is of the form 2**p - d. This differs from
// unrestricted_dr_reduce since "d" can be larger than a single digit.
template<class A, class T>
void unrestricted_dr_slow_reduce(mp_int<A,T>& x,
                                 const mp_int<A,T>& n,
                                 const mp_int<A,T>& d)
{
  const typename mp_int<A,T>::size_type p = n.precision();

top:

  mp_int<A,T> q(x);

  // q = x/2**p, r = r mod 2**p
  q.shift_right(p, &x);

  q *= d;

  x.add_magnitude(q);

  if (x.compare_magnitude(n) != -1)
  {
    x.sub_smaller_magnitude(n);
    goto top;
  }
}


