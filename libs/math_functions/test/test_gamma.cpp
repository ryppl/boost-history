//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/concepts/real_concept.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/tools/stats.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "test_gamma_hooks.hpp"
#include "handle_test_result.hpp"

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the functions tgamma and lgamma, and the 
// function tgamma1pm1.  There are two sets of tests, spot
// tests which compare our results with selected values computed
// using the online special function calculator at 
// functions.wolfram.com, while the bulk of the accuracy tests
// use values generated with NTL::RR at 1000-bit precision
// and our generic versions of these functions.
//
// Note that when this file is first run on a new platform many of
// these tests will fail: the default accuracy is 1 epsilon which
// is too tight for most platforms.  In this situation you will 
// need to cast a human eye over the error rates reported and make
// a judgement as to whether they are acceptable.  Either way please
// report the results to the Boost mailing list.  Acceptable rates of
// error are marked up below as a series of regular expressions that
// identify the compiler/stdlib/platform/data-type/test-data/test-function
// along with the maximum expected peek and RMS mean errors for that
// test.
//

void expected_results()
{
   //
   // Define the max and mean errors expected for
   // various compilers and platforms.
   //
   const char* largest_type;
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   if(boost::math::tools::digits<double>() == boost::math::tools::digits<long double>())
   {
      largest_type = "(long\\s+)?double";
   }
   else
   {
      largest_type = "long double";
   }
#else
   largest_type = "(long\\s+)?double";
#endif
   //
   // G++ on Linux, result vary a bit by processor type,
   // on Itanium results are *much* better than listed here,
   // but x86 appears to have much less accurate std::pow
   // that throws off the results:
   //
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      largest_type,                  // test type(s)
      "factorials",                  // test data group
      "boost::math::tgamma", 400, 200); // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                          // platform
      largest_type,                  // test type(s)
      "factorials",                  // test data group
      "boost::math::lgamma", 30, 10);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      largest_type,                  // test type(s)
      "near (1|2|-10)",              // test data group
      "boost::math::tgamma", 8, 5);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      largest_type,                  // test type(s)
      "near (1|2|-10)",              // test data group
      "boost::math::lgamma", 50, 30);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      largest_type,                  // test type(s)
      "tgamma1pm1.*",                // test data group
      "boost::math::tgamma1pm1", 50, 15);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      "real_concept",                // test type(s)
      "factorials",                  // test data group
      "boost::math::tgamma", 220, 70);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      "real_concept",                // test type(s)
      "near (0|-55)",                // test data group
      "boost::math::(t|l)gamma", 130, 60);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "linux",                       // platform
      "real_concept",                // test type(s)
      "tgamma1pm1.*",                // test data group
      "boost::math::tgamma1pm1", 40, 10);  // test function
   //
   // HP-UX results:
   //
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "HP-UX",                          // platform
      largest_type,                  // test type(s)
      "factorials",                  // test data group
      "boost::math::tgamma", 5, 4);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "HP-UX",                       // platform
      largest_type,                  // test type(s)
      "near (0|-55)",                // test data group
      "boost::math::tgamma", 10, 5);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "HP-UX",                       // platform
      largest_type,                  // test type(s)
      "near (1|2|-10)",              // test data group
      "boost::math::lgamma", 250, 200);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "HP-UX",                          // platform
      "real_concept",                // test type(s)
      "factorials",                  // test data group
      "boost::math::lgamma", 50, 20);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "HP-UX",                          // platform
      "real_concept",                // test type(s)
      "tgamma1pm1.*",                // test data group
      "boost::math::tgamma1pm1", 200, 60);  // test function

   //
   // Catch all cases come last:
   //
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "factorials",                  // test data group
      "boost::math::tgamma", 4, 1);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "factorials",                  // test data group
      "boost::math::lgamma", 9, 1);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "near (0|-55)",                // test data group
      "boost::math::(t|l)gamma", 200, 100);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "near (1|2|-10)",              // test data group
      "boost::math::tgamma", 8, 5);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "near (1|2|-10)",              // test data group
      "boost::math::lgamma", 14, 7);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "tgamma1pm1.*",                // test data group
      "boost::math::tgamma1pm1", 30, 9);  // test function

   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "factorials",                  // test data group
      "boost::math::tgamma", 70, 25);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "factorials",                  // test data group
      "boost::math::lgamma", 30, 4);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "near.*",                      // test data group
      "boost::math::tgamma", 50, 30);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "near.*",                      // test data group
      "boost::math::lgamma", 10000000, 10000000);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "tgamma1pm1.*",                // test data group
      "boost::math::tgamma1pm1", 20, 5);  // test function

   //
   // Finish off by printing out the compiler/stdlib/platform names,
   // we do this to make it easier to mark up expected error rates.
   //
   std::cout << "Tests run with " << BOOST_COMPILER << ", " 
      << BOOST_STDLIB << ", " << BOOST_PLATFORM << std::endl;
}

