// Copyright John Maddock 2008
// Copyright Paul A. Bristow 
// Copyright Gautam Sewani

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_MATH_OVERFLOW_ERROR_POLICY throw_on_error
#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/math/distributions/hypergeometric.hpp>

#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>

#include <iostream>
   using std::cout;
   using std::endl;
   using std::setprecision;

#include <boost/array.hpp>
#include "functor.hpp"
#include "handle_test_result.hpp"

#define BOOST_CHECK_EX(a) \
   {\
      unsigned int failures = boost::unit_test::results_collector.results( boost::unit_test::framework::current_test_case().p_id ).p_assertions_failed;\
      BOOST_CHECK(a); \
      if(failures != boost::unit_test::results_collector.results( boost::unit_test::framework::current_test_case().p_id ).p_assertions_failed)\
      {\
         std::cerr << "Failure was with data ";\
         std::cerr << std::setprecision(35); \
         std::cerr << "x = " << x << ", r = " << r << ", n = " << n\
         << ", N = " << N << ", p = " << cp << ", q = " << ccp << std::endl;\
      }\
   }

   void expected_results()
{
   //
   // Define the max and mean errors expected for
   // various compilers and platforms.
   //
   const char* largest_type;
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   if(boost::math::policies::digits<double, boost::math::policies::policy<> >() == boost::math::policies::digits<long double, boost::math::policies::policy<> >())
   {
      largest_type = "(long\\s+)?double|real_concept";
   }
   else
   {
      largest_type = "long double|real_concept";
   }
#else
   largest_type = "(long\\s+)?double";
#endif
   if((boost::math::tools::digits<long double>() > boost::math::tools::digits<double>())
      && (boost::math::tools::digits<long double>() < 100))
   {
      //
      // Some split of errors from long double into double:
      //
      add_expected_result(
         ".*",                          // compiler
         ".*",                          // stdlib
         ".*",                          // platform
         "double",                      // test type(s)
         "Random.*",                    // test data group
         ".*", 1500, 1500);      // test function
      add_expected_result(
         ".*",                          // compiler
         ".*",                          // stdlib
         ".*",                          // platform
         "double",                      // test type(s)
         ".*",                    // test data group
         ".*", 10, 10);      // test function
   }

   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "Random.*",                    // test data group
      ".*", 250000000, 25000000);    // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      "Random.*",                    // test data group
      ".*", 10000000, 5000000);      // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      ".*",                          // test data group
      ".*", 50, 20);                 // test function
}

template <class T>
inline unsigned make_unsigned(T x)
{
   return static_cast<unsigned>(x);
}
template<>
inline unsigned make_unsigned(boost::math::concepts::real_concept x)
{
   return static_cast<unsigned>(x.value());
}

template <class T>
T pdf_tester(T r, T n, T N, T x)
{
   boost::math::hypergeometric_distribution<T> d(make_unsigned(r), make_unsigned(n), make_unsigned(N));
   return pdf(d, x);
}

template <class T>
T cdf_tester(T r, T n, T N, T x)
{
   boost::math::hypergeometric_distribution<T> d(make_unsigned(r), make_unsigned(n), make_unsigned(N));
   return cdf(d, x);
}

template <class T>
T ccdf_tester(T r, T n, T N, T x)
{
   boost::math::hypergeometric_distribution<T> d(make_unsigned(r), make_unsigned(n), make_unsigned(N));
   return cdf(complement(d, x));
}

template <class T>
void do_test_hypergeometric(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type, value_type, value_type, value_type);
#if defined(BOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS)
   pg funcp = pdf_tester<value_type>;
#else
   pg funcp = pdf_tester;
#endif

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test hypergeometric against data:
   //
   result = boost::math::tools::test(
      data, 
      bind_func(funcp, 0, 1, 2, 3), 
      extract_result(4));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "hypergeometric PDF", test_name);

#if defined(BOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS)
   funcp = cdf_tester<value_type>;
#else
   funcp = cdf_tester;
#endif

   //
   // test hypergeometric against data:
   //
   result = boost::math::tools::test(
      data, 
      bind_func(funcp, 0, 1, 2, 3), 
      extract_result(5));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "hypergeometric CDF", test_name);

#if defined(BOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS)
   funcp = ccdf_tester<value_type>;
#else
   funcp = ccdf_tester;
#endif

   //
   // test hypergeometric against data:
   //
   result = boost::math::tools::test(
      data, 
      bind_func(funcp, 0, 1, 2, 3), 
      extract_result(6));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "hypergeometric CDF complement", test_name);
   std::cout << std::endl;
}

