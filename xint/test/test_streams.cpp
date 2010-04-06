
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the stream testing functions.
*/

#include <boost/xint/xint.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <sstream>

namespace xint {

namespace {

bool testStreamOut(std::string name, std::stringstream& str, const integer& n, std::string expected) {
    str.str(std::string());
    str << n;
    if (str.str() != expected) {
        BOOST_ERROR("testStreamOut "+name+": "+str.str());
        return false;
    } else return true;
}

bool testStreamIn(std::string name, std::stringstream& str, std::string value, const integer& expected) {
    integer test;

    str.clear(); // Reset any error states on the stream
    str.str(value);
    str >> test;
    if (test != expected) {
        std::ostringstream out;
        out << "testStreamIn " << name << ": ";
        if (!str) out << "fail-bit on";
        else out << test;
        BOOST_ERROR(out.str());
        return false;
    } else return true;
}

} // namespace

BOOST_AUTO_TEST_CASE(testStreams) {
    // Test streaming stuff
    const std::string nStreamSrc8="642547423257157";
    const std::string nStreamSrc10="28772997619311";
    const std::string nStreamSrc16="1a2b3c4d5e6f";
    const std::string nStreamSrc16u="1A2B3C4D5E6F";
    const std::string nStreamSrcL8="0642547423257157";
    const std::string nStreamSrcL10="28772997619311";
    const std::string nStreamSrcL16="0x1a2b3c4d5e6f";
    const std::string nStreamSrcL16u="0X1A2B3C4D5E6F";
    const std::string nStreamSrcLNeg16="-0x1a2b3c4d5e6f";
    integer n(nStreamSrc16u, 16);

    std::stringstream str;
    std::ios::fmtflags defaultFlags=str.flags();
    testStreamOut("10", str, n, nStreamSrc10);

    str.flags(defaultFlags);
    str << std::hex;
    testStreamOut("16", str, n, nStreamSrc16);

    str.flags(defaultFlags);
    str << std::hex << std::uppercase;
    testStreamOut("16u", str, n, nStreamSrc16u);

    str.flags(defaultFlags);
    str << std::oct;
    testStreamOut("8", str, n, nStreamSrc8);

    str.flags(defaultFlags);
    str << std::showbase;
    testStreamOut("L10", str, n, nStreamSrcL10);

    str.flags(defaultFlags);
    str << std::showbase << std::hex;
    testStreamOut("L16", str, n, nStreamSrcL16);

    str.flags(defaultFlags);
    str << std::showbase << std::hex << std::uppercase;
    testStreamOut("L16u", str, n, nStreamSrcL16u);

    str.flags(defaultFlags);
    str << std::showbase << std::oct;
    testStreamOut("L8", str, n, nStreamSrcL8);

    str.flags(defaultFlags);
    str << std::hex << std::showbase;
    testStreamOut("-L16", str, -n, nStreamSrcLNeg16);

    // Now input streams

    str.flags(defaultFlags);
    testStreamIn("10", str, nStreamSrc10, n);

    str.flags(defaultFlags);
    str << std::oct;
    testStreamIn("8", str, nStreamSrc8, n);

    str.flags(defaultFlags);
    str << std::hex;
    testStreamIn("16", str, nStreamSrc16, n);

    // Auto-base

    std::ios::fmtflags autoBase=(defaultFlags & ~std::ios::dec);
    str.flags(defaultFlags);
    str.setf(autoBase, std::ios::basefield); // Sets auto-base
    testStreamIn("auto-10", str, nStreamSrcL10, n);

    str.flags(defaultFlags);
    str.setf(autoBase, std::ios::basefield); // Sets auto-base
    testStreamIn("auto-8", str, nStreamSrcL8, n);

    str.flags(defaultFlags);
    str.setf(autoBase, std::ios::basefield); // Sets auto-base
    testStreamIn("auto-16", str, nStreamSrcL16, n);

    str.flags(defaultFlags);
    str.setf(autoBase, std::ios::basefield); // Sets auto-base
    testStreamIn("auto-16u", str, nStreamSrcL16u, n);

    str.flags(defaultFlags);
    str.setf(autoBase, std::ios::basefield); // Sets auto-base
    testStreamIn("auto-16 negative", str, nStreamSrcLNeg16, -n);

    // Check error-handling

    try {
        integer test(123);

        str.flags(defaultFlags);
        str.clear();
        str << std::oct;
        str.str("8192");
        str >> test;
        BOOST_CHECK_EQUAL(test, 123); // Shouldn't have touched it
        if (!!str) BOOST_ERROR("stream error signal wasn't set");
    } catch (std::exception& e) {
        std::string err=std::string("testStreams: erroneous input stream exception '")+e.what()+"'";
        BOOST_ERROR(err);
    }

    try {
        str.flags(defaultFlags);
        str.clear();

        str << integer(not_a_number());
        std::string temp=str.str();

        integer test;
        str.str(temp);
        str >> test;
        if (!test.is_nan()) BOOST_ERROR("testStream: isNaN test failed");
        if (!str) BOOST_ERROR("testStream: isNaN set stream error signal, shouldn't have");
    } catch (std::exception& e) {
        std::string err=std::string("testStreams: erroneous input stream exception '")+e.what()+"'";
        BOOST_ERROR(err);
    }
}

} // namespace xint
