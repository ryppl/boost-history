
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains tests for the modular math functions.
*/

#include <boost/xint/integer.hpp>

#ifdef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_DYN_LINK
#else
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

namespace boost {
namespace xint {

BOOST_AUTO_TEST_CASE(test_mod) {
    integer n(0x12345678);
    BOOST_CHECK_EQUAL(mod(n, 0x1000), 0x678);
}

BOOST_AUTO_TEST_CASE(test_invmod) {
    // When a number is multiplied modularly by its inverse, the result should
    // be one.
    integer n("2b6a501cd0b26e1dfd70918455621257ac2c697a", 16),
      modulus("47c45018f2f6e19dfbf4190c66d2bd306010261f", 16);
    integer inverse = invmod(n, modulus);
    BOOST_CHECK_EQUAL(mulmod(n, inverse, modulus), 1);

    inverse = invmod(-n, modulus);
    BOOST_CHECK_EQUAL(mulmod(-n, inverse, modulus), 1);
}

} // namespace xint
} // namespace boost