template <class T>
void do_test_gamma(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type);
   pg funcp = boost::math::tgamma;

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test tgamma against data:
   //
   result = boost::math::tools::test(
      data,
      boost::lambda::bind(funcp, boost::lambda::ret<value_type>(boost::lambda::_1[0])),
      boost::lambda::ret<value_type>(boost::lambda::_1[1]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::tgamma", test_name);
#ifdef TEST_OTHER
   if(::boost::is_floating_point<value_type>::value){
      funcp = other::tgamma;
      result = boost::math::tools::test(
         data,
         boost::lambda::bind(funcp, boost::lambda::ret<value_type>(boost::lambda::_1[0])),
         boost::lambda::ret<value_type>(boost::lambda::_1[1]));
      print_test_result(result, data[result.worst()], result.worst(), type_name, "other::tgamma");
   }
#endif
   //
   // test lgamma against data:
   //
   funcp = boost::math::lgamma;
   result = boost::math::tools::test(
      data,
      boost::lambda::bind(funcp, boost::lambda::ret<value_type>(boost::lambda::_1[0])),
      boost::lambda::ret<value_type>(boost::lambda::_1[2]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::lgamma", test_name);
#ifdef TEST_OTHER
   if(::boost::is_floating_point<value_type>::value){
      funcp = other::lgamma;
      result = boost::math::tools::test(
         data,
         boost::lambda::bind(funcp, boost::lambda::ret<value_type>(boost::lambda::_1[0])),
         boost::lambda::ret<value_type>(boost::lambda::_1[2]));
      print_test_result(result, data[result.worst()], result.worst(), type_name, "other::lgamma");
   }
#endif

   std::cout << std::endl;
}

template <class T>
void do_test_gammap1m1(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type);
   pg funcp = boost::math::tgamma1pm1;

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test tgamma1pm1 against data:
   //
   result = boost::math::tools::test(
      data,
      boost::lambda::bind(funcp, boost::lambda::ret<value_type>(boost::lambda::_1[0])),
      boost::lambda::ret<value_type>(boost::lambda::_1[1]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::tgamma1pm1", test_name);
   std::cout << std::endl;
}

template <class T>
void test_gamma(T, const char* name)
{
   //
   // The actual test data is rather verbose, so it's in a separate file
   //
   // The contents are as follows, each row of data contains
   // three items, input value, gamma and lgamma:
   //
   // gamma and lgamma at integer and half integer values:
   // boost::array<boost::array<T, 3>, N> factorials;
   //
   // gamma and lgamma for z near 0:
   // boost::array<boost::array<T, 3>, N> near_0;
   //
   // gamma and lgamma for z near 1:
   // boost::array<boost::array<T, 3>, N> near_1;
   //
   // gamma and lgamma for z near 2:
   // boost::array<boost::array<T, 3>, N> near_2;
   //
   // gamma and lgamma for z near -10:
   // boost::array<boost::array<T, 3>, N> near_m10;
   //
   // gamma and lgamma for z near -55:
   // boost::array<boost::array<T, 3>, N> near_m55;
   //
   // The last two cases are chosen more or less at random,
   // except that one is even and the other odd, and both are
   // at negative poles.  The data near zero also tests near
   // a pole, the data near 1 and 2 are to probe lgamma as
   // the result -> 0.
   //
#  include "test_gamma_data.ipp"

   do_test_gamma(factorials, name, "factorials");
   do_test_gamma(near_0, name, "near 0");
   do_test_gamma(near_1, name, "near 1");
   do_test_gamma(near_2, name, "near 2");
   do_test_gamma(near_m10, name, "near -10");
   do_test_gamma(near_m55, name, "near -55");

   //
   // And now tgamma1pm1 which computes gamma(1+dz)-1:
   //
   do_test_gammap1m1(gammap1m1_data, name, "tgamma1pm1(dz)");
}

template <class T>
void test_spots(T)
{
   //
   // basic sanity checks, tolerance is 50 epsilon expressed as a percentage:
   //
   T tolerance = boost::math::tools::epsilon<T>() * 5000;
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(3.5)), static_cast<T>(3.3233509704478425511840640312646472177454052302295L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(0.125)), static_cast<T>(7.5339415987976119046992298412151336246104195881491L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(-0.125)), static_cast<T>(-8.7172188593831756100190140408231437691829605421405L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(-3.125)), static_cast<T>(1.1668538708507675587790157356605097019141636072094L), tolerance);
   // Lower tolerance on this one, is only really needed on Linux x86 systems, result is mostly down to std lib accuracy:
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(-53249.0/1024)), static_cast<T>(-1.2646559519067605488251406578743995122462767733517e-65L), tolerance * 3);

   int sign = 1;
   BOOST_CHECK_CLOSE(::boost::math::lgamma(static_cast<T>(3.5), &sign), static_cast<T>(1.2009736023470742248160218814507129957702389154682L), tolerance);
   BOOST_CHECK(sign == 1);
   BOOST_CHECK_CLOSE(::boost::math::lgamma(static_cast<T>(0.125), &sign), static_cast<T>(2.0194183575537963453202905211670995899482809521344L), tolerance);
   BOOST_CHECK(sign == 1);
   BOOST_CHECK_CLOSE(::boost::math::lgamma(static_cast<T>(-0.125), &sign), static_cast<T>(2.1653002489051702517540619481440174064962195287626L), tolerance);
   BOOST_CHECK(sign == -1);
   BOOST_CHECK_CLOSE(::boost::math::lgamma(static_cast<T>(-3.125), &sign), static_cast<T>(0.1543111276840418242676072830970532952413339012367L), tolerance);
   BOOST_CHECK(sign == 1);
   BOOST_CHECK_CLOSE(::boost::math::lgamma(static_cast<T>(-53249.0/1024), &sign), static_cast<T>(-149.43323093420259741100038126078721302600128285894L), tolerance);
   BOOST_CHECK(sign == -1);
}

int test_main(int, char* [])
{
   expected_results();

   test_spots(0.0F);
   test_spots(0.0);
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(0.0L);
   test_spots(boost::math::concepts::real_concept(0.1));
#endif

   test_gamma(0.1F, "float");
   test_gamma(0.1, "double");
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_gamma(0.1L, "long double");
   test_gamma(boost::math::concepts::real_concept(0.1), "real_concept");
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif
   return 0;
}


