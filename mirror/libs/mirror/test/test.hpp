/**
 * \file test/test.hpp
 *
 * This file is part of the Mirror library testsuite.
 * Common definitions.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_MIRROR_LIBS_MIRROR_TEST_TEST_HPP
#define BOOST_MIRROR_LIBS_MIRROR_TEST_TEST_HPP

#include <boost/test/unit_test.hpp>
#include <boost/config.hpp>

#define BOOST_MIRROR_ASSERT_USES_STATIC_ASSERT

#ifdef BOOST_NO_STATIC_ASSERT
#ifdef BOOST_MIRROR_ASSERT_USES_STATIC_ASSERT
#undef BOOST_MIRROR_ASSERT_USES_STATIC_ASSERT
#endif
#endif

#ifndef BOOST_MIRROR_ASSERT_USES_STATIC_ASSERT
#include <boost/mpl/assert.hpp>
#endif

using namespace ::boost::unit_test;

namespace boost {
namespace mirror {
namespace test {

#ifdef BOOST_MIRROR_ASSERT_USES_STATIC_ASSERT

template <bool Result>
struct assertion_result;

template <>
struct assertion_result<false>
{
	char x;
};

template <>
struct assertion_result<true>
{
	double x;
};

typedef assertion_result<false> assertion_failed;
typedef assertion_result<true>  assertion_passed;

static_assert(
	sizeof(assertion_passed) != sizeof(assertion_failed),
	"Internal error in the implementation of MIRROR_ASSERT"
);

template <class IntegralConstant>
assertion_result<IntegralConstant::value>
get_assertion_result(void (*) (IntegralConstant));

#define BOOST_MIRROR_ASSERT_RESULT(EXPR, RESULT, ERROR_MSG) \
	static_assert( \
		sizeof(::boost::mirror::test::get_assertion_result(\
			( void (*) EXPR ) 0  \
		)) == \
		sizeof(::boost::mirror::test::assertion_result<RESULT>), \
		ERROR_MSG \
	)

#define BOOST_MIRROR_ASSERT(EXPR, ERROR_MSG) \
	BOOST_MIRROR_ASSERT_RESULT( \
		EXPR,  \
		true, \
		ERROR_MSG \
	)

#define BOOST_MIRROR_ASSERT_NOT(EXPR, ERROR_MSG) \
	BOOST_MIRROR_ASSERT_RESULT( \
		EXPR,  \
		false, \
		ERROR_MSG \
	)

#define BOOST_MIRROR_ASSERT_RELATION(LEFT, REL, RIGHT, ERROR_MSG)\
	static_assert(LEFT REL RIGHT, ERROR_MSG)

#else

#define BOOST_MIRROR_ASSERT(EXPR, ERROR_MSG) \
	BOOST_MPL_ASSERT(EXPR)

#define BOOST_MIRROR_ASSERT_NOT(EXPR, ERROR_MSG) \
	BOOST_MPL_ASSERT_NOT(EXPR)

#define BOOST_MIRROR_ASSERT_RELATION(LEFT, REL, RIGHT, ERROR_MSG)\
	BOOST_MPL_ASSERT_RELATION(LEFT, REL, RIGHT)

#endif

} // namespace test
} // namespace mirror
} // namespace boost

#endif // include guard

