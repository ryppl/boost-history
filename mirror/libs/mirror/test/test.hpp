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

#ifdef BOOST_NO_STATIC_ASSERT
#include <boost/mpl/assert.hpp>
#endif

using namespace ::boost::unit_test;

#ifdef BOOST_NO_STATIC_ASSERT

#define BOOST_MIRROR_ASSERT(EXPR, ERROR_MSG) \
	BOOST_MPL_ASSERT(EXPR)

#define BOOST_MIRROR_ASSERT_NOT(EXPR, ERROR_MSG) \
	BOOST_MPL_ASSERT_NOT(EXPR)

#define BOOST_MIRROR_ASSERT_RELATION(LEFT, REL, RIGHT, ERROR_MSG)\
	BOOST_MPL_ASSERT_RELATION(LEFT, REL, RIGHT)

#else

#define BOOST_MIRROR_ASSERT(EXPR, ERROR_MSG) \
	static_assert(EXPR :: value, ERROR_MSG)

#define BOOST_MIRROR_ASSERT_NOT(EXPR, ERROR_MSG) \
	static_assert(!EXPR :: value, ERROR_MSG)

#define BOOST_MIRROR_ASSERT_RELATION(LEFT, REL, RIGHT, ERROR_MSG)\
	static_assert(LEFT REL RIGHT, ERROR_MSG)

#endif

#endif // include guard

