// Copyright Paul Bristow 2007.
// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_constants.cpp

#include <pch.hpp>

#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/test/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>

#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/test.hpp> 

template <class RealType>
void test_spots(RealType)
{
   // Basic sanity checks for constants.

   RealType tolerance = boost::math::tools::epsilon<RealType>() * 2;  // double
   std::cout << "Tolerance for type " << typeid(RealType).name()  << " is " << tolerance << "." << std::endl;

   using namespace boost::math::constants;
   using namespace std; // Help ADL of std exp, log...
   using std::exp;

   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(3.14159265358979323846264338327950288419716939937510L), pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(3.14159265358979323846264338327950288419716939937510L), (pi<RealType, boost::math::policies::policy<> >)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(sqrt(3.14159265358979323846264338327950288419716939937510L)), root_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(sqrt(3.14159265358979323846264338327950288419716939937510L/2)), root_half_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(sqrt(3.14159265358979323846264338327950288419716939937510L * 2)), root_two_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(sqrt(log(4.0L))), root_ln_four<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(2.71828182845904523536028747135266249775724709369995L), e<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(0.5), half<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(0.57721566490153286060651209008240243104259335L), euler<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(sqrt(2.0L)), root_two<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(log(2.0L)), ln_two<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(log(log(2.0L))), ln_ln_two<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(1)/3, third<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(2)/3, twothirds<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(0.14159265358979323846264338327950288419716939937510L), pi_minus_three<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(4. - 3.14159265358979323846264338327950288419716939937510L), four_minus_pi<RealType>(), tolerance); 
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(pow((4 - 3.14159265358979323846264338327950288419716939937510L), 1.5L)), pow23_four_minus_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(exp(-0.5L)), exp_minus_half<RealType>(), tolerance); 
#else
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(pow((4 - 3.14159265358979323846264338327950288419716939937510), 1.5)), pow23_four_minus_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(exp(-0.5)), exp_minus_half<RealType>(), tolerance); 
#endif

} // template <class RealType>void test_spots(RealType)

void test_float_spots()
{
   // Basic sanity checks for constants in boost::math::float_constants::

   float tolerance = boost::math::tools::epsilon<float>() * 2;

   using namespace boost::math::float_constants;
   BOOST_MATH_STD_USING

   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(3.14159265358979323846264338327950288419716939937510L), pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(sqrt(3.14159265358979323846264338327950288419716939937510L)), root_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(sqrt(3.14159265358979323846264338327950288419716939937510L/2)), root_half_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(sqrt(3.14159265358979323846264338327950288419716939937510L * 2)), root_two_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(sqrt(log(4.0L))), root_ln_four, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(2.71828182845904523536028747135266249775724709369995L), e, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(0.5), half, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(0.57721566490153286060651209008240243104259335L), euler, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(sqrt(2.0L)), root_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(log(2.0L)), ln_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(log(log(2.0L))), ln_ln_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(1)/3, third, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(2)/3, twothirds, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(0.14159265358979323846264338327950288419716939937510L), pi_minus_three, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(4. - 3.14159265358979323846264338327950288419716939937510L), four_minus_pi, tolerance); 
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(pow((4 - 3.14159265358979323846264338327950288419716939937510L), 1.5L)), pow23_four_minus_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(exp(-0.5L)), exp_minus_half, tolerance); 
#else
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(pow((4 - 3.14159265358979323846264338327950288419716939937510), 1.5)), pow23_four_minus_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<float>(exp(-0.5)), exp_minus_half, tolerance); 
#endif

} // template <class RealType>void test_spots(RealType)

void test_double_spots()
{
   // Basic sanity checks for constants in boost::math::double_constants::

   double tolerance = boost::math::tools::epsilon<double>() * 2;

   using namespace boost::math::double_constants;
   BOOST_MATH_STD_USING

   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(3.14159265358979323846264338327950288419716939937510L), pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(sqrt(3.14159265358979323846264338327950288419716939937510L)), root_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(sqrt(3.14159265358979323846264338327950288419716939937510L/2)), root_half_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(sqrt(3.14159265358979323846264338327950288419716939937510L * 2)), root_two_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(sqrt(log(4.0L))), root_ln_four, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(2.71828182845904523536028747135266249775724709369995L), e, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(0.5), half, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(0.57721566490153286060651209008240243104259335L), euler, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(sqrt(2.0L)), root_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(log(2.0L)), ln_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(log(log(2.0L))), ln_ln_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(1)/3, third, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(2)/3, twothirds, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(0.14159265358979323846264338327950288419716939937510L), pi_minus_three, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(4. - 3.14159265358979323846264338327950288419716939937510L), four_minus_pi, tolerance); 
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(pow((4 - 3.14159265358979323846264338327950288419716939937510L), 1.5L)), pow23_four_minus_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(exp(-0.5L)), exp_minus_half, tolerance); 
#else
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(pow((4 - 3.14159265358979323846264338327950288419716939937510), 1.5)), pow23_four_minus_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<double>(exp(-0.5)), exp_minus_half, tolerance); 
#endif

} // template <class RealType>void test_spots(RealType)

void test_long_double_spots()
{
   // Basic sanity checks for constants in boost::math::long double_constants::

   long double tolerance = boost::math::tools::epsilon<long double>() * 2;

   using namespace boost::math::long_double_constants;
   BOOST_MATH_STD_USING

   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(3.14159265358979323846264338327950288419716939937510L), pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(sqrt(3.14159265358979323846264338327950288419716939937510L)), root_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(sqrt(3.14159265358979323846264338327950288419716939937510L/2)), root_half_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(sqrt(3.14159265358979323846264338327950288419716939937510L * 2)), root_two_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(sqrt(log(4.0L))), root_ln_four, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(2.71828182845904523536028747135266249775724709369995L), e, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(0.5), half, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(0.57721566490153286060651209008240243104259335L), euler, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(sqrt(2.0L)), root_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(log(2.0L)), ln_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(log(log(2.0L))), ln_ln_two, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(1)/3, third, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(2)/3, twothirds, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(0.14159265358979323846264338327950288419716939937510L), pi_minus_three, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(4. - 3.14159265358979323846264338327950288419716939937510L), four_minus_pi, tolerance); 
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(pow((4 - 3.14159265358979323846264338327950288419716939937510L), 1.5L)), pow23_four_minus_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(exp(-0.5L)), exp_minus_half, tolerance); 
#else
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(pow((4 - 3.14159265358979323846264338327950288419716939937510), 1.5)), pow23_four_minus_pi, tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<long double>(exp(-0.5)), exp_minus_half, tolerance); 
#endif

} // template <class RealType>void test_spots(RealType)

int test_main(int, char* [])
{
   // Basic sanity-check spot values.

   test_float_spots();
   test_double_spots();
   test_long_double_spots();

   // (Parameter value, arbitrarily zero, only communicates the floating point type).
   test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
   test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(0.0L); // Test long double.
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0582))
   test_spots(boost::math::concepts::real_concept(0.)); // Test real concept.
#endif
#else
  std::cout << "<note>The long double tests have been disabled on this platform "
    "either because the long double overloads of the usual math functions are "
    "not available at all, or because they are too inaccurate for these tests "
    "to pass.</note>" << std::cout;
#endif

  return 0;
} // int test_main(int, char* [])

/*

Output:

Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\test_constants.exe"
Running 1 test case...
*** No errors detected

*/






