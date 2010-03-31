
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This header file declares the library's self-testing functions.
*/

#ifndef BOOST_INCLUDED_XINT_TEST_H
#define BOOST_INCLUDED_XINT_TEST_H

namespace xint {

bool testAll();
bool testBitManipulations();
bool testAddSubtract();
bool testMultiply();
bool testDivideMod();
bool testConvert();
bool testStreams();
bool testMontyMultiply();
bool testMontyPowerMod();

} // namespace xint

#endif // BOOST_INCLUDED_XINT_TEST_H
