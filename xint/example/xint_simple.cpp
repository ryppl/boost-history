// xint_simple.cpp

// Copyright Paul A. Bristow 2010
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file     
\brief Simple demo of xint including stream output.
\details
Tests a few operators for output to a std:: stream.
**/

  // TODO I think you should get this into the boost namespace now or there will be zillions of places to change.
  //using namespace boost::xint

//[xint_simple_snippet_1
/*`First we need an include to use Boost.Xint
*/
#include <boost/xint/xint.hpp>

/*`It is also convenient to be able to write just 'integer',
rather than tediously typing xint::integer,
so in a main program, we can write simply
  using namespace xint;

but if the file is included by other programs, certain for .hpp files,
this should be placed a local scope to avoid enforcing this on other code.

Or you can explicitly declare which xint items you are using:
*/
  using xint::integer;
//] [/xint_simple_snippet_1]

#include <iostream>
using std::cout;
using std::endl;
using std::hex;
#include <iomanip>
using std::setprecision;
using std::setw;
#include <limits>

int main()
{    

//[xint_simple_snippet_2

/*`Some really unsurprising integer things we can do are:
*/
  using xint::integer;

  integer a; // Construct a xint::integer, zero by default.
  a++; // Try out an operator.
  integer b(3);  // Construct with a given value of 3.
  integer c = a + b; // Assign a sum.
  cout << "a = " << a << ", b = " << b << ", a + b = " << c << endl;
  // a = 1, b = 3, a + b = 4

/*`But we can also just as easily do thing that Plain Old Integers can't.
If we fill an integer using the max value for the type, and square it,
it must overflow, and the value we get (without any warning) is just wrong.
*/

  long k= (std::numeric_limits<long>::max)();
  cout << "k = " << k << ", k ^ 2 = " << k * k << endl; // Will overflow!
  //   k = 2147483647, k ^ 2 = 1


/*`But if we use the extended integer, it does what we would expect
(don't try this on your calculator either!)
*/
  integer xk =(std::numeric_limits<long>::max)();
  cout << "xk ^ 2 = "<< xk * xk << endl; // xk ^ 2 = 4611686014132420609

/*`In hex it may be clearer:*/
  cout << hex << "k = " << k << ", xk ^ 2 = "<< xk * xk << endl;

//] [xint_simple_snippet_2]
      return 0;
} // int(main)

/*


//[xint_simple_output
Output:

a = 1, b = 3, a + b = 4
k = 2147483647, k ^ 2 = 1
xk ^ 2 = 4611686014132420609
k = 7fffffff, xk ^ 2 = 3fffffff00000001


//] [xint_simple_output]
*/



