// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_PRIME_HPP
#define BOOST_MP_MATH_INTEGER_PRIME_HPP

#include <boost/mp_math/integer/random.hpp>
#include <boost/mp_math/integer/detail/multiplier.hpp>
#include <boost/mp_math/integer/detail/prime_tab.hpp>


namespace boost {
namespace mp_math {

// this one simply divides x by some small primes found in prime_tab
// returns false if definitely composite, returns true if probably prime
// this test is to be used to quickly filter out composite numbers
// for example testing for 2,5,7 will eliminate 54% of all odd numbers
// testing for all primes less than 100 eliminates 70%, for all primes
// less than 256 it is 80%.
struct primality_division_test
{
  typedef bool result_type;

  template<class ApInt>
  bool operator()(const ApInt& p) const;
};


template<class ApInt>
bool primality_division_test::operator()(const ApInt& p) const
{
  typedef typename ApInt::digit_type digit_type;
  typedef detail::prime_tab<digit_type>      prime_tab;

  if (p.is_even())
  {
    if (p != digit_type(2))
      return false;
    else
      return true;
  }

  for (int i = 0; i < prime_tab::num_primes; ++i)
  {
    const digit_type small_prime = static_cast<digit_type>(prime_tab::values[i]);
    const ApInt remainder = p % small_prime;

    if (remainder == digit_type(0))
    {
      if (p != small_prime)
        return false;
      else
        return true; // definitely prime
    }
  }
  return true;
}



// fermat primality test:
// If an integer p is a prime number, then for all integers a,
// dividing both a p and a by p gives a result with the same remainder.
//
// if p is in fact prime then a**p = a (mod p)
// for all 0 < a < p
// or alternatively: a**(p-1) = 1 (mod p)
//
// returns true if p is probably prime
// NOTE: may return true for carmichael numbers
template<
  class Distribution = uniform_integer<>
>
class primality_fermat_test
{
  const unsigned int rounds_;

public:

  typedef Distribution distribution_type;
  typedef bool         result_type;

  explicit primality_fermat_test(unsigned int rounds)
  :
    rounds_(rounds)
  {}

  template<class Engine, class ApInt>
  bool operator()(Engine& e, const ApInt& p) const;
};


template<class Distribution>
template<class Engine, class ApInt>
bool
primality_fermat_test<Distribution>::operator()
  (Engine& eng, const ApInt& p) const
{
  typedef typename ApInt::digit_type digit_type;

  const digit_type one = 1;
  const ApInt p1(p-one);

  distribution_type rng(digit_type(2), p1);

  modpow_ctx<ApInt> ctx;
  for (unsigned int i = 0; i < rounds_; ++i)
  {
    ApInt base = rng(eng);

    base |= one; // test only odd bases

    const ApInt tmp = modpow(base, p1, p, &ctx);

    if (tmp != one)
      return false; // definitely composite!
  }

  return true;
}


// Miller-Rabin test
// This is an improved version of the Fermat test that will also detect
// carmichael numbers.
// The probability that a composite number is reported as prime
// is less than 1/(4**k) where k is the number of rounds
template<
  class Distribution = uniform_integer<>
>
class primality_miller_rabin_test
{
  const unsigned int rounds_;

  static const struct rtable
  {
    unsigned k, t;
  } sizes_[];

public:

  typedef Distribution distribution_type;
  typedef bool         result_type;

  explicit primality_miller_rabin_test(unsigned int rounds = 0)
  :
    rounds_(rounds)
  {}

  // p must be odd
  template<class Engine, class ApInt>
  bool operator()(Engine& e, const ApInt& p) const;

