// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_MODPOW_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_MODPOW_HPP

#include <boost/mp_math/integer/detail/modular_reduction.hpp>

namespace boost {
namespace mp_math {
namespace detail {


// modpow_ctx allows us to precalculate and store certain values so that modpow
// calls with the same modulus are sped up. We should probably store a pointer
// value to the original modulus value here and then assert that we are handed
// the same modulus in subsequent invocations.

template<class ApInt>
struct modpow_ctx
{
  typedef typename ApInt::digit_type  digit_type;
  typedef typename ApInt::size_type   size_type;
  typedef typename ApInt::traits_type traits_type;

  // dr means diminished radix
  enum modulus_type_t
  {                           // R is our radix
    mod_restricted_dr,        // m = R^k - d; d <= R
    mod_unrestricted_dr,      // m = 2^k - d; d <= R
    mod_unrestricted_dr_slow, // m = 2^k - d; d <  d^(k/2)
    mod_odd,
    mod_generic
  }modulus_type;

  modpow_ctx() : precalculated(false) {}

  modulus_type_t do_detect(const ApInt& m) const;

  void detect_modulus_type(const ApInt& m)
  {
    modulus_type = do_detect(m);
  }

  void precalculate(const ApInt& m);

  ApInt mu;
  digit_type rho;
  bool precalculated;
};


template<class ApInt>
typename modpow_ctx<ApInt>::modulus_type_t
modpow_ctx<ApInt>::do_detect(const ApInt& m) const
{
  if (m.size() == 1)
    return mod_unrestricted_dr;

  size_type count = 0;

  const size_type bits = m.precision() % traits_type::radix_bits;

  if (!bits && m[m.size()-1] == traits_type::max_digit_value)
    ++count;

  for (typename ApInt::const_reverse_iterator d = m.rbegin() + 1;
       d != m.rend(); ++d)
  {
    if (*d != traits_type::max_digit_value)
      break;
    else
      ++count;
  }

  // if all bits are set
  if (count == m.size() - 1)
    return mod_restricted_dr;

  // if all bits until the most significant digit are set
  if (count == m.size() - 2)
  {
    bool all_bits_set = true;

    // handle the remaining bits in the most significant digit
    digit_type mask = 1;
    for (size_type i = 0; i < bits; ++i)
    {
      if ((m[m.size()-1] & mask) == 0)
      {
        all_bits_set = false;
        break;
      }
      mask <<= 1;
    }
    if (all_bits_set)
      return mod_unrestricted_dr;
  }

  // if more than half of the bits are set
  if (count >= m.size() / 2)
    return mod_unrestricted_dr_slow;

  if (m.is_odd())
    return mod_odd;

  return mod_generic;
}

template<class ApInt>
void modpow_ctx<ApInt>::precalculate(const ApInt& m)
{
  switch (modulus_type)
  {
    case mod_restricted_dr:
    {
      rho = traits_type::max_digit_value - m[0] + 1U;
      break;
    }
    case mod_unrestricted_dr:
    {
      const size_type p = m.precision();

      ApInt tmp;
      power<ApInt>::pow2(tmp, p);
      base::adder<ApInt>::subtract_smaller_magnitude(tmp, m);

      rho = tmp[0];
      break;
    }
    case mod_unrestricted_dr_slow:
    {
      ApInt tmp;

      power<ApInt>::pow2(tmp, m.precision());
      mu = tmp - m;
      break;
    }
    case mod_odd:
    {
      assert(m.is_odd());

      // fast inversion mod 2**k
      //
      // Based on the fact that
      //
      // XA = 1 (mod 2**n)  =>  (X(2-XA)) A = 1 (mod 2**2n)
      //                    =>  2*X*A - X*X*A*A = 1
      //                    =>  2*(1) - (1)     = 1
      const digit_type b = m[0];

      static const size_type S = std::numeric_limits<digit_type>::digits;

      digit_type x = (((b + 2) & 4) << 1) + b; // here x*a==1 mod 2**4
      x *= 2 - b * x;                          // here x*a==1 mod 2**8
      if (S != 8)
        x *= 2 - b * x;                        // here x*a==1 mod 2**16
      if (S == 64 || !(S == 8 || S == 16))
        x *= 2 - b * x;                        // here x*a==1 mod 2**32
      if (S == 64)
        x *= 2 - b * x;                        // here x*a==1 mod 2**64

      // rho = -1/m mod b
      rho = traits_type::max_digit_value - x + 1U;
      break;
    }
    case mod_generic:
    {
      // mu = b^2k/m
      power<ApInt>::pow2(mu, m.size() * 2 * traits_type::digit_bits);
      mu /= m;
      break;
    }
  }

  precalculated = true;
}


template<class ApInt>
struct modular_power
{
  typedef typename ApInt::size_type   size_type;
  typedef typename ApInt::digit_type  digit_type;
  typedef typename ApInt::traits_type traits_type;

