//! \file test_checks.cpp

// Copyright Paul A. Bristow 2011.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Boost test of various check 'digit' systems.

// See http://www.boost.org/doc/libs/1_46_1/libs/test/doc/html/utf/testing-tools/reference.html

#define BOOST_TEST_MAIN
#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-gd-1_47.lib  (trunk at 12 Jun 11)

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::dec;
using std::hex;
using std::boolalpha;

#include <iomanip>
using std::setprecision;
using std::setw;
#include <string>
using std::string;
#include <cassert>

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,

#include <boost/checks/ISBN_PAB.hpp> // ISBN for books (Old PAB version using just string parameter).
#include <boost/checks/ISBN.hpp> // ISBN for books.
#include <boost/checks/ISSN_PAB.hpp> // ISSN for journals.
#include <boost/checks/IBMcheck.hpp> // IBM check system.
#include <boost/checks/VISAcheck.hpp> // Credit card check used by most credit or debit cards.
#include <boost/checks/UPCcheck.hpp> // Credit card check used by most credit or debit cards.

bool check(string); // Verhoeff/Gumm decimal check digit.
char compute(string);

bool ISBNcheck(string); // ISBN decimal or X check digit.
char ISBNcompute(string);

bool ISSNcheck(string s); // ISSN decimal or X check digit.
char ISSNcompute(string s);// International Standard Serial System ISO 3297:1998
// issn.org  1234-1231

bool IBMcheck(string s);
char IBMcompute(string s);

bool VISAcheck(string s);
char VISAcompute(string s);

//bool UPCcheck(string s); // in upc.cpp
//char UPCcompute(string s); // in upc.cpp

bool EANcheck(string s); // in EAN.cpp
char EANcompute(string s); // in EAN.cpp

bool EAN8check(string s); // in EAN.cpp
char EAN8compute(string s); // in EAN.cpp

// This test case is automatically registered by using BOOST_AUTO_TEST_CASE.
BOOST_AUTO_TEST_CASE(checks_test)
{ // Checks tests.  Template only!

  BOOST_TEST_MESSAGE("Test Boost.Checks."); // Only seems to go to a log file?
  BOOST_CHECK(true);
  //BOOST_CHECK(false); // always fails!
  BOOST_CHECK_EQUAL(1, 1);
  BOOST_CHECK_NE(1, -1);

} // BOOST_AUTO_TEST_CASE(checks_test)


BOOST_AUTO_TEST_CASE(PAB_ISBN_checks_test)
{ // ISBN Checks tests.

  BOOST_CHECK(ISBNcheck("0201700735"));  // B. Stroustrup, The C++ Programming Language.
  BOOST_CHECK_EQUAL(ISBNcompute("020170073"),'5');

  BOOST_CHECK(ISBNcheck("013152447X"));// Edward M. Reingold, Combinatorial Algorithms: Theory and Practice
  BOOST_CHECK_EQUAL(ISBNcompute("013152447"),'X');

  // Now check that a few 'errors' are detected using _NE rather than _EQUAL.
 // BOOST_CHECK_EQUAL(ISBNcompute("113152447"),'X'); // 1st digits wrong.
  //  test_checks.cpp(70): error in "ISBN_checks_test": check ISBNcompute("113152447") == 'X' failed ['0' != 'X']
  BOOST_CHECK_NE(ISBNcompute("113152447"),'X'); // 1st digit wrong.

  BOOST_CHECK_NE(ISBNcompute("103152447"),'X'); // 1st 2 digits transposed.

} // BOOST_AUTO_TEST_CASE(PAB_ISBN_checks_test)

BOOST_AUTO_TEST_CASE(ISBN_checks_test)
{ // ISBN Checks tests.

  string s = "0201700735";
  BOOST_CHECK_EQUAL(s.size(), static_cast<std::size_t>(10));

  BOOST_CHECK(boost::checks::Is_isbn10(s.begin(), s.end()));

  string s1 = "013152447";
  BOOST_CHECK_EQUAL(s1.size(), static_cast<std::size_t>(9));
  BOOST_CHECK_EQUAL(boost::checks::isbn10_check_digit(s1.begin(), s1.end()), 'X');

  // error in "ISBN_checks_test": check boost::checks::isbn10_check_digit(s1.begin(), s1.end()) == 'X' failed ['4' != 'X']
  // Doesn't seem to agree with my version???
  // But I could easily be wrong!

  // http://en.wikipedia.org/wiki/Isbn example
  string s3 = "0306406152";
  BOOST_CHECK(boost::checks::Is_isbn10(s3.begin(), s3.end()));

  string s2 = "030640615";
  BOOST_CHECK_EQUAL(boost::checks::isbn10_check_digit(s2.begin(), s2.end()), '2');
  // doesn't give the right check digit?

}


BOOST_AUTO_TEST_CASE(ISSN_checks_test)
{ // ISBN Checks tests.

  BOOST_CHECK(ISSNcheck("00189448"));   // IEEE transactions on Information Theory
  BOOST_CHECK_EQUAL(ISSNcompute("0018944"),'8');

  BOOST_CHECK(ISSNcheck("0003889X")); // Archiv der Mathematik
  BOOST_CHECK_EQUAL(ISSNcompute("0003889"),'X');

} // BOOST_AUTO_TEST_CASE(ISBN_checks_test)

/*

This works on MSVC but fails using bjam for reasons unclear.

Suggest rewriting as .hpp rather than splitting code to .cpp.

BOOST_AUTO_TEST_CASE(UPC_checks_test)
{ // UPC Checks tests.

  BOOST_MESSAGE("UPC Test checks"); //
  BOOST_CHECK(UPCcheck("038000137105"));
  BOOST_CHECK_EQUAL(UPCcompute("03800013710"),'5');

} // BOOST_AUTO_TEST_CASE(UPC_checks_test)

*/
/*

------ Build started: Project: test_checks, Configuration: Debug Win32 ------
  testChecks.cpp
  Linking to lib file: libboost_unit_test_framework-vc100-mt-gd-1_47.lib
  test_checks.vcxproj -> J:\Cpp\Checks\Debug\test_checks.exe
  Running 4 test cases...

  *** No errors detected
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========


  EANcheck.cpp
  radix32Check.cpp
  UPCcheck.cpp
  testChecks.cpp
  Linking to lib file: libboost_unit_test_framework-vc100-mt-1_47.lib
  Generating code
  Finished generating code
  test_checks.vcxproj -> J:\Cpp\Checks\Release\test_checks.exe
  Running 5 test cases...
  I:/boost-sandbox/SOC/2011/checks/libs/checks/test/testChecks.cpp(108): error in "ISBN_checks_test": check boost::checks::isbn10_check_digit(s1.begin(), s1.end()) == 'X' failed ['4' != 'X']

  *** 2 failures detected in test suite "Master Test Suite"
  I:/boost-sandbox/SOC/2011/checks/libs/checks/test/testChecks.cpp(119): error in "ISBN_checks_test": check boost::checks::isbn10_check_digit(s2.begin(), s2.end()) == '2' failed ['5' != '2']
C:\Program Files\MSBuild\Microsoft.Cpp\v4.0\Microsoft.CppCommon.targets(183,5): error MSB3073: The command ""J:\Cpp\Checks\Release\test_checks.exe
C:\Program Files\MSBuild\Microsoft.Cpp\v4.0\Microsoft.CppCommon.targets(183,5): error MSB3073: :VCEnd" exited with code 201.


*/