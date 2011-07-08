//! \file
//! \warning This example is entirely contrived to show how to embed into Quickbook.
//! \brief A very simple example of checking an ISBN that is provided as a std::string.

// Copyright Pierre Talbot 2011.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!


#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

#include <sstream>
#include <fstream> 

#include <iterator>

#include <boost/checks/checks_fwd.hpp> // Forward declarations.
#include <boost/checks/modulus.hpp> // Modulus algorithms such as MOD11 or Luhn.

// You need to [import modulus_example.cpp]
// [include checks_isbn_example_1] where you want the include info - probably wrong now!
// and then [include checks_isbn_example_2] where you want the program line
// and then [include checks_isbn_output_1] when you want the output

inline void example1()
{
  // Create a string with numbers
  std::string luhn_numbers = "";
  for(int i=9; i >= 0; --i)
  {
		std::ostringstream num_gen;
		num_gen << 1023456 << i ;
		std::string num = num_gen.str();
		char checkdigit = boost::checks::compute_luhn(num.begin(), num.end(), num.size());
		luhn_numbers = luhn_numbers + num + checkdigit ;
  }

  // Check all the numbers from this string
  std::string::iterator luhn_iter = luhn_numbers.begin();
  while(luhn_iter != luhn_numbers.end())
    if(boost::checks::check_luhn(luhn_iter, luhn_numbers.end(), 9))
      std::cout << "The number is valid." << std::endl;
    else
      std::cout << "The number is invalid." << std::endl;
}

template <typename Out, typename In>
inline Out f(In begin, In end)
{
	return *begin;
}

void test(unsigned int w[])
{
  if(w== NULL)
  {    unsigned int t[1]={1};
  
  w = t;}
}

int main()
{
  
  // Example 1
  example1();

  // Other tests / examples - Obsolete.
  std::string luhn_transposition = "12345678900"; // even  
  std::cout << "ok : "<< boost::checks::compute_luhn( luhn_transposition.begin(), luhn_transposition.end(), luhn_transposition.size() ) << std::endl;
  luhn_transposition = "173456289003"; // even  
  
  std::cout << "ko : " << boost::checks::check_luhn( luhn_transposition.begin(), luhn_transposition.end(), luhn_transposition.size() ) << std::endl;
  test(NULL);

  std::string luhn2 = "1234567814";
  std::wstring luhn1(luhn2.begin(), luhn2.end());
  std::wstring::iterator iter1 = luhn1.begin();
  wchar_t w = f<wchar_t>(luhn1.begin(), luhn1.end());
 // std::cout << w;

 std::string luhn_alterate = "1234567871";
 std::cout << boost::checks::check_luhn (luhn_alterate.begin(), luhn_alterate.end(), luhn_alterate.size()) << std::endl;
 std::cout << boost::checks::check_luhn (luhn_alterate.begin(), luhn_alterate.end()) << std::endl;
 luhn_alterate[0] += 9;
 std::cout << luhn_alterate<< std::endl;
 std::cout << boost::checks::check_luhn (luhn_alterate.begin(), luhn_alterate.end(), luhn_alterate.size()) << std::endl;
 std::cout << boost::checks::check_luhn (luhn_alterate.begin(), luhn_alterate.end()) << std::endl;

 std::string testt="12345678900";
 std::cout << boost::checks::compute_luhn(testt.begin(), testt.end(), 10) << std::endl;

 /*
  std::cout <<  boost::checks::check_luhn(luhn1.begin(), luhn1.end(), 10) << std::endl;

	  std::cout <<  boost::checks::compute_luhn<char>(luhn2.begin(), luhn2.end(), 9) << std::endl;
	  std::cout <<  boost::checks::compute_luhn<char>(luhn2.begin(), luhn2.end()) << std::endl;
	   std::cout <<  boost::checks::compute_luhn(luhn2.begin(), luhn2.end(), 9) << std::endl;*/
  return 0;

} // int main()

