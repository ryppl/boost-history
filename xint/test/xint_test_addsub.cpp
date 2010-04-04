// xint_test_addsub.cpp

// Copyright Paul A. Bristow 2010
// Copyright Chad Nelson 2010

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file     
\brief Test xint add and subtract operators.
\details
Tests Test xint add and subtract operators.
Simple and longer tests using random numbers are provided.
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

BOOST_AUTO_TEST_CASE(test_xint_add)
{  //
  #define FORMATTERS
  xint::integer my_int1(2);
  xint::integer my_int2(2);
  BOOST_CHECK_EQUAL(my_int1 + my_int2, 4); // 
  BOOST_XINT_TEST_OUTPUT((my_int2 + my_int2), "4");// 
#undef FORMATTERS
}

/*

Output

------ Rebuild All started: Project: xint_test_addsub, Configuration: Debug Win32 ------
Deleting intermediate and output files for project 'xint_test_addsub', configuration 'Debug|Win32'
Compiling...
xint_test_addsub.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\Misc\debug\xint_test_addsub.exe"
Running 1 test case...
*** No errors detected
Detected memory leaks!
Dumping objects ->
{395} normal block at 0x007520F8, 52 bytes long.
 Data: <         !u     > 01 00 00 00 03 00 00 00 04 21 75 00 00 00 00 00 
{394} normal block at 0x00721FD0, 4 bytes long.
 Data: <  u > F8 20 75 00 
Object dump complete.
Build Time 0:05
Build log was saved at "file://j:\Cpp\Misc\xint_test_addsub\Debug\BuildLog.htm"
xint_test_addsub - 0 error(s), 0 warning(s)
========== Rebuild All: 1 succeeded, 0 failed, 0 skipped ==========

*/