template <class T>
void do_test_hypergeometric_quantile(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   std::cout << "Checking quantiles with " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   if(boost::math::tools::digits<value_type>() > 50)
   {
      for(unsigned i = 0; i < data.size(); ++i)
      {
         using namespace boost::math::policies;

         unsigned r = make_unsigned(data[i][0]);
         unsigned n = make_unsigned(data[i][1]);
         unsigned N = make_unsigned(data[i][2]);
         unsigned x = make_unsigned(data[i][3]);
         value_type cp = data[i][5];
         value_type ccp = data[i][6];
#if 0
         boost::math::hypergeometric_distribution<RealType, 
            policy<discrete_quantile<integer_round_up> > > du(r, n, N);

         BOOST_CHECK_EX(quantile(du, cp) >= x);
         BOOST_CHECK_EX(quantile(complement(du, ccp)) >= x);

         boost::math::hypergeometric_distribution<RealType, 
            policy<discrete_quantile<integer_round_down> > > dl(r, n, N);
         BOOST_CHECK_EX(quantile(dl, cp) <= x);
         BOOST_CHECK_EX(quantile(complement(dl, ccp)) <= x);
#endif
         boost::math::hypergeometric_distribution<value_type, 
            policy<discrete_quantile<integer_round_nearest> > > dn(r, n, N);

         if((cp < 0.9) && (cp > boost::math::tools::min_value<value_type>()))
         {
            BOOST_CHECK_EX(quantile(dn, cp) == x);
         }
         if((ccp < 0.9) && (ccp > boost::math::tools::min_value<value_type>()))
         {
            BOOST_CHECK_EX(quantile(complement(dn, ccp)) == x);
         }
      }
   }
}


template <class RealType>
void test_spot(unsigned x, unsigned n, unsigned r, unsigned N, 
               RealType p, RealType cp, RealType ccp, RealType tol)
{
   boost::math::hypergeometric_distribution<RealType> d(r, n, N);

   std::pair<unsigned, unsigned> extent = range(d);
   // CDF's:
   BOOST_CHECK_CLOSE(pdf(d, x), p, tol);
   BOOST_CHECK_CLOSE(cdf(d, x), cp, tol);
   BOOST_CHECK_CLOSE(cdf(complement(d, x)), ccp, tol);
   // Again with real-value arguments:
   BOOST_CHECK_CLOSE(pdf(d, static_cast<RealType>(x)), p, tol);
   BOOST_CHECK_CLOSE(cdf(d, static_cast<RealType>(x)), cp, tol);
   BOOST_CHECK_CLOSE(cdf(complement(d, static_cast<RealType>(x))), ccp, tol);
   //
   // Quantiles, don't bother checking these for type float
   // as there's not enough precision in the p and q values
   // to get back to where we started:
   //
   if(boost::math::tools::digits<RealType>() > 50)
   {
      using namespace boost::math::policies;
#if 0
      boost::math::hypergeometric_distribution<RealType, 
         policy<discrete_quantile<integer_round_up> > > du(r, n, N);

      BOOST_CHECK_EX(quantile(du, cp) >= x);
      BOOST_CHECK_EX(quantile(complement(du, ccp)) >= x);

      boost::math::hypergeometric_distribution<RealType, 
         policy<discrete_quantile<integer_round_down> > > dl(r, n, N);
      BOOST_CHECK_EX(quantile(dl, cp) <= x);
      BOOST_CHECK_EX(quantile(complement(dl, ccp)) <= x);
#endif
      boost::math::hypergeometric_distribution<RealType, 
         policy<discrete_quantile<integer_round_nearest> > > dn(r, n, N);

      BOOST_CHECK_EX(quantile(dn, cp) == x);
      BOOST_CHECK_EX(quantile(complement(dn, ccp)) == x);
   }
   //
   // Error checking of out of bounds arguments:
   //
   BOOST_CHECK_THROW(pdf(d, extent.second + 1), std::domain_error);
   BOOST_CHECK_THROW(cdf(d, extent.second + 1), std::domain_error);
   BOOST_CHECK_THROW(cdf(complement(d, extent.second + 1)), std::domain_error);
   if(extent.first > 0)
   {
      BOOST_CHECK_THROW(pdf(d, extent.first - 1), std::domain_error);
      BOOST_CHECK_THROW(cdf(d, extent.first - 1), std::domain_error);
      BOOST_CHECK_THROW(cdf(complement(d, extent.first - 1)), std::domain_error);
   }
   BOOST_CHECK_THROW(quantile(d, 1.1f), std::domain_error);
   BOOST_CHECK_THROW(quantile(complement(d, 1.1f)), std::domain_error);
   BOOST_CHECK_THROW(quantile(d, -0.001f), std::domain_error);
   BOOST_CHECK_THROW(quantile(complement(d, -0.001f)), std::domain_error);
   //
   // Checking of extreme values:
   //
   BOOST_CHECK_EQUAL(quantile(d, 0), extent.first);
   BOOST_CHECK_EQUAL(quantile(d, 1), extent.second);
   BOOST_CHECK_EQUAL(quantile(complement(d, 0)), extent.second);
   BOOST_CHECK_EQUAL(quantile(complement(d, 1)), extent.first);
   BOOST_CHECK_EQUAL(cdf(d, extent.second), 1);
   BOOST_CHECK_EQUAL(cdf(complement(d, extent.second)), 0);
}

