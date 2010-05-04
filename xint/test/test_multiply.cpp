
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the multiplication-testing function. It also tests
    division, but only indirectly, and not very thoroughly.
*/

#include <boost/xint/integer.hpp>
#include <boost/xint/random.hpp>
#include <boost/xint/fixed_integer.hpp>

#ifdef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_DYN_LINK
#else
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>

namespace boost {
namespace xint {

using std::endl;

namespace {

void _test(size_t section, size_t test, integer n, integer m) {
    //BOOST_MESSAGE("testMultiply " << section << ", " << test);

    integer result(m*n);
    if (m.sign()==0 || n.sign()==0) BOOST_CHECK_EQUAL(result, 0);
    else {
        if (result/m != n || result/n != m) {
            std::ostringstream out;
            out << "testMultiply, section " << section << ", " << test << endl;
            out << "n     = " << to_string(n, 16) << endl;
            out << "m     = " << to_string(m, 16) << endl;
            out << "r     = " << to_string(result, 16) << endl << endl;

            out << "r / m = " << to_string(result/m, 16)
                << (result/m == n ? " (good)" : " (should be n, isn't)")
                << endl;
            out << "r / n = " << to_string(result/n, 16)
                << (result/n == m ? " (good)" : " (should be m, isn't)")
                << endl;
            BOOST_ERROR(out.str());
        }
    }
}

} // namespace

BOOST_AUTO_TEST_CASE(testMultiply) {
    {
        // These caused a problem while trying to backport the new division
        // algorithm into an alternate version.
        integer n=integer("-41e0348d52a1be49b7da339137639cde", 16);
		integer m=integer("c3a1d54a63e80d5a8b851a01d244ea61", 16);
		integer expected=integer("-32576CEB63105D373FE83F8B3C8A0B68ADAD386F3B668C694D7ADCA8D1195C1E", 16);
        BOOST_CHECK_EQUAL(n*m, expected);

        // 'm' contains a full digit of zeros, which caused problems with an
        // earlier implementation.
        n=integer("300493435053701420844513300446198902283");
        m=integer("-10530911911644352242410713382920651977");
		_test(0, 1, n, m);
    }

    boost::mt19937 random(42u);

    for (size_t nsize = detail::bits_per_digit * 3; nsize <
        detail::bits_per_digit * 5; nsize += 4)
    {
        for (size_t msize = detail::bits_per_digit * 3; msize <
            detail::bits_per_digit * 5; msize += 4)
        {
            integer n(random_by_size(random, nsize, false, false, true)),
                m(random_by_size(random, msize, false, false, true));
            _test(nsize, msize, n, m);
        }
    }
}

BOOST_AUTO_TEST_CASE(test_fixed_multiply) {
    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef fixed_integer<bits> T; // Digit-and-a-half

    T n; n = ~n;
    T a = n * n;
    T check(integer(n) * integer(n));
    BOOST_CHECK_EQUAL(a, check);
}

BOOST_AUTO_TEST_CASE(test_multiply_by_sqr) {
    integer n(5);
    BOOST_CHECK_EQUAL(n * -n, -25);
}

} // namespace xint
} // namespace boost
