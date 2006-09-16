// Copyright Paul A. Bristow 2006.
// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_normal.cpp

// http://en.wikipedia.org/wiki/Normal_distribution
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda3661.htm
// Also:
// Weisstein, Eric W. "Normal Distribution." 
// From MathWorld--A Wolfram Web Resource. 
// http://mathworld.wolfram.com/NormalDistribution.html 


#define BOOST_MATH_THROW_ON_DOMAIN_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4100) // unreferenced formal parameter.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4510) // default constructor could not be generated.
#  pragma warning(disable: 4610) // can never be instantiated - user defined constructor required.
#  if !(defined _SCL_SECURE_NO_DEPRECATE) || (_SCL_SECURE_NO_DEPRECATE == 0)
#    pragma warning(disable: 4996) // 'std::char_traits<char>::copy' was declared deprecated.
     // #define _SCL_SECURE_NO_DEPRECATE = 1 // avoid C4996 warning.
#  endif
//#  pragma warning(disable: 4244) // conversion from 'double' to 'float', possible loss of data.
#endif

#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>

#include <boost/math/distributions/normal.hpp>
	 using boost::math::normal_distribution;
#include <boost/math/tools/test.hpp> 

#include <iostream>
	using std::cout;
	using std::endl;
	using std::setprecision;
#include <limits>
  using std::numeric_limits;

template <class RealType>
RealType NaivePDF(RealType mean, RealType sd, RealType x)
{
   // Deliberately naive PDF calculator again which
   // we'll compare our pdf function.  However some
   // published values to compare against would be better....
   using namespace std;
   return exp(-(x-mean)*(x-mean)/(2*sd*sd))/(sd * sqrt(2*boost::math::constants::pi<RealType>()));
}

template <class RealType>
void check_normal(RealType mean, RealType sd, RealType x, RealType p, RealType q, RealType tol)
{
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         normal_distribution<RealType>(mean, sd),       // distribution.
         x),                                            // random variable.
         p,                                             // probability.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         complement(
            normal_distribution<RealType>(mean, sd),    // distribution.
            x)),                                        // random variable.
         q,                                             // probability complement.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         normal_distribution<RealType>(mean, sd),       // distribution.
         p),                                            // probability.
         x,                                             // random variable.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         complement(
            normal_distribution<RealType>(mean, sd),    // distribution.
            q)),                                        // probability complement.
         x,                                             // random variable.
			tol);                                          // %tolerance.
}

