// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_students_t.cpp

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_OVERFLOW_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4100) // unreferenced formal parameter.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4510) // default constructor could not be generated.
#  pragma warning(disable: 4610) // can never be instantiated - user defined constructor required.
//#  pragma warning(disable: 4535) // calling _set_se_translator() requires /EHa (in Boost.test)
// Enable C++ Exceptions Yes With SEH Exceptions (/EHa) prevents warning 4535.
#endif

#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/math/distributions/extreme_value.hpp>
	 using boost::math::extreme_value_distribution;

#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>


#include <iostream>
	using std::cout;
	using std::endl;
	using std::setprecision;

template <class RealType>
void test_spot(RealType a, RealType b, RealType x, RealType p, RealType q, RealType tolerance)
{
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         extreme_value_distribution<RealType>(a, b),      
         x),
         p,
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         complement(extreme_value_distribution<RealType>(a, b),      
         x)),
         q,
			tolerance); // %
   if((p < 0.999) && (p > 0))
   {
      BOOST_CHECK_CLOSE(
         ::boost::math::quantile(
            extreme_value_distribution<RealType>(a, b),      
            p),
            x,
			   tolerance); // %
   }
   if((q < 0.999) && (q > 0))
   {
      BOOST_CHECK_CLOSE(
         ::boost::math::quantile(
            complement(extreme_value_distribution<RealType>(a, b),      
            q)),
            x,
			   tolerance); // %
   }
}

template <class RealType>
void test_spots(RealType T)
{
   // Basic sanity checks.
   // 50eps as a persentage, up to a maximum of double precision
   // (that's the limit of our test data).
   RealType tolerance = (std::max)(
      static_cast<RealType>(boost::math::tools::epsilon<double>()),
      boost::math::tools::epsilon<RealType>());
   tolerance *= 50 * 100;  

	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

   // Results calculated by punching numbers into a calculator,
   // and using the formula at http://mathworld.wolfram.com/ExtremeValueDistribution.html
   test_spot(
      static_cast<RealType>(0.5), // a
      static_cast<RealType>(1.5), // b
      static_cast<RealType>(0.125), // x
      static_cast<RealType>(0.27692033409990891617007608217222L), // p
      static_cast<RealType>(0.72307966590009108382992391782778L), //q
      tolerance);
   test_spot(
      static_cast<RealType>(0.5), // a
      static_cast<RealType>(2), // b
      static_cast<RealType>(-5), // x
      static_cast<RealType>(1.6087601139887776413169427645933e-7L), // p
      static_cast<RealType>(0.99999983912398860112223586830572L), //q
      tolerance);
   test_spot(
      static_cast<RealType>(0.5), // a
      static_cast<RealType>(0.25), // b
      static_cast<RealType>(0.75), // x
      static_cast<RealType>(0.69220062755534635386542199718279L), // p
      static_cast<RealType>(0.30779937244465364613457800281721), //q
      tolerance);
   test_spot(
      static_cast<RealType>(0.5), // a
      static_cast<RealType>(0.25), // b
      static_cast<RealType>(5), // x
      static_cast<RealType>(0.99999998477002037126351248727041L), // p
      static_cast<RealType>(1.5229979628736487512729586276294e-8L), //q
      tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         extreme_value_distribution<RealType>(0.5, 2),      
         static_cast<RealType>(0.125)),              // x
         static_cast<RealType>(0.18052654830890205978204427757846L),                // probability.
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         extreme_value_distribution<RealType>(1, 3),      
         static_cast<RealType>(5)),              // x
         static_cast<RealType>(0.0675057324099851209129017326286L),                // probability.
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         extreme_value_distribution<RealType>(1, 3),      
         static_cast<RealType>(0)),              // x
         static_cast<RealType>(0.11522236828583456431277265757312L),                // probability.
			tolerance); // %

   BOOST_CHECK_CLOSE(
      ::boost::math::mean(
         extreme_value_distribution<RealType>(2, 3)),
         static_cast<RealType>(3.731646994704598581819536270246L),           
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::standard_deviation(
         extreme_value_distribution<RealType>(-1, 0.5)), 
         static_cast<RealType>(0.6412749150809320477720181798355L),
			tolerance); // %

   //
   // Things that are errors:
   //
   extreme_value_distribution<RealType> dist(0.5, 2);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(1.0)),
       std::overflow_error);
   BOOST_CHECK_THROW(
       quantile(complement(dist, RealType(0.0))),
       std::overflow_error);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(0.0)),
       std::overflow_error);
   BOOST_CHECK_THROW(
       quantile(complement(dist, RealType(1.0))),
       std::overflow_error);
   BOOST_CHECK_THROW(
       cdf(extreme_value_distribution<RealType>(0, -1), RealType(1)),
       std::domain_error);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(-1)),
       std::domain_error);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(2)),
       std::domain_error);
} // template <class RealType>void test_spots(RealType)

int test_main(int, char* [])
{
	 // Basic sanity-check spot values.
	// (Parameter value, arbitrarily zero, only communicates the floating point type).
  test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
  test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
  test_spots(0.0L); // Test long double.
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
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

