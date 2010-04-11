
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

#include <boost/xint/xint.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

namespace boost {
namespace xint {

using namespace test;

using std::endl;

namespace {

void _test(int section, int test, integer n, integer m) {
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
        // algorithm into the version used elsewhere.
        integer n=integer("-41e0348d52a1be49b7da339137639cde", 16);
		integer m=integer("c3a1d54a63e80d5a8b851a01d244ea61", 16);
        _test(0, 0, n, m);

        // 'm' contains a full digit of zeros, which caused problems with an
        // earlier implementation.
        n=integer("300493435053701420844513300446198902283");
        m=integer("-10530911911644352242410713382920651977");
		_test(0, 1, n, m);
    }

    for (int i=0; i<10000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*4, false, false, true)),
            m(random_by_size(detail::bits_per_digit*4, false, false, true));
        _test(1, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*4, false, false, true)),
            m(random_by_size(detail::bits_per_digit*3, false, false, true));
        _test(2, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*3, false, false, true)),
            m(random_by_size(detail::bits_per_digit*4, false, false, true));
        _test(3, i, n, m);
    }
}

} // namespace xint
} // namespace boost
