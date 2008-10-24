// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/* computes a = 2**b 
 *
 * Simple algorithm which zeroes the int, grows it then just sets one bit
 * as required.
 */
template<class A, class T>
void mp_int<A,T>::pow2(typename mp_int<A,T>::size_type b)
{
  /* grow a to accomodate the single bit */
  grow_capacity(b / digit_bits + 1);

  /* set the used count of where the bit will go */
  used_ = b / digit_bits + 1;

  // set all bits to zero
  std::memset(digits_, 0, used_ * sizeof(digit_type));
  
  /* put the single bit in its place */
  digits_[b / digit_bits] = digit_type(1) << (b % digit_bits);
}

/* calculate c = a**b  using a square-multiply algorithm */
template<class A, class T>
void mp_int<A,T>::pow(digit_type b)
{
  mp_int g(*this);

  /* set initial result */
  *this = digit_type(1);

  for (int x = 0; x < digit_bits; ++x)
  {
    sqr();

    /* if the bit is set multiply */
    if (b & (digit_type(1) << (digit_bits - 1)))
      *this *= g;

    /* shift to next bit */
    b <<= 1;
  }
}

// this is a shell function that calls either the normal or Montgomery
// modpow functions. Originally the call to the montgomery code was
// embedded in the normal function but that wasted alot of stack space
// for nothing (since 99% of the time the Montgomery code would be called)
//
// y = g^x mod p
// *this = *this^exp mod m
template<class A, class T>
void mp_int<A,T>::modpow(const mp_int& exp, const mp_int& m)
{
  if (m.is_negative())
    throw std::domain_error("modpow: modulus m must be positive");

  if (exp.is_negative())
  {
    // first compute 1 / *this mod m
    modinv(m);

    // and now compute (1 / *this)**|exp| instead of *this**exp [exp < 0]
    modpow(abs(exp), m);
    return;
  }

  /* modified diminished radix reduction */
  if (m.reduce_is_2k_l())
  {
    //std::cout << "barret_modpow1\n";
    barret_modpow(exp, m, 1);
    return;
  }

  /* is it a DR modulus? */
  int dr = m.is_dr_modulus();

  /* if not, is it a unrestricted DR modulus? */
  if (!dr)
    dr = m.reduce_is_2k() << 1;
    
  /* if the modulus is odd or dr == true use the montgomery method */
  if (m.is_odd() || dr)
  {
    //std::cout << "fast_modpow dr = " << dr << "\n";
    fast_modpow(exp, m, dr);
  }
  else
  {
    //std::cout << "barret_modpow2\n";
    /* otherwise use the generic Barrett reduction technique */
    barret_modpow(exp, m, 0);
  }
}