  // return the recommended number of rounds for numbers of size 'bits'
  // so that the probability of error is lower than 2^-96
  static unsigned int recommended_number_of_rounds(unsigned bits);
};

// This table comes from libtommath, HAC has one too but with slightly lower
// numbers of rounds.
template<class Distribution>
const typename primality_miller_rabin_test<Distribution>::rtable
primality_miller_rabin_test<Distribution>::sizes_[] =
{
  {   128,    28 },
  {   256,    16 },
  {   384,    10 },
  {   512,     7 },
  {   640,     6 },
  {   768,     5 },
  {   896,     4 },
  {  1024,     4 }
};


template<class Distribution>
template<class Engine, class ApInt>
bool
primality_miller_rabin_test<Distribution>::operator()
  (Engine& eng, const ApInt& p) const
{
  assert(p.is_odd());

  unsigned int r;
  if (rounds_)
    r = rounds_;
  else
    r = recommended_number_of_rounds(p.precision());

  ApInt n = abs(p);
  --n;

  const typename ApInt::size_type s = n.count_trailing_zero_bits();
  n >>= s; // p-1 / 2**s

  const typename ApInt::digit_type one = 1;
  const ApInt p_minus_one(p - one);
  distribution_type rng(one, p_minus_one);

  modpow_ctx<ApInt> ctx;
  for (unsigned int i = 0; i < r; ++i)
  {
    const ApInt base = rng(eng);
    ApInt y = modpow(base, n, p, &ctx);

    for (typename ApInt::size_type j = 0; j < s; ++j)
    {
      const bool b = (y != one) && (y != p_minus_one);
      y *= y;
      y %= p;
      if (b && (y == one))
        return false;
    }
    if (y != one)
      return false;
  }
  return true;
}

template<class Distribution>
unsigned int
primality_miller_rabin_test<Distribution>::
recommended_number_of_rounds(unsigned bits)
{
  int i;

  for (i = 0; i < 8; ++i)
  {
    if (sizes_[i].k == bits)
      return sizes_[i].t;
    else if (sizes_[i].k > bits)
      return (i == 0) ? sizes_[0].t : sizes_[i - 1].t;
  }

  return sizes_[i-1].t + 1;
}




template<class ApInt, class PrimalityTest>
inline bool is_prime(const ApInt& x, PrimalityTest f = PrimalityTest())
{
  return f(x);
}


template<
  class PrimalityTest,
  class Dist = uniform_integer_bits<>
>
struct prime_generator
{
  typedef PrimalityTest              primality_test;
  typedef Dist                       distribution_type;
  typedef typename Dist::result_type result_type;

  explicit prime_generator(std::size_t bits,
                           PrimalityTest test = PrimalityTest())
  :
    bits_(bits),
    test_(test)
  {}

  template<class Engine>
  result_type operator()(Engine& e) const;

  std::size_t bits_;
  const primality_test test_;
};

template<class PrimalityTest, class Dist>
template<class Engine>
typename Dist::result_type
prime_generator<PrimalityTest,Dist>::operator()(Engine& eng) const
{
  distribution_type dist(bits_);
  result_type candidate;
  do
  {
    candidate = dist(eng);
    candidate |= typename result_type::digit_type(1); // make odd
  } while (!is_prime(candidate, test_));

  return candidate;
}


// A prime p is called safe if (p-1)/2 is also prime
template<
  class PrimalityTest,
  class Dist = uniform_integer_bits<>
>
struct safe_prime_generator
{
  typedef PrimalityTest              primality_test;
  typedef Dist                       distribution_type;
  typedef typename Dist::result_type result_type;

  explicit safe_prime_generator(std::size_t bits,
                                PrimalityTest test = PrimalityTest())
  :
    bits_(bits),
    test_(test)
  {}

  template<class Engine>
  result_type operator()(Engine& e) const;

  std::size_t bits_;
  const primality_test test_;
};


template<class PrimalityTest, class Dist>
template<class Engine>
typename Dist::result_type
safe_prime_generator<PrimalityTest,Dist>::operator()(Engine& eng) const
{
  prime_generator<PrimalityTest, Dist> prime_gen(bits_-1, test_);
  result_type p;
  do
  {
    do
    {
      p = prime_gen(eng);
      detail::multiplier<result_type>::multiply_by_2(p);
      ++p;
    } while (!is_prime(p, test_));
  // Catch extremely rare case, this occurs if the carry from ++p ripples
  // through the whole number thereby adding one more bit to it.
  } while (p.precision() > bits_);

  return p;
}


} // namespace mp_math
} // namespace boost

#endif

