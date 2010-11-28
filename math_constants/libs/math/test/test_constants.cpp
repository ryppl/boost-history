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
#include <boost/static_assert.hpp>

BOOST_STATIC_ASSERT((boost::is_same<boost::math::constants::construction_traits<float, boost::math::policies::policy<> >::type, boost::mpl::int_<boost::math::constants::construct_from_float> >::value));
BOOST_STATIC_ASSERT((boost::is_same<boost::math::constants::construction_traits<double, boost::math::policies::policy<> >::type, boost::mpl::int_<boost::math::constants::construct_from_double> >::value));
BOOST_STATIC_ASSERT((boost::is_same<boost::math::constants::construction_traits<long double, boost::math::policies::policy<> >::type, boost::mpl::int_<(sizeof(double) == sizeof(long double) ? boost::math::constants::construct_from_double : boost::math::constants::construct_from_long_double)> >::value));
BOOST_STATIC_ASSERT((boost::is_same<boost::math::constants::construction_traits<boost::math::concepts::real_concept, boost::math::policies::policy<> >::type, boost::mpl::int_<0> >::value));

typedef boost::math::policies::policy<boost::math::policies::digits2<LDBL_MANT_DIG> > real_concept_policy_1;
typedef boost::math::policies::policy<boost::math::policies::digits2<LDBL_MANT_DIG + 2> > real_concept_policy_2;

BOOST_STATIC_ASSERT((boost::is_same<boost::math::constants::construction_traits<boost::math::concepts::real_concept, real_concept_policy_1 >::type, boost::mpl::int_<(sizeof(double) == sizeof(long double) ? boost::math::constants::construct_from_double : boost::math::constants::construct_from_long_double) > >::value));
BOOST_STATIC_ASSERT((boost::is_same<boost::math::constants::construction_traits<boost::math::concepts::real_concept, real_concept_policy_2 >::type, boost::mpl::int_<boost::math::constants::construct_from_string> >::value));

//
// We need to declare a conceptual type whose precision is unknown at
// compile time, and is so enormous when checked at runtime, that we're
// forced to calculate the values of the constants ourselves.
//
namespace boost{ namespace math{ namespace concepts{

class big_real_concept : public real_concept
{
public:
   big_real_concept() {}
   template <class T>
   big_real_concept(const T& t) : real_concept(t) {}
};

}
namespace tools{

template <>
inline int digits<concepts::big_real_concept>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return 2 * boost::math::constants::max_string_digits;
}

}}}

template <class RealType>
void test_spots(RealType)
{
   // Basic sanity checks for constants.
   //
   // Actual tolerance is never really smaller than epsilon for long double, even if some of our
   // test types pretend otherwise:
   //
   RealType tolerance = std::max(static_cast<RealType>(boost::math::tools::epsilon<long double>()), boost::math::tools::epsilon<RealType>()) * 2;  // double
   std::cout << "Tolerance for type " << typeid(RealType).name()  << " is " << tolerance << "." << std::endl;

   //typedef typename boost::math::policies::precision<RealType, boost::math::policies::policy<> >::type t1;

   //std::cout << "Precision for type " << typeid(RealType).name()  << " is " << t1::value << "." << std::endl;

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
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(sqrt(2.0L)), root_two<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(log(2.0L)), ln_two<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(log(log(2.0L))), ln_ln_two<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(1)/3, third<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(2)/3, twothirds<RealType>(), tolerance); 
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(pow((4 - 3.14159265358979323846264338327950288419716939937510L), 1.5L)), pow23_four_minus_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(exp(-0.5L)), exp_minus_half<RealType>(), tolerance); 
#else
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(pow((4 - 3.14159265358979323846264338327950288419716939937510), 1.5)), pow23_four_minus_pi<RealType>(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(exp(-0.5)), exp_minus_half<RealType>(), tolerance); 
#endif

   //
   // Last of all come the test cases that behave differently if we're calculating the constants on the fly:
   //
   if(boost::math::tools::digits<RealType>() > boost::math::constants::max_string_digits)
   {
      // We have no implementation for eulers constant / takes too long to calculate.
      BOOST_CHECK_THROW(euler<RealType>(), std::runtime_error);
      // This suffers from cancelation error:
      BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(4. - 3.14159265358979323846264338327950288419716939937510L), four_minus_pi<RealType>(), tolerance * 3); 
      BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(0.14159265358979323846264338327950288419716939937510L), pi_minus_three<RealType>(), tolerance * 3); 
   }
   else
   {
      BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(0.57721566490153286060651209008240243104259335L), euler<RealType>(), tolerance); 
      BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(4. - 3.14159265358979323846264338327950288419716939937510L), four_minus_pi<RealType>(), tolerance); 
      BOOST_CHECK_CLOSE_FRACTION(static_cast<RealType>(0.14159265358979323846264338327950288419716939937510L), pi_minus_three<RealType>(), tolerance); 
   }
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

