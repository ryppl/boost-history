
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for functions related to prime numbers.
*/

#include "../xint.hpp"

#include <vector>

namespace xint {

namespace {

std::vector<int> sieveOfEratosthenes(int upTo) {
    std::vector<int> sieve;
    sieve.reserve(upTo);

    // Zero and one aren't prime, by this definition.
    sieve.push_back(0);
    sieve.push_back(0);

    for (int p=2; p<upTo; ++p) sieve.push_back(p);

    std::vector<int> ret;

    int *p=&sieve[0], *e=p+upTo;
    while (p<e) {
        if (*p==0) { ++p; continue; }
        ret.push_back(*p);

        int *p2=p+(*p);
        while (p2<e) { *p2=0; p2+=*p; }

        ++p;
    }

    return ret;
}

// The Miller-Rabin Probabilistic Test of Primality
int isProbablePrimeBaseB(const integer& n, const integer &b, callback_t
    callback)
{
    const integer nMinus1(n - integer::one());

    // Find r and a satisfying: n-1=2^a * r, r odd
    integer r=nMinus1;
    unsigned long a=0;
    while (r.even()) { r >>= 1; ++a; }

    // We check the congruence class of b^((2^i)r) % n for each i
    // from 0 to a - 1. If any one is correct, then n passes.
    // Otherwise, n fails.
    integer test=powmod(b, r, n);
    if (test==integer::one() || test==nMinus1) return 1; // Better than 3/4 chance it's prime

    while (a-->0) {
        test=sqrmod(test, n);
        if (test==nMinus1) return 1;
    }

    if (callback && !callback()) return -1;
    return 0;
}

} // namespace

int is_prime(const integer& n, callback_t callback) {
    if (n < 2) {
        if (exceptions_allowed()) throw std::invalid_argument("xint::is_prime cannot test numbers below 2");
        else return -1;
    }

    // First we trial-divide it by the primes below 2000
    static const std::vector<int> cLowPrimes(sieveOfEratosthenes(2000));
    std::vector<int>::const_iterator i=cLowPrimes.begin(), ie=cLowPrimes.end();
    for (; i!=ie; ++i) if ((n % *i)==0) return (n==*i);

    // Run the number through the Miller-Rabin Probabilistic Test of Primality
    // a few times to see if it's actually (probably) prime.
    for (int count=0; count<5; ++count) {
        unsigned int k=detail::get_random();
        int isP=isProbablePrimeBaseB(n, abs(k), callback);
        if (isP <= 0) return isP;
    }
    return 1; // Appears to be prime!
}

integer random_prime(size_t sizeInBits, callback_t callback) {
    // Call the callback for the first time
    if (callback && !callback()) return integer(not_a_number());

    integer pe=pow2(sizeInBits+1);
    while (1) {
        integer p(random_by_size(sizeInBits, true, true));
        while (p < pe) {
            int r=is_prime(p, callback);
            if (r < 0) return integer(not_a_number());
            if (r == 1) return p;
            p+=2;
        }
    }
}

} // namespace xint
