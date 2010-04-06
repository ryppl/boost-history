
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
    \brief Contains the definitions for functions related to prime numbers.
*/

#include "../boost/xint/xint.hpp"

#include <vector>

namespace boost {
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

/*! \brief Tests an integer for primality.

\param[in] n The number to test.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns 1 if \c n seems to be prime, 0 if it isn't, or -1 if the provided
callback function cancelled the operation.

\exception std::invalid_argument if \c n is less than 2.

\note If exceptions are blocked, it will return -1 instead of throwing an
exception.

\remarks
This function uses the Rabin-Miller probabilistic primality test. There is an
infinitesimally small chance that it will think that a composite number is
actually prime, but that is so small that it can be ignored for most practical
purposes. If even that chance is too much, you can run this function over the
number several times, it will use different random "witness" numbers each time.

\see \ref primes
*/
int is_prime(const integer& n, callback_t callback) {
    if (n < 2) {
        if (exceptions_allowed()) throw std::invalid_argument("xint::is_prime "
            "cannot test numbers below 2");
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

/*! \brief Generate a randomly-generated prime number of a particular bit-size.

\param[in] size_in_bits The number of bits that you want the returned value to
have.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns A randomly-generated prime integer with the specified number of bits,
or zero if the provided callback function cancelled the operation.

\exception std::invalid_argument if \c size_in_bits is less than two.

\note This function uses xint::is_prime. See the description of it for details
of its limitations.

\par
It also uses the library's currently-set random number generator. See the \link
random Random Number Functions page\endlink for details on its limitations and
requirements, and how to get cryptographically-secure random numbers.

\see \ref primes
*/
integer random_prime(size_t size_in_bits, callback_t callback) {
    if (size_in_bits < 2) {
        if (exceptions_allowed()) throw std::invalid_argument(
            "xint::random_prime cannot create prime numbers smaller than two "
            "bits");
        else return integer(not_a_number());
    }

    // Call the callback for the first time
    if (callback && !callback()) return integer::zero();

    integer pe=pow2(size_in_bits+1);
    while (1) {
        integer p(random_by_size(size_in_bits, true, true));
        while (p < pe) {
            int r=is_prime(p, callback);
            if (r < 0) return integer::zero();
            if (r == 1) return p;
            p+=2;
        }
    }
}

} // namespace xint
} // namespace boost
