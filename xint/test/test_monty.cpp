
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the test functions for the Montgomery stuff.
*/

#include <boost/xint/xint.hpp>
#include <boost/xint/monty.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <boost/random/mersenne_twister.hpp>

namespace boost {
namespace xint {

using std::endl;

namespace {

void _test1(int section, int test, integer x, integer y, integer m) {
    integer xx=toMontgomeryForm(x, m), yy=toMontgomeryForm(y, m);
    integer mResult=fromMontgomeryForm(montgomeryMultiplyMod(xx, yy, m,
        inverse0(m)), m);
    integer cResult=mod(x * y, m);

    if (mResult != cResult) {
        std::ostringstream out;
        out << "testMontyMultiply, section " << section << ", " << test << endl;
        out << "x        = " << to_string(x, 16) << endl;
        out << "y        = " << to_string(y, 16) << endl;
        out << "m        = " << to_string(m, 16) << endl;
        out << "answer   = " << to_string(mResult, 16) << endl;
        out << "expected = " << to_string(cResult, 16) << endl << endl;
        BOOST_ERROR(out.str());
    }
}

void _test2(int section, int test, integer b, integer r, integer modulus) {
    integer mResult=montgomeryPowerMod(b%modulus, r, modulus);
    integer cResult=powmod(b%modulus, r, modulus, true);
    if (mResult != cResult) {
        std::ostringstream out;
        out << "testMontyPowerMod, section " << section << ", " << test << endl;
        out << "b        = " << to_string(b, 16) << endl;
        out << "r        = " << to_string(r, 16) << endl;
        out << "m        = " << to_string(modulus, 16) << endl;
        out << "answer   = " << to_string(mResult, 16) << endl;
        out << "expected = " << to_string(cResult, 16) << endl << endl;
        BOOST_ERROR(out.str());
    }
}

size_t random(size_t low, size_t high) {
    unsigned long r=detail::get_random();
    size_t range=high-low+1;
    return size_t(low + (r % range));
}

size_t random1ToDigitLength() {
    return random(1, detail::bits_per_digit);
}

} // namespace

BOOST_AUTO_TEST_CASE(testMontyMultiply) {
    // We always want these to run on the same data, so we can reproduce it if
    // there are problems.
    set_random_generator(new boost::mt19937(42u));

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false, true));
        integer m(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false, true));
        integer modulus(random_by_size(detail::bits_per_digit*4 + random1ToDigitLength(), true, true, false));
        _test1(1, i, n, m, modulus);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false, true));
        integer m(random_by_size(detail::bits_per_digit*2 + random1ToDigitLength(), false, false, true));
        integer modulus(random_by_size(detail::bits_per_digit*4 + random1ToDigitLength(), true, true, false));
        _test1(2, i, n, m, modulus);
    }

    for (int i=0; i<1000; ++i) {
        integer n(random_by_size(detail::bits_per_digit*2 + random1ToDigitLength(), false, false, true));
        integer m(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false, true));
        integer modulus(random_by_size(detail::bits_per_digit*4 + random1ToDigitLength(), true, true, false));
        _test1(3, i, n, m, modulus);
    }
}

BOOST_AUTO_TEST_CASE(testMontyPowerMod) {
    // We always want these to run on the same data, so we can reproduce it if
    // there are problems.
    set_random_generator(new boost::mt19937(42u));

    {
        integer modulus(random_by_size(detail::bits_per_digit * 5 + random1ToDigitLength(), true, true));
        for (int i=2; i<100; ++i)
            _test2(0, i, integer("abcd1234", 16), i, modulus);
    }

    for (int i=0; i<100; ++i) {
        integer n(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false));
        integer m(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false));
        integer modulus(random_by_size(detail::bits_per_digit * 2 + random(size_t(1), 3*detail::bits_per_digit), true, true));
        _test2(1, i, n, m, modulus);
    }

    for (int i=0; i<100; ++i) {
        integer n(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false));
        integer m(random_by_size(detail::bits_per_digit*2 + random1ToDigitLength(), false, false));
        integer modulus(random_by_size(detail::bits_per_digit * 4 + random1ToDigitLength(), true, true));
        _test2(2, i, n, m, modulus);
    }

    for (int i=0; i<100; ++i) {
        integer n(random_by_size(detail::bits_per_digit*2 + random1ToDigitLength(), false, false));
        integer m(random_by_size(detail::bits_per_digit*3 + random1ToDigitLength(), false, false));
        integer modulus(random_by_size(detail::bits_per_digit * 4 + random1ToDigitLength(), true, true));
        _test2(3, i, n, m, modulus);
    }
}

} // namespace xint
} // namespace boost
