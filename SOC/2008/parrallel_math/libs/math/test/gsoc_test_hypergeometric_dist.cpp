

// Copyright John Maddock 
// Copyright Paul A. Bristow 
// Copyright Gautam Sewani

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


/*
Currently checks only the PDF function of
the hypergeometric distribution.However, the ipp 
file it uses as test data source contains data
for testing all accessors. 
*/

//#define BOOST_MATH_UNDERFLOW_ERROR_POLICY throw_on_error
#define BOOST_MATH_OVERFLOW_ERROR_POLICY throw_on_error
#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/math/distributions/hypergeometric_pdf.hpp>


#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>


#include <iostream>
   using std::cout;
   using std::endl;
   using std::setprecision;





template <class RealType>
void test_spots(RealType T)
{
   // Basic sanity checks.
   // 50 eps as a percentage, up to a maximum of double precision
   // Test data taken from Mathematica 6
   RealType tolerance = (std::max)(
      static_cast<RealType>(1e-33L),
      boost::math::tools::epsilon<RealType>());
   cout<<"Absolute tolerance:"<<tolerance<<endl;
   
   tolerance *= 50 * 100; 
   // #  pragma warning(disable: 4100) // unreferenced formal parameter.
   // prevent his spurious warning.
   if (T != 0)
   {
     cout << "Expect parameter T == 0!" << endl;
   }
   cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

#define T RealType
#include "hypergeometric_test_data.ipp"

   for(unsigned i=0;i<hypergeometric_test_data.size();++i) {
     BOOST_CHECK_CLOSE(
       boost::math::hypergeometric_pdf(
         hypergeometric_test_data[i][3],
         hypergeometric_test_data[i][0],
         hypergeometric_test_data[i][1],
         hypergeometric_test_data[i][2]      
         ),
         hypergeometric_test_data[i][4],
         tolerance);
              
   }
    
}


int test_main(int, char* [])
{
  

    // Basic sanity-check spot values.
   // (Parameter value, arbitrarily zero, only communicates the floating point type).
  test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
  test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
  test_spots(0.0L); // Test long double.

#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif

   return 0;
} // int test_main(int, char* [])

