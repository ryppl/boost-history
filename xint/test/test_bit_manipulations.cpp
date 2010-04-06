
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the functions that test the low-level bit manipulations.
*/

#include <boost/xint/xint.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>

namespace boost {
namespace xint {

using std::hex;

BOOST_AUTO_TEST_CASE(testBitManipulations) {
    const std::string nSrc="1a2b3c4d5e6f7f6e5d4c3b2a1a2b3c4d5e6f7f6e5d4c3b2a1";
    integer n(nSrc, 16), n2;

    // If this fails, abort the rest of the test.
    if (to_string(n, 16) != nSrc) BOOST_FAIL("failure in reading string, can't test");

    // Bits only

    n2 = n << 4;
    if (to_string(n2, 16) != nSrc+'0') {
        std::ostringstream out;
        out << "testBitManipulations: << 4 failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    n2 >>= 4;
    if (n2 != n) {
        std::ostringstream out;
        out << "testBitManipulations: >>= 4 failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    // Digits only

    const int cDigitsPerDigit=(detail::bits_per_digit / 4);
    n2 <<= detail::bits_per_digit;
    if (to_string(n2, 16) != nSrc + std::string(cDigitsPerDigit, '0')) {
        std::ostringstream out;
        out << "testBitManipulations: <<= BpD failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    n2 >>= detail::bits_per_digit;
    if (n2 != n) {
        std::ostringstream out;
        out << "testBitManipulations: >>= BpD failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    // Bits and digits simultaaneously

    const int bits_per_digit2=(detail::bits_per_digit + detail::bits_per_digit/2);
    const int cDigitsPerDigit2=(bits_per_digit2 / 4);
    n2 <<= bits_per_digit2;
    if (to_string(n2, 16) != nSrc + std::string(cDigitsPerDigit2, '0')) {
        std::ostringstream out;
        out << "testBitManipulations: <<= BpD2 failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    n2 >>= bits_per_digit2;
    if (n2 != n) {
        std::ostringstream out;
        out << "testBitManipulations: >>= BpD2 failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    // Right-shift to oblivion

    n2 >>= (detail::bits_per_digit * n2._get_length());
    if (n2.sign()!=0) {
        std::ostringstream out;
        out << "testBitManipulations: >>= inf failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    // Single-digit by bits

    n=n2=0xEF;
    n2 <<= 4;
    if (to_string(n2, 16) != "ef0") {
        std::ostringstream out;
        out << "testBitManipulations: single <<= 4 failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }

    n2 >>= 4;
    if (n2 != n) {
        std::ostringstream out;
        out << "testBitManipulations: single >>= 4 failure, " << hex << n2;
        BOOST_ERROR(out.str());
    }
}

} // namespace xint
} // namespace boost
