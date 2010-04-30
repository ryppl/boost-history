
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
    \brief Contains tests that don't fit elsewhere.
*/

#include <boost/xint/integer.hpp>
#include <boost/xint/fixed_integer.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

namespace boost {
namespace xint {

BOOST_AUTO_TEST_CASE(test_log2) {
    // Check the detail::log2 template functions, unsigned and signed
    unsigned int u = (1 << (std::numeric_limits<unsigned int>::digits - 1));
    BOOST_CHECK_EQUAL(detail::log2(u), std::numeric_limits<unsigned int>::digits);
    int s = (1 << (std::numeric_limits<int>::digits - 1));
    BOOST_CHECK_EQUAL(detail::log2(s), std::numeric_limits<int>::digits);

    // Check the log2 function for the integer class
    integer n(0x87654321);
    n <<= detail::bits_per_digit;
    size_t expected = 32 + detail::bits_per_digit;
    while (expected != 0) {
        n >>= 1;
        --expected;
        BOOST_CHECK_EQUAL(log2(n), expected);
    }
}

BOOST_AUTO_TEST_CASE(test_gcd) {
    // These two numbers share no factors.
    integer n("47c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
            m("2b6a501cd0b26e1dfd70918455621257ac2c697a", 16);
    BOOST_CHECK_EQUAL(gcd(n, m), 1);
    BOOST_CHECK_EQUAL(gcd(n*127, m*127), 127);
}

BOOST_AUTO_TEST_CASE(test_lcm) {
    integer n(1307), m(1663), q(3571); // Arbitrarily-selected prime numbers
    BOOST_CHECK_EQUAL(lcm(n*q, m*q), n*m*q);
}

} // namespace xint
} // namespace boost
