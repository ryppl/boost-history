// xint_test.cpp

// Copyright Paul A. Bristow 2010
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file     
\brief Test xint stream output.
\details
Tests for output to a std:: stream.
**/

#include <boost/xint/xint.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <sstream>
#include <limits>

//! Compare stream output with expected.
#define BOOST_XINT_TEST_OUTPUT(v, expected) \
{                                           \
    std::ostringstream ss;                  \
    ss FORMATTERS << v;                     \
    BOOST_CHECK_EQUAL(ss.str(), expected);  \
}

BOOST_AUTO_TEST_CASE(test_output_1)
{  // No formatting
#define FORMATTERS
  xint::integer my_int2(2);
  BOOST_XINT_TEST_OUTPUT(my_int2, "2"); // 
#undef FORMATTERS
}

//BOOST_AUTO_TEST_CASE(test_output_2)
//{  // Formatting
//#define FORMATTERS << std::showpos
//  xint::integer my_int2(-2);
//  BOOST_XINT_TEST_OUTPUT(my_int2, "-2");// 
//#undef FORMATTERS
//}


//BOOST_AUTO_TEST_CASE(test_output_2)
//{  // Formatting
//#define FORMATTERS << std::showpos
//  xint::integer my_int2(2);
//  BOOST_XINT_TEST_OUTPUT(my_int2, "+2");// 
//#undef FORMATTERS
//}


/*

Output

Autorun "j:\Cpp\Misc\debug\xint_test.exe"
Running 2 test cases...
./xint_test.cpp(45): error in "test_output_2": check ss.str() == "+2" failed [2 != +2]
*** 1 failure detected in test suite "Master Test Suite"
Detected memory leaks!
Dumping objects ->
{397} normal block at 0x00232038, 52 bytes long.
 Data: <        D #     > 01 00 00 00 03 00 00 00 44 20 23 00 00 00 00 00 
{396} normal block at 0x00021FA0, 4 bytes long.
 Data: <8 # > 38 20 23 00 
Object dump complete.
Project : error PRJ0019: A tool returned an error code from "Autorun "j:\Cpp\Misc\debug\xint_test.exe""
Build Time 0:03
*/