template <class Policy>
void test_real_concept_policy(const Policy&)
{
   // Basic sanity checks for constants.

   boost::math::concepts::real_concept tolerance = boost::math::tools::epsilon<boost::math::concepts::real_concept>() * 2;  // double
   std::cout << "Tolerance for type " << typeid(boost::math::concepts::real_concept).name()  << " is " << tolerance << "." << std::endl;

   //typedef typename boost::math::policies::precision<boost::math::concepts::real_concept, boost::math::policies::policy<> >::type t1;

   //std::cout << "Precision for type " << typeid(boost::math::concepts::real_concept).name()  << " is " << t1::value << "." << std::endl;

   using namespace boost::math::constants;
   using namespace std; // Help ADL of std exp, log...
   using std::exp;

   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(3.14159265358979323846264338327950288419716939937510L), (pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(sqrt(3.14159265358979323846264338327950288419716939937510L)), (root_pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(sqrt(3.14159265358979323846264338327950288419716939937510L/2)), (root_half_pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(sqrt(3.14159265358979323846264338327950288419716939937510L * 2)), (root_two_pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(sqrt(log(4.0L))), (root_ln_four<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(2.71828182845904523536028747135266249775724709369995L), (e<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(0.5), (half<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(0.57721566490153286060651209008240243104259335L), (euler<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(sqrt(2.0L)), (root_two<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(log(2.0L)), (ln_two<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(log(log(2.0L))), (ln_ln_two<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(1)/3, (third<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(2)/3, (twothirds<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(0.14159265358979323846264338327950288419716939937510L), (pi_minus_three<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(4. - 3.14159265358979323846264338327950288419716939937510L), (four_minus_pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(pow((4 - 3.14159265358979323846264338327950288419716939937510L), 1.5L)), (pow23_four_minus_pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(exp(-0.5L)), (exp_minus_half<boost::math::concepts::real_concept, Policy>)(), tolerance); 
#else
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(pow((4 - 3.14159265358979323846264338327950288419716939937510), 1.5)), (pow23_four_minus_pi<boost::math::concepts::real_concept, Policy>)(), tolerance); 
   BOOST_CHECK_CLOSE_FRACTION(static_cast<boost::math::concepts::real_concept>(exp(-0.5)), (exp_minus_half<boost::math::concepts::real_concept, Policy>)(), tolerance); 
#endif

} // template <class boost::math::concepts::real_concept>void test_spots(boost::math::concepts::real_concept)

int test_main(int, char* [])
{
   // Basic sanity-check spot values.

   test_float_spots();
   test_double_spots();
   test_long_double_spots();

   test_real_concept_policy(real_concept_policy_1());
   test_real_concept_policy(real_concept_policy_2());
   test_real_concept_policy(boost::math::policies::policy<>());

   // (Parameter value, arbitrarily zero, only communicates the floating point type).
   test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
   test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(0.0L); // Test long double.
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0582))
   test_spots(boost::math::concepts::real_concept(0.)); // Test real concept.
   test_spots(boost::math::concepts::big_real_concept(0.)); // Test real concept.
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

