/**
 * \file test/types_ct_02.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL_ASSERT
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
//
#include <boost/mirror/meta_namespace.hpp>
// pre-registered types
#include <boost/mirror/meta_type.hpp>
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"

void test_main()
{
	using ::boost::is_same;
	// usage without 'using ::boost::mirror'
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPE(int)::scope, 
		BOOST_MIRRORED_GLOBAL_SCOPE()
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPE(::std::string)::scope, 
		BOOST_MIRRORED_NAMESPACE(::std)
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPE(::boost::cts::bstring)::scope, 
		BOOST_MIRRORED_NAMESPACE(::std)
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPE_NS(::boost::cts, bstring)::scope, 
		BOOST_MIRRORED_NAMESPACE(::std)
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEDEF(::boost::cts, bstring)::scope, 
		BOOST_MIRRORED_NAMESPACE(::boost::cts)
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEDEF(::boost::cts, bstring)::scope::scope, 
		BOOST_MIRRORED_NAMESPACE(::boost)
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPE(::test::feature::detail::foo_impl)::scope, 
		BOOST_MIRRORED_NAMESPACE(::test::feature::detail)
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPE(::test::feature::foo)::scope::scope, 
		BOOST_MIRRORED_NAMESPACE(::test)
	> ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types compile test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


