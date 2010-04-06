
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the tests for the addition and subtraction functions.
*/

#include <boost/xint/xint.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

namespace xint {

using std::endl;
using std::dec;
using std::hex;

namespace {

void _test(int section, int test, integer n, integer m) {
    integer answer(m+n);
    if (answer-m != n || answer-n != m) {
        std::ostringstream out;
        out << "testAddSubtract, section " << section << ", " << test << endl;

        out << "n     = " << n;
        if (section==0) out << " (" << hex << n << dec << ')';
        out << endl;

        out << "m     = " << m;
        if (section==0) out << " (" << hex << m << dec << ')';
        out << endl;

        out << "a     = " << answer;
        if (section==0) out << " (" << hex << answer << dec << ')';
        out << endl << endl;

        out << "a - m = " << (answer-m)
            << (answer-m == n ? " (good)" : " (should be n, isn't)")
            << endl;
        out << "a - n = " << (answer-n)
            << (answer-n == m ? " (good)" : " (should be m, isn't)")
            << endl;
        BOOST_FAIL(out.str());
    }
}

} // namespace

BOOST_AUTO_TEST_CASE(testAddSubtract) {
    {
        integer n("-3000000080000000600000000", 16),
            m("8000000090000000600000000", 16);
        _test(0, 0, n, m);
    }

    for (int i=0; i<10000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*4)),
            m(random_by_size(detail::bits_per_digit*4));
        _test(1, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*4, false, false, true)),
            m(random_by_size(detail::bits_per_digit*4, false, false, true));
        _test(2, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*4, false, false, true)),
            m(random_by_size(detail::bits_per_digit*3, false, false, true));
        _test(3, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*3, false, false, true)),
            m(random_by_size(detail::bits_per_digit*4, false, false, true));
        _test(4, i, n, m);
    }
}

} // namespace xint
