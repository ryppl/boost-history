
#include "test.hpp"
#include "check_integral_constant.hpp"
#include TYPE_TRAITS(is_float)

TT_TEST_BEGIN(is_float)

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float const volatile>::value, true);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<double>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<double const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<double volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<double const volatile>::value, true);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<long double>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<long double const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<long double volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<long double const volatile>::value, true);

//
// cases that should not be true:
//
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<void>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<int>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<UDT>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<test_abc1>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<empty_UDT>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float*>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float&>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<const float&>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_float<float[2]>::value, false);

TT_TEST_END






