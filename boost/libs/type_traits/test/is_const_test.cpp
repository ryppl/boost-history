
#include "test.hpp"
#include "check_integral_constant.hpp"
#include TYPE_TRAITS(is_const)

TT_TEST_BEGIN(is_const)

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<void>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<const void>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<test_abc1>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<const test_abc1>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<int>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<const int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<const UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<const volatile UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<const int&>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_const<cr_type>::value, false);

TT_TEST_END






