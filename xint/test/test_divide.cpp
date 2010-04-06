
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the division testing functions. It also tests some other
    operations, such as multiplication, but only indirectly and not very
    thoroughly.
*/

#include <boost/xint/xint.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/random.hpp>

namespace boost {
namespace xint {

using std::endl;

namespace {

void _test(int section, int test, integer n, integer m) {
    integer answer, remainder;
    boost::tie(answer, remainder)=divide_r(n, m);

    integer check=answer*m+remainder;
    if (check != n || abs(remainder) >= abs(m)) {
        bool flagRemainder=(check == n);

        std::ostringstream out;
        out << "testDivideMod, section div-" << section << ", " << test << endl;
        out << "n         = " << to_string(n, 16) << endl;
        out << "m         = " << to_string(m, 16) << endl;
        out << "a         = " << to_string(answer, 16) << endl;
        out << "r         = " << to_string(remainder, 16)
            << (flagRemainder ? " (should be < m, isn't)" : "")
            << endl << endl;

        out << "a * m + r = " << to_string(check, 16)
            << (check != n ? " (should be n, isn't)" : " (ok)")
            << endl;
        BOOST_ERROR(out.str());
    }

    integer modAnswer=mod(n, m);
    bool modValid=(modAnswer == remainder || modAnswer == remainder+abs(m));
    if (modAnswer < 0 || modAnswer >= abs(m) || !modValid) {
        std::ostringstream out;
        out << "testDivideMod, section mod-" << section << ", " << test << endl;
        out << "n         = " << to_string(n, 16) << endl;
        out << "m         = " << to_string(m, 16) << endl;
        out << "a         = " << to_string(modAnswer, 16) << endl;

        out << "expected  = " << to_string(remainder, 16) << endl;
        out << "or        = " << to_string(remainder+abs(m), 16) << endl;
        BOOST_ERROR(out.str());
    }
}

} // namespace

BOOST_AUTO_TEST_CASE(testDivide) {
    set_random_generator(new boost::mt19937(42u));

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
