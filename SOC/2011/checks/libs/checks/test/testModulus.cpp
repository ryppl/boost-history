//! \file testModulus.cpp

// Copyright Pierre Talbot 2011.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Boost test of various check 'digit' systems.

// See http://www.boost.org/doc/libs/1_46_1/libs/test/doc/html/utf/testing-tools/reference.html

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "Modulus Check Test Suite"
#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-gd-1_47.lib  (trunk at 12 Jun 11)

#include <iostream>
#include <iomanip>
#include <cassert>

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,
#include <boost/checks/checks_fwd.hpp> // Forward declarations.
#include <boost/checks/modulus.hpp> // Modulo algorithms such the Luhn algorithm or the modulus 11 algorithm.

BOOST_AUTO_TEST_SUITE(modulus_remainders_test)

BOOST_AUTO_TEST_CASE(luhn_remainders_test)
{ 
  BOOST_TEST_MESSAGE( "Boost.Checks.Modulus.Luhn : tests on the remainders.");
  // Initialization of the numbers with a odd and even size of digit (10 digits for 10 remainders).
  std::string luhn_numbers_odd[] = 
  {    
    "102345600",
	"102345691",
    "102345642",
    "102345683",
    "102345634",
    "102345675",
    "102345626",
    "102345667",
    "102345618",
    "102345659"
  };
  std::string luhn_numbers_even[] = 
  {    
    "1234567830",
    "1234567871",
    "1234567822",
    "1234567863",
    "1234567814",
    "1234567855",
    "1234567806",
	"1234567897",
    "1234567848",
    "1234567889",
  };
  // Test the numbers with a different remainder and an odd and even size of digits.
  for(int i=0; i<10; ++i)
  {
	// Test the validation of the check digit with the luhn algorithm with the size precised.
    BOOST_CHECK( boost::checks::check_luhn ( luhn_numbers_even[i].begin(), luhn_numbers_even[i].end(), luhn_numbers_even[i].size()) );
    BOOST_CHECK( boost::checks::check_luhn ( luhn_numbers_odd[i].begin(),  luhn_numbers_odd[i].end(),  luhn_numbers_odd[i].size() ) );

	// Test the validation of the check digit with the luhn algorithm without the size precised.
	BOOST_CHECK( boost::checks::check_luhn ( luhn_numbers_even[i].begin(), luhn_numbers_even[i].end()) );
    BOOST_CHECK( boost::checks::check_luhn ( luhn_numbers_odd[i].begin(),  luhn_numbers_odd[i].end() ) );

	// Test the equality between the check digit computed and the last digit with the size precised.
	BOOST_CHECK_EQUAL( boost::checks::compute_luhn(luhn_numbers_even[i].begin(), luhn_numbers_even[i].end(), luhn_numbers_even[i].size()-1), *(--luhn_numbers_even[i].end()) );
    BOOST_CHECK_EQUAL( boost::checks::compute_luhn(luhn_numbers_odd[i].begin(),  luhn_numbers_odd[i].end(),  luhn_numbers_odd[i].size()-1),  *(--luhn_numbers_odd[i].end())  );
	
	// Test the equality between the check digit computed and the last digit without the size precised.
	BOOST_CHECK_EQUAL( boost::checks::compute_luhn(luhn_numbers_even[i].begin(), luhn_numbers_even[i].end()-1), *(--luhn_numbers_even[i].end()) );
    BOOST_CHECK_EQUAL( boost::checks::compute_luhn(luhn_numbers_odd[i].begin(),  luhn_numbers_odd[i].end() -1), *(--luhn_numbers_odd[i].end())  );
  }
} // BOOST_AUTO_TEST_CASE(luhn_remainders_test)
BOOST_AUTO_TEST_CASE(mod10_remainders_test)
{
  BOOST_TEST_MESSAGE( "Boost.Checks.Modulus.Modulus10 : tests on the remainders.");
}
BOOST_AUTO_TEST_CASE(mod11_remainders_test)
{
  BOOST_TEST_MESSAGE( "Boost.Checks.Modulus.Modulus11 : tests on the remainders.");
}
BOOST_AUTO_TEST_SUITE_END() //modulus_remainders_test

BOOST_AUTO_TEST_SUITE(modulus_alteration_test)