template <class RealType>
void test_spots(RealType T)
{
   // Basic santity checks
	RealType tolerance = static_cast<RealType>(std::pow(10., -(4-2))); // 1e-4 (as %)
	// Some tests only pass at 1e-4 because values generated by
   // http://faculty.vassar.edu/lowry/VassarStats.html
   // give only 5 or 6 *fixed* places, so small values have fewer
   // digits.

	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

   check_normal(
      static_cast<RealType>(5),
      static_cast<RealType>(2),
      static_cast<RealType>(4.8),
      static_cast<RealType>(0.46017),
      static_cast<RealType>(1 - 0.46017),
      tolerance);

   check_normal(
      static_cast<RealType>(5),
      static_cast<RealType>(2),
      static_cast<RealType>(5.2),
      static_cast<RealType>(1 - 0.46017),
      static_cast<RealType>(0.46017),
      tolerance);

   check_normal(
      static_cast<RealType>(5),
      static_cast<RealType>(2),
      static_cast<RealType>(2.2),
      static_cast<RealType>(0.08076),
      static_cast<RealType>(1 - 0.08076),
      tolerance);

   check_normal(
      static_cast<RealType>(5),
      static_cast<RealType>(2),
      static_cast<RealType>(7.8),
      static_cast<RealType>(1 - 0.08076),
      static_cast<RealType>(0.08076),
      tolerance);

   check_normal(
      static_cast<RealType>(-3),
      static_cast<RealType>(5),
      static_cast<RealType>(-4.5),
      static_cast<RealType>(0.38209),
      static_cast<RealType>(1 - 0.38209),
      tolerance);

   check_normal(
      static_cast<RealType>(-3),
      static_cast<RealType>(5),
      static_cast<RealType>(-1.5),
      static_cast<RealType>(1 - 0.38209),
      static_cast<RealType>(0.38209),
      tolerance);

   check_normal(
      static_cast<RealType>(-3),
      static_cast<RealType>(5),
      static_cast<RealType>(-8.5),
      static_cast<RealType>(0.13567),
      static_cast<RealType>(1 - 0.13567),
      tolerance);

   check_normal(
      static_cast<RealType>(-3),
      static_cast<RealType>(5),
      static_cast<RealType>(2.5),
      static_cast<RealType>(1 - 0.13567),
      static_cast<RealType>(0.13567),
      tolerance);

   //
   // Tests for PDF: we know that the peek value is at 1/sqrt(2*pi)
   //
   tolerance = boost::math::tools::epsilon<RealType>() * 5 * 100; // 5 eps as a persentage
   BOOST_CHECK_CLOSE(
      pdf(normal_distribution<RealType>(), static_cast<RealType>(0)), 
      static_cast<RealType>(0.3989422804014326779399460599343818684759L),
      tolerance);
   BOOST_CHECK_CLOSE(
      pdf(normal_distribution<RealType>(3), static_cast<RealType>(3)), 
      static_cast<RealType>(0.3989422804014326779399460599343818684759L),
      tolerance);
   BOOST_CHECK_CLOSE(
      pdf(normal_distribution<RealType>(3, 5), static_cast<RealType>(3)), 
      static_cast<RealType>(0.3989422804014326779399460599343818684759L / 5),
      tolerance);

   //
   // Spot checks for mean = -5, sd = 6:
   //
   for(RealType x = -15; x < 5; x += 0.125)
   {
      BOOST_CHECK_CLOSE(
         pdf(normal_distribution<RealType>(-5, 6), x), 
         NaivePDF(RealType(-5), RealType(6), x),
         tolerance);
   }

    RealType tol2 = boost::math::tools::epsilon<RealType>() * 5;
    normal_distribution<RealType> dist(8, 3);
    RealType x = static_cast<RealType>(0.125);
    using namespace std; // ADL of std names.
    // mean:
    BOOST_CHECK_CLOSE(
       mean(dist)
       , static_cast<RealType>(8), tol2);
    // variance:
    BOOST_CHECK_CLOSE(
       variance(dist)
       , static_cast<RealType>(9), tol2);
    // std deviation:
    BOOST_CHECK_CLOSE(
       standard_deviation(dist)
       , static_cast<RealType>(3), tol2);
    // hazard:
    BOOST_CHECK_CLOSE(
       hazard(dist, x)
       , pdf(dist, x) / cdf(complement(dist, x)), tol2);
    // cumulative hazard:
    BOOST_CHECK_CLOSE(
       chf(dist, x)
       , -log(cdf(complement(dist, x))), tol2);
    // coefficient_of_variation:
    BOOST_CHECK_CLOSE(
       coefficient_of_variation(dist)
       , standard_deviation(dist) / mean(dist), tol2);

} // template <class RealType>void test_spots(RealType)

int test_main(int, char* [])
{
	 // Basic sanity-check spot values.
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

------ Build started: Project: test_normal, Configuration: Debug Win32 ------
Compiling...
test_normal.cpp
Linking...
Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\test_normal.exe"
Running 1 test case...
Tolerance for type float is 0.01 %
Tolerance for type double is 0.01 %
Tolerance for type long double is 0.01 %
Tolerance for type class boost::math::concepts::real_concept is 0.01 %
*** No errors detected
Build Time 0:07
Build log was saved at "file://i:\boost-06-05-03-1300\libs\math\test\Math_test\test_normal\Debug\BuildLog.htm"
test_normal - 0 error(s), 0 warning(s)
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========


*/

