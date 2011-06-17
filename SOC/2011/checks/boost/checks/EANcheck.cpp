//! \file EANcheck.cpp

// Copyright Paul A. Bristow 2011.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// European Article numbering

// EAN check

// EAN Symbol Specification Manual,
// Universal Product Code, Uniform Code Council, Dayton, Ohio, USA.
// M. Blocksma, Reading the Numbers, Penguin, New York 1989.
// Joseph A Gallian, ACM Computing Surveys, 28(3), 504-517 (Sep 1996)
// Section 2, page 505 example of Kelloggs Low fat Granola 03800013710 & check digit 5.
// EAN-13 check is (
// Example is 7 012345 678908
// EAN-8 check is -(1, 2, 3, 4, 5, 6, 7) * (3,1,3,1,3,1,3,1,3) mod 10 == 0
// eg -(1 * 3 + 2 * 1 * 3 * 3 + 4 * 1 + %* 3 + 6 * 1 + 7 * 3) mod 10
//
// Stephen Barnett, Discrete Mathematics, Numbers and Beyond, Addison-Wesley.
// ISBN 0201342928  EAN 978 201 342925
// EAN for books is based on the ISBN and begins with the digits 978,
// and the next nine digits being the first nine digits of the ISBN,
// finally an EAN-13 check decimal digit.

#include <iostream> // For testing only.
//using std::cout; // For testing only.
//using std::endl; // For testing only.

#include <string>
//using std::string;
#include <vector>
//using std::vector;

#include <cassert>

#include "EANcheck.hpp" // Declarations of bool EANcheck(string s); & char EANcompute(string s);

bool EANcheck(std::string s)
{ // Check that a EAN 13 includes the correct check end digit.
  if (s.size() != 13)
  { // Must be exactly 12 decimal digits + one decimal check digit.
    std::cout << "length is " << unsigned int(s.size()) << std::endl;
    return false;
  }
  std::vector<int> v(13); // Exactly 13 requirement.
  assert(v.size() == 13);
  for (int i = 0; i < 13; i++)
  {
    if (isdigit(s[i]))
    {
      v[i] = s[i] - '0';
    }
    else
    { // Unexpected character in string!
      std::cout << " unexpected char " << s[i] << std::endl;
      return false;
    }
  }
  int check = (v[0] * 1 + v[1] * 3 + v[2] * 1 + v[3] * 3 + v[4] * 1 + v[5] * 3
    + v[6] * 1 + v[7] * 3 + v[8] * 1 + v[9] * 3 + v[10] * 1 + v[11] * 3
    + v[12] * 1); // the 13th decimal check digit.
  //std::cout << check << std::endl;
  check %= 10; // modulus 10 because must be a decimal check digit,
  // (not X as ISBN system).
  //std::cout << check << std::endl;
  return (check == 0);
} // bool EANcheck(string s)

char EANcompute(std::string s)
{ // Compute EAN check digit.
  if (s.size() != 12)
  { // Must be exactly 12 decimal digits.
    std::cout << "EAN length is " << unsigned int(s.size()) << std::endl;
    return false;
  }
  std::vector<int> v(13); // Exactly 12 digits required,
  // but leave space for check digit too?
  assert(v.size() == 13);

  for (int i = 0; i < 12; i++)
  {
    if (isdigit(s[i]))
    { // Convert ACSII digit chars to int.
      v[i] = s[i] - '0';
    }
    else
    { // Unexpected character in string!
      std::cout << " unexpected char " << s[i] << std::endl;
      return false;
    }
  }
  int check = (v[0] * 1 + v[1] * 3 + v[2] * 1 + v[3] * 3 + v[4] * 1 + v[5] * 3
    + v[6] * 1 + v[7] * 3 + v[8] * 1 + v[9] * 3 + v[10] * 1 + v[11] * 3
  ); // decimal check digit.

  //cout << check << endl;
  check %= 10; // modulus 10 so can be a decimal digit.
  check = (check != 0) ? 10 - check : 0;
  //std::cout << check << std::endl;
  return char(check  + '0'); // ASCII '0' to '9'
} // bool EANcompute(string s)

bool EAN8check(std::string s)
{ // Check that a EAN-8 string includes the correct check digit.
  // Examples:  EAN8check("12345670")
 //   EAN8check("00511728")
  if (s.size() != 8)
  { // Must be exactly 7 decimal digits & one decimal check digit.
    std::cout << "length is " << unsigned int(s.size()) << "!" << std::endl;
    return false;
  }
  std::vector<int> v(8); // Exactly 8 decimal digits required.
    assert(v.size() == 8);
  for (int i = 0; i < 8; i++)
  {
    if (isdigit(s[i]))
    { // Convert ascii digit chars to int.
      v[i] = s[i] - '0'; // Convert ascii digit chars to int.
    }
    else
    { // Unexpected character in string!
      std::cout << " unexpected char " << s[i] << std::endl;
      return false;
    }
  }
  int check = (v[0] * 3 + v[1] * 1 + v[2] * 3 + v[3] * 1 + v[4] * 3 + v[5] * 1 + v[6] * 3
    + v[7] * 1  // the 8th decimal check digit.
    );
  //std::cout << check << std::endl;
  check %= 10; // modulus 10 because must be a decimal check digit (not X as ISBN system).
  //std::cout << check << std::endl;
  return (check == 0);
} // bool EANcheck(string s)

char EAN8compute(std::string s)
{ // Compute EAN-8 check digit.
  if (s.size() != 7)
  { // Must be exactly 7 decimal digits, because 8th will be the check digit.
    std::cout << "EAN length is " << unsigned int(s.size()) << std::endl;
    return false;
  }
  std::vector<int> v(7);// Exactly 7 digits required,
  assert(v.size() == 7);
  for (int i = 0; i < 7; i++)
  {
    if (isdigit(s[i]))
    {
      v[i] = s[i] - '0';
    }
    else
    { // Unexpected character in string!
      std::cout << " unexpected char " << s[i] << std::endl;
      return false;
    }
  }
  int check = - (
    v[0] * 3 + v[1] * 1 + v[2] * 3 + v[3] * 1 + v[4] * 3 + v[5] * 1 + v[6] * 3
    ); // decimal check digit.

  //std::cout << check << std::endl;
  check %= 10; // modulus 10 as must be be a decimal digit (not X as ISBN system).
  //std::cout << check << std::endl;
  return char(check  + '0'); // ASCII '0' to '9'
} // bool EANcompute(string s)

// and bar code 2 in 5...