  // computes z = base^exp % m
  static void modpow(ApInt& z,
                     const ApInt& base,
                     const ApInt& exp,
                     const ApInt& m,
                     modpow_ctx<ApInt>* ctx);

  static void modpow_with_ctx(ApInt& z,
                              const ApInt& base,
                              const ApInt& exp,
                              const ApInt& m,
                              const modpow_ctx<ApInt>& ctx);

  static void barret_modpow(ApInt& z,
                            const ApInt& base,
                            const ApInt& exp,
                            const ApInt& m,
                            int redmode,
                            const ApInt& mu);

  static void montgomery_modpow(ApInt& z,
                                const ApInt& base,
                                const ApInt& exp,
                                const ApInt& m,
                                int redmode,
                                digit_type mp);
};


template<class ApInt>
void modular_power<ApInt>::modpow(ApInt& z,
                                  const ApInt& base,
                                  const ApInt& exp,
                                  const ApInt& mod,
                                  modpow_ctx<ApInt>* ctx = 0)
{
  if (mod.is_negative())
    throw std::domain_error("modpow: modulus must be positive");

  if (exp.is_negative())
  {
    //modpow(z, modinv(base, mod), abs(exp), mod, ctx);
    return;
  }

  if (!ctx)
  {
    modpow_ctx<ApInt> tmp_ctx;
    tmp_ctx.detect_modulus_type(mod);
    tmp_ctx.precalculate(mod);
    modpow_with_ctx(z, base, exp, mod, tmp_ctx);
  }
  else
  {
    if (!ctx->precalculated)
    {
      ctx->detect_modulus_type(mod);
      ctx->precalculate(mod);
    }
    modpow_with_ctx(z, base, exp, mod, *ctx);
  }
}

template<class ApInt>
void modular_power<ApInt>::modpow_with_ctx(ApInt& z,
                                           const ApInt& base,
                                           const ApInt& exp,
                                           const ApInt& mod,
                                           const modpow_ctx<ApInt>& ctx)
{
  typedef modpow_ctx<ApInt> ctx_t;

  switch (ctx.modulus_type)
  {
    case ctx_t::mod_restricted_dr:
      montgomery_modpow(z, base, exp, mod, 1, ctx.rho); break;
    case ctx_t::mod_unrestricted_dr:
      montgomery_modpow(z, base, exp, mod, 2, ctx.rho); break;
    case ctx_t::mod_unrestricted_dr_slow:
      barret_modpow    (z, base, exp, mod, 1, ctx.mu); break;
    case ctx_t::mod_odd:
      montgomery_modpow(z, base, exp, mod, 0, ctx.rho); break;
    case ctx_t::mod_generic:
    default:
      barret_modpow    (z, base, exp, mod, 0, ctx.mu); break;
  }
}

template<class ApInt>
void modular_power<ApInt>::barret_modpow(ApInt& z,
                                         const ApInt& base,
                                         const ApInt& exp,
                                         const ApInt& m,
                                         int redmode,
                                         const ApInt& mu)
{
  void (*redux)(ApInt&, const ApInt&, const ApInt&);

  // find window size
  const size_type x = exp.precision();
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

  if (redmode == 0)
    redux = &modular_reducer<ApInt>::barrett_reduce;
  else
    redux = &modular_reducer<ApInt>::unrestricted_dr_slow_reduce;

  // The M table contains powers of the base, e.g. M[i] = base**i % m
  // The first half of the table is not computed though except for M[0] and M[1]
  ApInt M[256];
  M[1] = base % m;

  // compute the value at M[1<<(winsize-1)] by squaring M[1] (winsize-1) times
  M[1 << (winsize - 1)] = M[1];

  for (size_type i = 0; i < (winsize - 1); ++i)
  {
    const size_type offset = 1 << (winsize - 1);
    multiplier<ApInt>::sqr(M[offset]);
    // reduce modulo m
    (*redux)(M[offset], m, mu);
  }

  // create upper table, that is M[i] = M[i-1] * M[1] (mod P)
  // for i = (2**(winsize - 1) + 1) to (2**winsize - 1)
  for (size_type i = (1 << (winsize - 1)) + 1;
       i < (size_type(1) << winsize);
       ++i)
  {
    M[i] = M[i-1] * M[1];
    (*redux)(M[i], m, mu);
  }

  // setup result
  z = digit_type(1);

  int mode   = 0;
  int bitcnt = 1;
  digit_type buf = 0;
  typename ApInt::const_reverse_iterator exp_digit = exp.rbegin();
  size_type bitcpy = 0;
  int bitbuf = 0;

  for (;;) // while (exp_digit != exp.rend())
  {
    // grab next digit as required
    if (--bitcnt == 0)
    {
      if (exp_digit == exp.rend())
        break;

      // read next digit and reset the bitcnt
      buf    = *exp_digit++;
      bitcnt = traits_type::radix_bits;
    }

    // grab the next msb from the exponent
    int y = (buf >> static_cast<digit_type>(traits_type::radix_bits - 1)) & 1;
    buf <<= digit_type(1);

    // if the bit is zero and mode == 0 then we ignore it
    // These represent the leading zero bits before the first 1 bit
    // in the exponent. Technically this opt is not required but it
    // does lower the # of trivial squaring/reductions used
    if (mode == 0 && y == 0)
      continue;

    // if the bit is zero and mode == 1 then we square
    if (mode == 1 && y == 0)
    {
      multiplier<ApInt>::sqr(z);
      (*redux)(z, m, mu);
      continue;
    }

    // else we add it to the window
    bitbuf |= (y << (winsize - ++bitcpy));
    mode    = 2;

    if (bitcpy == winsize)
    {
      // ok window is filled so square as required and multiply
      // square first
      for (size_type i = 0; i < winsize; ++i)
      {
        multiplier<ApInt>::sqr(z);
        (*redux)(z, m, mu);
      }

      // then multiply
      z *= M[bitbuf];
      (*redux)(z, m, mu);

      // empty window and reset
      bitcpy = 0;
      bitbuf = 0;
      mode   = 1;
    }
  }

  // if bits remain then square/multiply
  if (mode == 2 && bitcpy > 0)
  {
    // square then multiply if the bit is set
    for (size_type i = 0; i < bitcpy; ++i)
    {
      multiplier<ApInt>::sqr(z);
      (*redux)(z, m, mu);

      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0)
      {
        z *= M[1];
        (*redux)(z, m, mu);
      }
    }
  }
}

// computes Y == G**X mod P, HAC pp.616, Algorithm 14.85
//
// Uses a left-to-right k-ary sliding window to compute the modular exponentiation.
// The value of k changes based on the size of the exponent.
//
// Uses Montgomery or Diminished Radix reduction [whichever appropriate]
template<class ApInt>
void modular_power<ApInt>::montgomery_modpow(ApInt& z,
                                             const ApInt& base,
                                             const ApInt& exp,
                                             const ApInt& m,
                                             int redmode,
                                             digit_type mp)
{
  void (*redux)(ApInt&, const ApInt&, digit_type);

  // find window size
  const size_type x = exp.precision();
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

  if (redmode == 0)
    redux = &modular_reducer<ApInt>::montgomery_reduce;
  else if (redmode == 1)
    redux = &modular_reducer<ApInt>::restricted_dr_reduce;
  else
    redux = &modular_reducer<ApInt>::unrestricted_dr_reduce;

  // create M table
  // The first half of the table is not computed though except for M[0] and M[1]
  ApInt M[256];

  if (redmode == 0)
  {
    // now we need R mod m
    modular_reducer<ApInt>::montgomery_normalize(z, m);
    // now set M[1] to G * R mod m
    M[1] = (base * z) % m;
  }
  else
  {
    z = digit_type(1);
    M[1] = base % m;
  }

  // compute the value at M[1<<(winsize-1)] by squaring M[1] (winsize-1) times
  M[1 << (winsize - 1)] = M[1];

  for (size_type i = 0; i < (winsize - 1); ++i)
  {
    const size_type offset = 1 << (winsize - 1);
    multiplier<ApInt>::sqr(M[offset]);
    (*redux)(M[offset], m, mp);
  }

  // create upper table
  for (size_type i = (1 << (winsize - 1)) + 1;
       i < (size_type(1) << winsize); ++i)
  {
    multiplier<ApInt>::mul(M[i], M[i-1], M[1]);
    (*redux)(M[i], m, mp);
  }

  // set initial mode and bit cnt
  int mode   = 0;
  int bitcnt = 1;
  digit_type buf = 0;
  typename ApInt::const_reverse_iterator exp_digit = exp.rbegin();
  size_type bitcpy = 0;
  unsigned int bitbuf = 0;

  for (;;)
  {
    // grab next digit as required
    if (--bitcnt == 0)
    {
      if (exp_digit == exp.rend())
        break;
      // read next digit and reset bitcnt
      buf    = *exp_digit++;
      bitcnt = traits_type::radix_bits;
    }

    // grab the next msb from the exponent
    int y = (buf >> (traits_type::radix_bits - 1)) & 1;
    buf <<= digit_type(1);

    // if the bit is zero and mode == 0 then we ignore it
    // These represent the leading zero bits before the first 1 bit
    // in the exponent.  Technically this opt is not required but it
    // does lower the # of trivial squaring/reductions used
    if (mode == 0 && y == 0)
      continue;

    // if the bit is zero and mode == 1 then we square
    if (mode == 1 && y == 0)
    {
      multiplier<ApInt>::sqr(z);
      (*redux)(z, m, mp);
      continue;
    }

    // else we add it to the window
    bitbuf |= (y << (winsize - ++bitcpy));
    mode    = 2;

    if (bitcpy == winsize)
    {
      // ok window is filled so square as required and multiply
      for (size_type i = 0; i < winsize; ++i)
      {
        multiplier<ApInt>::sqr(z);
        (*redux)(z, m, mp);
      }

      z *= M[bitbuf];
      (*redux)(z, m, mp);

      // empty window and reset
      bitcpy = 0;
      bitbuf = 0;
      mode   = 1;
    }
  }

  // if bits remain then square/multiply
  if (mode == 2 && bitcpy > 0)
  {
    for (size_type i = 0; i < bitcpy; ++i)
    {
      multiplier<ApInt>::sqr(z);
      (*redux)(z, m, mp);

      // get next bit of the window
      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0)
      {
        z *= M[1];
        (*redux)(z, m, mp);
      }
    }
  }

  // Fixup result if Montgomery reduction is used. Recall that any value in a
  // Montgomery system is actually multiplied by R mod n. So we have to reduce
  // one more time to cancel out the factor of R.
  if (redmode == 0)
    (*redux)(z, m, mp);
}


} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

