
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Contains the function definitions for the prime number functions.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
    #include "../random.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_INLINE std::vector<int> sieveOfEratosthenes(int upTo) {
    std::vector<int> sieve;
    sieve.reserve(upTo);

    // Zero and one aren't prime, by definition.
    sieve.push_back(0);
    sieve.push_back(0);

    for (int p = 2; p < upTo; ++p) sieve.push_back(p);

    std::vector<int> ret;

    int *p = &sieve[0], *e = p + upTo;
    while (p < e) {
        if (*p == 0) { ++p; continue; }
        ret.push_back(*p);

        int *p2 = p + (*p);
        while (p2 < e) { *p2 = 0; p2 += *p; }

        ++p;
    }

    return ret;
}

// The Miller-Rabin Probabilistic Test of Primality
BOOST_XINT_INLINE int isProbablePrimeBaseB(const data_t n, const data_t b,
    callback_t callback)
{
    const data_t one(n.new_holder(), 1), nMinus1(n - one);

    // Find r and a satisfying: n - 1 = 2^a * r, r odd
    data_t r(nMinus1);
    unsigned long a = 0;
    while (r.is_even()) { r >>= 1; ++a; }

    // We check the congruence class of b^((2^i)r) % n for each i
    // from 0 to a - 1. If any one is correct, then n passes.
    // Otherwise, n fails.
    data_t test(n.new_holder());
    powmod(test, b, r, n);
    if (test == one || test == nMinus1) return 1; // Better than 3/4 chance it's prime

    while (a-- > 0) {
        sqrmod(test, test, n);
        if (test == nMinus1) return 1;
    }

    if (callback && !callback()) return -1;
    return 0;
}

BOOST_XINT_INLINE int is_prime(const data_t n, callback_t callback) {
    data_t tmp(n.new_holder(), 2);
    if (n < tmp) throw exceptions::invalid_argument("xint::is_prime cannot "
        "test numbers below 2");

    // First we trial-divide it by the primes below 2000
    static const std::vector<int> low_primes(sieveOfEratosthenes(2000));
    const data_t zero(n.new_holder());
    std::vector<int>::const_iterator i = low_primes.begin(), ie =
        low_primes.end();
    for (; i != ie; ++i) {
        tmp.set_unsigned(*i);
        if ((n % tmp) == zero)
            return (n == tmp);
    }

    // Ensure that we've got a random generator.
    static std::auto_ptr<default_random_generator> gen(new
        default_random_generator);
    detail::random_generator<default_random_generator> random(*gen.get());

    // Run the number through the Miller-Rabin Probabilistic Test of Primality
    // a few times to see if it's actually (probably) prime.
    for (int count = 0; count < 5; ++count) {
        unsigned int k = random();
        tmp.set_unsigned(k);
        int isP = isProbablePrimeBaseB(n, tmp, callback);
        if (isP <= 0) return isP;
    }
    return 1; // Appears to be prime!
}

BOOST_XINT_INLINE void random_prime(data_t& target, base_random_generator& gen,
    size_t size_in_bits, callback_t callback)
{
    if (size_in_bits < 2) throw exceptions::invalid_argument("cannot create "
        "prime numbers smaller than two bits");

    // Call the callback for the first time
    if (callback && !callback()) { target.set(0); return; }

    // If, by rare chance, the number we're testing gets larger than the number
    // of bits requested, we have to start over with a new random number of the
    // proper size.
    data_t pe(target.new_holder());
    pow2(pe, size_in_bits + 1);

    const data_t two(target.new_holder(), 2);
    while (1) {
        random_by_size(target, gen, size_in_bits, true, true, false);
        while (target < pe) {
            int r = is_prime(target, callback);
            if (r < 0) { target.set(0); return; }
            if (r == 1) return;
            target += two;
        }
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
