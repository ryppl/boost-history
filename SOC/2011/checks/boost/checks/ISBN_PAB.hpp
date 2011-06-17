// ISBN_PAB.hpp  obselete version

//! \file
//! \brief Obselete versio of ISBN check and compute.

//  Boost checks/isbn_pab.hpp header file  ------------------------------------//

//  (C) Copyright Paul A. Bristow, 2002, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt

// The Standard Book Numbering Agency Ltd, London.
// ISBN.org
// Philip M. Tuchinsky, International Standard Book Numbers,
// UMAP Journal 5(1985) 41-45, ISSN 0197-3622
// ISO Standard International Standard Book Number 2108:1992.
// http://www.isbn.org/standards/home/isbn/digitalworld.asp
// Length of the ISBN String/Check Digit

// In January 2005, the ISBN standard will expand the current 10-digit string of numbers to 13.
// This action is being taken to increase the availability of new numbers
// and to make the ISBN standard compatible with European standard numbering systems EAN13.
// All ISBNs in the original pool of numbers will be prefaced
// by the prefix "978" to accomplish this increase in digits.
// All new ISBNs will be given a prefix "979",
// allowing the agency to effectively "reuse" the current pool of ISBNs.


//#include <iostream> // test only.
//using std::cout;
//using std::endl;

#include <string>
//using std::string;

#include <vector>
//using std::vector;

#include <cctype>
// using std::isdigit and std::tolower

// Defines:
bool ISBNcheck(string s);
char ISBNcompute(string s);


bool ISBNcheck(std::string s)
{ // Check an existing ISBN string s
  // which includes a check digit 0 to 9 or X (=11)
  std::vector<int> v(10);
  for (int i = 0; i < 10; i++)
  {
    if (std::isdigit(s[i]))
    {
      v[i] = s[i] - '0';
    }
    else if (toupper(s[i] == 'X'))
    {
      v[i] = 10;
    }
    else
    { // give up! (assumes no whitespace or punctuation).
      return false;
    }
    // cout << v[i] << ' ';
  }
  int check = 10 * v[0] + 9 * v[1] + 8 * v[2] + 7 * v[3] + 6 * v[4]
  + 5 * v[5] + 4 * v[6] + 3 * v[7] + 2 * v[8]
  + 1 * v[9]; // check digit
  // std::cout << ' ' << check << std::endl;
  check %= 11; // modulus 11
  // std::cout << ' ' << check << std::endl;
  return (check == 0); // true if check digit is correct.
} // bool ISBNcheck(std::string s)

char ISBNcompute(std::string s)
{ // Compute check digit 0 to 9 or X (=11) for the string provided.
  if (s.size() != 9)
  {
    return -1;  // Must be exactly 9 digits (to be a 10 digit ISBN)
  }
  // Might allow other size if remove whitespace and punctutation?
  std::vector<int> v(9);
  for (int i = 0; i < 9; i++)
  {
    if (std::isdigit(s[i]))
    {
      v[i] = s[i] - '0';
    }
    else if (std::toupper(s[i] == 'X'))
    {
      v[i] = 10;
    }
    else
    { // throw?
      return '?';
    }
    // cout << v[i] << ' ';
  }
  int check = 10 * v[0] + 9 * v[1] + 8 * v[2] + 7 * v[3] + 6 * v[4]
    + 5 * v[5] + 4 * v[6] + 3 * v[7] + 2 * v[8];

  // std::cout << '\n' << check << std::endl;

  check %= 11; // modulus 11
  // std::cout << ' ' << check << std::endl;
  if (check == 0) return '0';
  check = 11 - check;
  return (check == 10) ? 'X' : char(check + '0');
} // char ISBNcomputer(std::string s)