template<class A, class T>
void mp_int<A,T>::barret_modpow(const mp_int& exp, const mp_int& m, int redmode)
{
  static const int TAB_SIZE = 256;
  mp_int M[TAB_SIZE];

  void (mp_int::*redux)(const mp_int&, const mp_int&);

  /* find window size */
  size_type x = exp.precision();
  size_type winsize;
  if (x <= 7)
    winsize = 2;
  else if (x <= 36)
    winsize = 3;
  else if (x <= 140)
    winsize = 4;
  else if (x <= 450)
    winsize = 5;
  else if (x <= 1303)
    winsize = 6;
  else if (x <= 3529)
    winsize = 7;
  else
    winsize = 8;

  /* create mu, used for Barrett reduction */
  mp_int mu;

  mp_int mm(m);
  if (redmode == 0)
  {
    mu.reduce_setup(m);
    //std::cout << "redux = reduce\n";
    redux = &mp_int::reduce;
  }
  else
  {
    mu = mm.reduce_2k_l_setup();
    //std::cout << "redux = reduce_2k_l\n";
    redux = &mp_int::reduce_2k_l;
  }

  /* create M table
   *
   * The M table contains powers of the base, 
   * e.g. M[x] = G**x mod P
   *
   * The first half of the table is not 
   * computed though accept for M[0] and M[1]
   */
  M[1] = *this % mm;


  /* compute the value at M[1<<(winsize-1)] by squaring 
   * M[1] (winsize-1) times 
   */
  M[1 << (winsize - 1)] = M[1];

  for (x = 0; x < (winsize - 1); x++)
  {
    /* square it */
    M[1 << (winsize - 1)].sqr();
    /* reduce modulo P */
    (M[1 << (winsize - 1)].*redux)(mm, mu);
  }

  /* create upper table, that is M[x] = M[x-1] * M[1] (mod P)
   * for x = (2**(winsize - 1) + 1) to (2**winsize - 1)
   */
  for (x = (1 << (winsize - 1)) + 1; x < (size_type(1) << winsize); ++x)
  {
    M[x] = M[x-1] * M[1];
    (M[x].*redux)(mm, mu);
  }

  /* setup result */
  mp_int res = digit_type(1);

  /* set initial mode and bit cnt */
  int mode   = 0;
  int bitcnt = 1;
  digit_type buf = 0;
  int digidx = exp.used_ - 1;
  size_type bitcpy = 0;
  int bitbuf = 0;

  for (;;)
  {
    /* grab next digit as required */
    if (--bitcnt == 0)
    {
      /* if digidx == -1 we are out of digits */
      if (digidx == -1)
        break;
      /* read next digit and reset the bitcnt */
      buf    = exp.digits_[digidx--];
      bitcnt = valid_bits;
    }

    /* grab the next msb from the exponent */
    int y = (buf >> static_cast<digit_type>(valid_bits - 1)) & 1;
    buf <<= digit_type(1);

    /* if the bit is zero and mode == 0 then we ignore it
     * These represent the leading zero bits before the first 1 bit
     * in the exponent. Technically this opt is not required but it
     * does lower the # of trivial squaring/reductions used
     */
    if (mode == 0 && y == 0)
      continue;

    /* if the bit is zero and mode == 1 then we square */
    if (mode == 1 && y == 0)
    {
      res.sqr();
      (res.*redux)(mm, mu);
      continue;
    }

    /* else we add it to the window */
    bitbuf |= (y << (winsize - ++bitcpy));
    mode    = 2;

    if (bitcpy == winsize)
    {
      /* ok window is filled so square as required and multiply  */
      /* square first */
      for (x = 0; x < winsize; ++x)
      {
        res.sqr();
        (res.*redux)(mm, mu);
      }

      /* then multiply */
      res *= M[bitbuf];
      (res.*redux)(mm, mu);

      /* empty window and reset */
      bitcpy = 0;
      bitbuf = 0;
      mode   = 1;
    }
  }

  /* if bits remain then square/multiply */
  if (mode == 2 && bitcpy > 0)
  {
    /* square then multiply if the bit is set */
    for (x = 0; x < bitcpy; ++x)
    {
      res.sqr();
      (res.*redux)(mm, mu);
      //mu = res * mm;

      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0)
      {
        /* then multiply */
        res *= M[1];
        (res.*redux)(mm, mu);
      }
    }
  }

  swap(res);
}

/* computes Y == G**X mod P, HAC pp.616, Algorithm 14.85
 *
 * Uses a left-to-right k-ary sliding window to compute the modular exponentiation.
 * The value of k changes based on the size of the exponent.
 *
 * Uses Montgomery or Diminished Radix reduction [whichever appropriate]
 */