BOOST_AUTO_TEST_CASE(luhn_alteration_test)
{
  // Initialization of the numbers with a odd and even size of digit (10 digits for 10 remainders).
  std::string luhn_numbers_odd[] = 
  {    
    "102345600",
	"102345691",
    "102345642",
    "102345683",
    "102345634",
    "102345675",
    "102345626",
    "102345667",
    "102345618",
    "102345659"
  };
  std::string luhn_numbers_even[] = 
  {    
    "1234567830",
    "1234567871",
    "1234567822",
    "1234567863",
    "1234567814",
    "1234567855",
    "1234567806",
	"1234567897",
    "1234567848",
    "1234567889",
  };
  // Test the alteration of one digit
  for(int i=0; i<10; ++i)
  {
	// odd : test the alteration on each digits
    for(unsigned int j=0; j< luhn_numbers_odd[i].length(); ++j)
    {
      // alteration + [1|2|3|4|5|6|7|8|9] % 10
	  for(int k=1; k <= 9; ++k)
	  {
        std::string luhn_alterate = luhn_numbers_odd[i] ;
		luhn_alterate[j] = '0' + (luhn_alterate[j] + k) % 58  % 48 ;
		BOOST_CHECK_EQUAL ( boost::checks::check_luhn( luhn_alterate.begin(), luhn_alterate.end(), luhn_alterate.size() ), false );
		BOOST_CHECK_EQUAL ( boost::checks::check_luhn( luhn_alterate.begin(), luhn_alterate.end() ), false );
	  }
	}
	// even : test the alteration of each digits
	for(unsigned int j=0; j< luhn_numbers_even[i].length(); ++j)
    {
      // alteration + ([1|2|3|4|5|6|7|8|9] % 10)
	  for(int k=1; k <= 9; ++k)
	  {
        std::string luhn_alterate = luhn_numbers_even[i] ;
		luhn_alterate[j] = '0' + (luhn_alterate[j] + k) % 58 % 48 ;
		BOOST_CHECK_EQUAL ( boost::checks::check_luhn( luhn_alterate.begin(), luhn_alterate.end(), luhn_alterate.size() ), false );
		BOOST_CHECK_EQUAL ( boost::checks::check_luhn( luhn_alterate.begin(), luhn_alterate.end() ), false );
	  }
	}
  }
} //BOOST_AUTO_TEST_CASE(luhn_alteration_test)

BOOST_AUTO_TEST_SUITE_END() //modulus_alteration_test

BOOST_AUTO_TEST_SUITE(modulus_transposition_test)

// Transposition of 90 and 09 fail with odd and even tests.
BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES( luhn_transposition_test, 2 )
BOOST_AUTO_TEST_CASE(luhn_transposition_test)
{
  std::string luhn_transposition_odd = "12345678903"; //odd
  std::string luhn_transposition_even = "123456789007"; // even  
  std::string luhn_transposition;
  // for each digits
  for(int i=0; i < 9; ++i)
  {
    // Test a permutation with all the digits remaining
    for(int j=i+1; j < 10; j+=2)
    {
      // Odd
      luhn_transposition = luhn_transposition_odd;
      luhn_transposition[i] = luhn_transposition[j];
      luhn_transposition[j] = luhn_transposition_odd[i];
      std::cout << "Odd test : switch " << luhn_transposition[j] << " with " << luhn_transposition[i] << std::endl;
      BOOST_CHECK_EQUAL ( boost::checks::check_luhn( luhn_transposition.begin(), luhn_transposition.end(), luhn_transposition.size() ), false );
      // Even
      luhn_transposition = luhn_transposition_even;
      luhn_transposition[i] = luhn_transposition[j];
      luhn_transposition[j] = luhn_transposition_even[i];
      std::cout << "Even test : switch " << luhn_transposition[j] << " with " << luhn_transposition[i] << std::endl;
      BOOST_CHECK_EQUAL ( boost::checks::check_luhn( luhn_transposition.begin(), luhn_transposition.end(), luhn_transposition.size() ), false );
    }
  }
} // BOOST_AUTO_TEST_CASE(luhn_transposition_test)

BOOST_AUTO_TEST_SUITE_END() //modulus_transposition_test

BOOST_AUTO_TEST_CASE(modulus_encoding_test)
{ 
	// Modulus 11 tests on the different encoding of the characters.
	// Luhn tests on the different encoding of the characters.

} // BOOST_AUTO_TEST_CASE(modulus_encoding_test)

BOOST_AUTO_TEST_CASE(modulus_collection_test)
{ 
	// Test the validity of the algorithms with all the collections.
	// Show that forward iterator work too
} // BOOST_AUTO_TEST_CASE(modulus_collection_test)