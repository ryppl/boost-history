// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_gamma_dist.cpp

// http://en.wikipedia.org/wiki/Gamma_distribution
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda366b.htm
// Also:
// Weisstein, Eric W. "Gamma Distribution." 
// From MathWorld--A Wolfram Web Resource. 
// http://mathworld.wolfram.com/GammaDistribution.html


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

#include <boost/math/distributions/gamma.hpp>
	 using boost::math::gamma_distribution;
#include <boost/math/tools/test.hpp> 

#include <iostream>
	using std::cout;
	using std::endl;
	using std::setprecision;
#include <limits>
  using std::numeric_limits;

template <class RealType>
RealType NaivePDF(RealType shape, RealType scale, RealType x)
{
   // Deliberately naive PDF calculator again which
   // we'll compare our pdf function.  However some
   // published values to compare against would be better....
   using namespace std;
   RealType result = log(x) * (shape - 1) - x / scale - boost::math::lgamma(shape) - log(scale) * shape;
   return exp(result);
}

template <class RealType>
void check_gamma(RealType shape, RealType scale, RealType x, RealType p, RealType q, RealType tol)
{
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         gamma_distribution<RealType>(shape, scale),    // distribution.
         x),                                            // random variable.
         p,                                             // probability.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         complement(
            gamma_distribution<RealType>(shape, scale), // distribution.
            x)),                                        // random variable.
         q,                                             // probability complement.
			tol);                                          // %tolerance.
   if(p < 0.999)
   {
      BOOST_CHECK_CLOSE(
         ::boost::math::quantile(
            gamma_distribution<RealType>(shape, scale),    // distribution.
            p),                                            // probability.
            x,                                             // random variable.
			   tol);                                          // %tolerance.
   }
   if(q < 0.999)
   {
      BOOST_CHECK_CLOSE(
         ::boost::math::quantile(
            complement(
               gamma_distribution<RealType>(shape, scale), // distribution.
               q)),                                        // probability complement.
            x,                                             // random variable.
			   tol);                                          // %tolerance.
   }
   // PDF:
   BOOST_CHECK_CLOSE(
      boost::math::pdf(
         gamma_distribution<RealType>(shape, scale),    // distribution.
         x),                                            // random variable.
         NaivePDF(shape, scale, x),                     // PDF
		   tol);                                          // %tolerance.
}

template <class RealType>
void test_spots(RealType T)
{
   // Basic santity checks
   //
	// 15 decimal places expressed as a persentage.
   // The first tests use values generated by MathCAD,
   // and should be accurate to around double precision.
   //
   RealType tolerance = (std::max)(5e-14f, boost::math::tools::real_cast<float>(std::numeric_limits<RealType>::epsilon() * 20)) * 100; 
	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

   check_gamma(
      static_cast<RealType>(0.5),
      static_cast<RealType>(1),
      static_cast<RealType>(0.5),
      static_cast<RealType>(0.682689492137085),
      static_cast<RealType>(1-0.682689492137085),
      tolerance);
   check_gamma(
      static_cast<RealType>(2),
      static_cast<RealType>(1),
      static_cast<RealType>(0.5),
      static_cast<RealType>(0.090204010431050),
      static_cast<RealType>(1-0.090204010431050),
      tolerance);
   check_gamma(
      static_cast<RealType>(40),
      static_cast<RealType>(1),
      static_cast<RealType>(10),
      static_cast<RealType>(7.34163631456064E-13),
      static_cast<RealType>(1-7.34163631456064E-13),
      tolerance);

   //
   // Some more test data generated by the online
   // calculator at http://espse.ed.psu.edu/edpsych/faculty/rhale/hale/507Mat/statlets/free/pdist.htm
   // This has the advantage of supporting the scale parameter as well
   // as shape, but has only a few digits accuracy, and produces
   // some deeply suspect values if the shape parameter is < 1
   // (it doesn't agree with MathCAD or this implementation).
   // To be fair the incomplete gamma is tricky to get right in this area...
   //
	tolerance = 1e-5f * 100; // 5 decimal places as a persentage
	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

   check_gamma(
      static_cast<RealType>(2),
      static_cast<RealType>(1)/5,
      static_cast<RealType>(0.1),
      static_cast<RealType>(0.090204),
      static_cast<RealType>(1-0.090204),
      tolerance);
   check_gamma(
      static_cast<RealType>(2),
      static_cast<RealType>(1)/5,
      static_cast<RealType>(0.5),
      static_cast<RealType>(1-0.287298),
      static_cast<RealType>(0.287298),
      tolerance);
   check_gamma(
      static_cast<RealType>(3),
      static_cast<RealType>(2),
      static_cast<RealType>(1),
      static_cast<RealType>(0.014388),
      static_cast<RealType>(1-0.014388),
      tolerance * 10); // one less decimal place in the test value
   check_gamma(
      static_cast<RealType>(3),
      static_cast<RealType>(2),
      static_cast<RealType>(5),
      static_cast<RealType>(0.456187),
      static_cast<RealType>(1-0.456187),
      tolerance);


    RealType tol2 = boost::math::tools::epsilon<RealType>() * 5;
    gamma_distribution<RealType> dist(8, 3);
    RealType x = static_cast<RealType>(0.125);
    using namespace std; // ADL of std names.
    // mean:
    BOOST_CHECK_CLOSE(
       mean(dist)
       , static_cast<RealType>(8*3), tol2);
    // variance:
    BOOST_CHECK_CLOSE(
       variance(dist)
       , static_cast<RealType>(8*3*3), tol2);
    // std deviation:
    BOOST_CHECK_CLOSE(
       standard_deviation(dist)
       , sqrt(static_cast<RealType>(8*3*3)), tol2);
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
    // mode:
    BOOST_CHECK_CLOSE(
       mode(dist)
       , static_cast<RealType>(7 * 3), tol2);
    // skewness:
    BOOST_CHECK_CLOSE(
       skewness(dist)
       , 2 / sqrt(static_cast<RealType>(8)), tol2);
    // kertosis:
    BOOST_CHECK_CLOSE(
       kurtosis(dist)
       , 3 + 6 / static_cast<RealType>(8), tol2);
    // kertosis excess:
    BOOST_CHECK_CLOSE(
       kurtosis_excess(dist)
       , 6 / static_cast<RealType>(8), tol2);

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