template<class A, class T>
void mp_int<A,T>::fast_modpow(const mp_int& exp, const mp_int& m, int redmode)
{
  static const int TAB_SIZE = 256;
  mp_int M[TAB_SIZE];

  /* use a pointer to the reduction algorithm.  This allows us to use
   * one of many reduction algorithms without modding the guts of
   * the code with if statements everywhere.
   */
  void (mp_int::*redux)(const mp_int&, digit_type);
  
  /* find window size */
  size_type x = exp.precision();
  size_type winsize;
  if (x <= 7)
    winsize = 2;
  else if (x <= 36)
    winsize = 3;
  else if (x <= 140)
    winsize = 4;
  else if (x <= 450)
    winsize = 5;
  else if (x <= 1303)
    winsize = 6;
  else if (x <= 3529)
    winsize = 7;
  else
    winsize = 8;
  
  digit_type mp;
  /* determine and setup reduction code */
  if (redmode == 0)
  {
    /* now setup montgomery  */
    mp = m.montgomery_setup();

    /* automatically pick the comba one if available (saves quite a few calls/ifs) */
    /*if (((m.used_ * 2 + 1) < mp_warray) &&
         m.used_ < 1 << ((CHAR_BIT * sizeof(word_type)) - (2 * valid_bits)))
    {
      std::cout << "redux = fast_montgomery_reduce\n";
      redux = &mp_int::fast_montgomery_reduce;
    }
    else 
    {*/
      /* use slower baseline Montgomery method */
      //std::cout << "redux = montgomery_reduce\n";
      redux = &mp_int::montgomery_reduce;
    //}
  }
  else if (redmode == 1)
  {
    /* setup DR reduction for moduli of the form B**k - b */
    mp = m.dr_setup();
    redux = &mp_int::dr_reduce;
  }
  else
  {
    /* setup DR reduction for moduli of the form 2**k - b */
    mp = m.reduce_2k_setup();
    redux = &mp_int::reduce_2k;
  }

  /* setup result */
  mp_int res;

  /* create M table
   *
   * The first half of the table is not computed though accept for M[0] and M[1]
   */
  if (redmode == 0)
  {
    /* now we need R mod m */
    res.montgomery_calc_normalization(m);
    /* now set M[1] to G * R mod m */
    M[1] = (*this * res) % m;
  }
  else
  {
    res = digit_type(1);
    M[1] = *this % m;
  }

  /* compute the value at M[1<<(winsize-1)] by squaring M[1] (winsize-1) times */
  M[1 << (winsize - 1)] = M[1];

  for (x = 0; x < (winsize - 1); ++x)
  {
    M[1 << (winsize - 1)].sqr();
    (M[1 << (winsize - 1)].*redux)(m, mp);
  }

  /* create upper table */
  for (x = (1 << (winsize - 1)) + 1; x < (size_type(1) << winsize); ++x)
  {
    M[x] = M[x-1] * M[1];
    (M[x].*redux)(m, mp);
  }

  /* set initial mode and bit cnt */
  int mode   = 0;
  int bitcnt = 1;
  digit_type buf = 0;
  int digidx = exp.used_ - 1;
  size_type bitcpy = 0;
  unsigned int bitbuf = 0;

  for (;;)
  {
    /* grab next digit as required */
    if (--bitcnt == 0)
    {
      /* if digidx == -1 we are out of digits so break */
      if (digidx == -1)
        break;
      /* read next digit and reset bitcnt */
      buf    = exp.digits_[digidx--];
      bitcnt = valid_bits;
    }

    /* grab the next msb from the exponent */
    int y = (buf >> (valid_bits - 1)) & 1;
    buf <<= digit_type(1);

    /* if the bit is zero and mode == 0 then we ignore it
     * These represent the leading zero bits before the first 1 bit
     * in the exponent.  Technically this opt is not required but it
     * does lower the # of trivial squaring/reductions used
     */
    if (mode == 0 && y == 0)
      continue;

    /* if the bit is zero and mode == 1 then we square */
    if (mode == 1 && y == 0)
    {
      res.sqr();
      (res.*redux)(m, mp);
      continue;
    }

    /* else we add it to the window */
    bitbuf |= (y << (winsize - ++bitcpy));
    mode    = 2;

    if (bitcpy == winsize)
    {
      /* ok window is filled so square as required and multiply  */
      /* square first */
      for (x = 0; x < winsize; ++x)
      {
        res.sqr();
        (res.*redux)(m, mp);
      }

      /* then multiply */
      res *= M[bitbuf];
      (res.*redux)(m, mp);

      /* empty window and reset */
      bitcpy = 0;
      bitbuf = 0;
      mode   = 1;
    }
  }

  /* if bits remain then square/multiply */
  if (mode == 2 && bitcpy > 0)
  {
    /* square then multiply if the bit is set */
    for (x = 0; x < bitcpy; ++x)
    {
      res.sqr();
      (res.*redux)(m, mp);

      /* get next bit of the window */
      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0)
      {
        /* then multiply */
        res *= M[1];
        (res.*redux)(m, mp);
      }
    }
  }

  /* fixup result if Montgomery reduction is used
   * recall that any value in a Montgomery system is
   * actually multiplied by R mod n.  So we have
   * to reduce one more time to cancel out the factor
   * of R.
   */
  if (redmode == 0)
    (res.*redux)(m, mp);

  swap(res);
}



template<class A, class T>
inline mp_int<A,T> pow(const mp_int<A,T>& x, typename mp_int<A,T>::digit_type n)
{
  mp_int<A,T> tmp(x);
  tmp.pow(n);
  return tmp;
}

template<class A, class T>
inline mp_int<A,T> modpow(const mp_int<A,T>& base,
                          const mp_int<A,T>& exp,
                          const mp_int<A,T>& mod)
{
  mp_int<A,T> tmp(base);
  tmp.modpow(exp, mod);
  return tmp;
}




