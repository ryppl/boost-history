//! \file
//! \warning This example is entirely contrived to show how to embed into Quickbook.
//! \brief A very simple example of checking an ISBN that is provided as a std::string.

// Copyright Paul A. Bristow 2011.

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

#include <boost/checks/checks_fwd.hpp> // Forward declarations.

//[checks_isbn_example_1
#include <boost/checks/ISBN_PAB.hpp> // ISBN for books (Old PAB version using just string parameter).
//] [/checks_isbn_example_1]

// This is a really, really trivial example, but shows how to imbed into Quickbook tutorial section.

// You need to [import isbn_example.cpp]
// [include checks_isbn_example_1] where you want the include info - probably wrong now!
// and then [include checks_isbn_example_2] where you want the program line
// and then [include checks_isbn_output_1] when you want the output

int main()
{

//[checks_isbn_example_2
  string s1 = "0201700735";

  cout << "ISBN " << s1 << (ISBNcheck(s1) ? " is OK" : " is Wrong!") << endl;

  string s2 = "020170073";

  cout << "Check digit of " << s2 << " is " << ISBNcompute("020170073") << endl;

//] [/checks_isbn_example_2]
  
} // int main()

/*

Output
//[checks_isbn_output_1

  ISBN 0201700735 is OK
  Check digit of 020170073 is 5

//] [/checks_isbn_output_1]


*/

