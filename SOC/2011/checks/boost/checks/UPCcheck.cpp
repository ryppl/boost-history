//! \file UPCcheck.cpp

// Copyright Paul A. Bristow 2011.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// UPC check

// Universal Product Code
// UPC Symbol Specification Manual, Uniform Code Council, Dayton, Ohio, USA.
// M. Blocksma, Reading the Numbers, Penguin, New York 1989.
// Joseph A Gallian, ACM Computing Surveys, 28(3), 504-517 (Sep 1996)
// Section 2, page 505 example of Kellogs Low fat Granola 03800013710 & check digit 5.

// Shipping code adds a second using modulo 103 arithmetic.
// USS-128 Uniform Symbology Specification, p5,
// Uniform Code Couuncil, Ohio, USA.  (1986)

#include <iostream> // For testing only.
using std::cout; // For testing only.
using std::endl; // For testing only.
#include <string>
// using std::string;
#include <vector>
//using std::vector;
#include <cassert>

#include <boost/checks/UPCcheck.hpp> // Declarations of bool UPCcheck(string s);
// & char UPCcompute(string s);

bool UPCcheck(std::string s)
{ // Check that a UPC includes the correct check digit.
  if (s.size() != 12)
  { // Must be exactly 11 decimal digits + one decimal check digit.
    cout << "length is " << unsigned int(s.size()) << " ! but must be exactly 12."<< endl;
    return false;
  }
  std::vector<int> v (12);
  assert(v.size() == 12);
  for (int i = 0; i < 12; i++)
  {
    char d = s[i];
    if (isdigit(d))
    {
      int c = d - int('0');
      //v.push_back(c);
      v[i] = c;
    }
    else
    { // Unexpected character in string!
      cout << " Unexpected char " << s[i] << endl;
      return false;
    }
  }
  int check = (v[0] * 3 + v[1] * 1 + v[2] * 3 + v[3] * 1 + v[4] * 3 + v[5] * 1
    + v[6] * 3 + v[7] * 1 + v[8] * 3 + v[9] * 1 + v[10] * 3
    + v[11] * 1); // the 12th decimal check digit.
  // cout << check << endl;
  check %= 10; // modulus 10 because a decimal check digit.
  //cout << check << endl;
  return bool(check == 0);
} // bool UPCcheck(string s)

char UPCcompute(std::string s)
{ // Compute UPC check digit.
  if (s.size() != 11)
  { // Must be exactly 11 decimal digits.
    cout << "UPC length is " << unsigned int(s.size()) << "! but should be exactly 11." << endl;
    return false;
  }
  std::vector<int> v (12);
  assert(v.size() == 12); // Exactly 11 digits required,
  // but leave one space for check digit too.
  for (int i = 0; i < 10; i++)
  {
    if (isdigit(s[i]))
    {
      v[i] = s[i] - int('0');
    }
    else
    { // Unexpected character in string!
      cout << " unexpected char " << s[i] << endl;
      return false;
    }
  }
  int check = (v[0] * 3 + v[1] * 1 + v[2] * 3 + v[3] * 1 + v[4] * 3 + v[5] * 1
    + v[6] * 3 + v[7] * 1 + v[8] * 3 + v[9] * 1 + v[10] * 3);
  // cout <<"check int before mod " << check << endl;
  check %= 10; // modulus 10 so can be a decimal digit.
  check = (check != 0) ? 10 - check : 0;
  // cout << check << endl;
  return char(check  + '0'); // ASCII '0' to '9'
} // bool UPCcompute(string s)