template <class RealType>
void test_spots(RealType /*T*/, const char* type_name)
{
   // Basic sanity checks.
   // 50 eps as a percentage, up to a maximum of double precision
   // Test data taken from Mathematica 6
#define T RealType
#include "hypergeometric_test_data.ipp"
   do_test_hypergeometric(hypergeometric_test_data, type_name, "Mathematica data");

#include "hypergeometric_dist_data2.ipp"
   if(boost::is_floating_point<RealType>::value)
   {
      //
      // Don't test this for real_concept: it's too slow!!!
      //
      do_test_hypergeometric(hypergeometric_dist_data2, type_name, "Random large data");
   }

   do_test_hypergeometric_quantile(hypergeometric_test_data, type_name, "Mathematica data");
   if(boost::is_floating_point<RealType>::value)
   {
      //
      // Don't test this for real_concept: it's too slow!!!
      //
      do_test_hypergeometric_quantile(hypergeometric_dist_data2, type_name, "Random large data");
   }

   RealType tolerance = (std::max)(
      static_cast<RealType>(2e-16L),    // limit of test data
      boost::math::tools::epsilon<RealType>());
   cout<<"Absolute tolerance:"<<tolerance<<endl;
   
   tolerance *= 50 * 100; // 50eps as a persentage
   cout << "Tolerance for type " << typeid(RealType).name()  << " is " << tolerance << " %" << endl;

   //
   // These sanity check values were calculated using the online
   // calculator at http://stattrek.com/Tables/Hypergeometric.aspx
   // It's assumed that the test values are accurate to no more than
   // double precision.
   //
   test_spot(20, 200, 50, 500, static_cast<T>(0.120748236361163), static_cast<T>(0.563532430195156), static_cast<T>(1 - 0.563532430195156), tolerance);
   test_spot(53, 452, 64, 500, static_cast<T>(0.0184749573044286), static_cast<T>(0.0299118078796907), static_cast<T>(1 - 0.0299118078796907), tolerance);
   test_spot(32, 1287, 128, 5000, static_cast<T>(0.0807012167418264), static_cast<T>(0.469768774237742), static_cast<T>(1 - 0.469768774237742), tolerance);
   test_spot(1, 13, 4, 26, static_cast<T>(0.248695652173913), static_cast<T>(0.296521739130435), static_cast<T>(1 - 0.296521739130435), tolerance);
   test_spot(2, 13, 4, 26, static_cast<T>(0.40695652173913), static_cast<T>(0.703478260869565), static_cast<T>(1 - 0.703478260869565), tolerance);
   test_spot(3, 13, 4, 26, static_cast<T>(0.248695652173913), static_cast<T>(0.952173913043478), static_cast<T>(1 - 0.952173913043478), tolerance);

   boost::math::hypergeometric_distribution<RealType> d(50, 200, 500);
   BOOST_CHECK_EQUAL(range(d).first, 0u);
   BOOST_CHECK_EQUAL(range(d).second, 50u);
   BOOST_CHECK_CLOSE(mean(d), static_cast<RealType>(20), tolerance);
   BOOST_CHECK_CLOSE(mode(d), static_cast<RealType>(20), tolerance);
   BOOST_CHECK_CLOSE(variance(d), static_cast<RealType>(10.821643286573146292585170340681L), tolerance);
   BOOST_CHECK_CLOSE(skewness(d), static_cast<RealType>(0.048833071022952084732902910189366L), tolerance);
   BOOST_CHECK_CLOSE(kurtosis_excess(d), static_cast<RealType>(2.5155486690782804816404001878293L), tolerance);
   BOOST_CHECK_CLOSE(kurtosis(d), kurtosis_excess(d) + 3, tolerance);
   BOOST_CHECK_EQUAL(quantile(d, 0.5f), median(d));

   BOOST_CHECK_THROW(d = boost::math::hypergeometric_distribution<RealType>(501, 40, 500), std::domain_error);
   BOOST_CHECK_THROW(d = boost::math::hypergeometric_distribution<RealType>(40, 501, 500), std::domain_error);
}


int test_main(int, char* [])
{
   expected_results();
   // Basic sanity-check spot values.
   // (Parameter value, arbitrarily zero, only communicates the floating point type).
   test_spots(0.0F, "float"); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
   test_spots(0.0, "double"); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(0.0L, "long double"); // Test long double.
   test_spots(boost::math::concepts::real_concept(0), "real_concept"); // Test real_concept.
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif

   return 0;
} // int test_main(int, char* [])

