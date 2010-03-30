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

#include <boost/xint/xint.hpp>

//#include <iostream>
//using std::cout;
//using std::endl;
//#include <iomanip>
//using std::setprecision;
//using std::setw;
//#include <limits>

int main()
{    
  // TODO I think you should get this into the boost namespace now or there will be zillions of places to change.
  //using namespace boost::xint

  //using namespace xint;
  //using xint::integer;

//[xint_simple_snippet_1

  xint::integer a;
  //integer b = 3;

  //integer c = a + b;

  //cout << a << ", b = " << b << ", a + b = " << a + b << endl;

//] [xint_simple_snippet_1]
      return 0;
} // int(main)

/*


//[xint_simple_output
Output:


//] [xint_simple_output]
*/



