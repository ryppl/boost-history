/**
 * \file test/namespaces_ct_03.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// namespace registering 
#include <boost/mirror/traits/reflects_namespace.hpp>
//
#include "./namespaces.hpp"
#include "./test.hpp"

void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRRORED_GLOBAL_SCOPE()
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRRORED_NAMESPACE(::test)
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRRORED_NAMESPACE(::test::stuff)
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRRORED_NAMESPACE(::test::stuff::detail)
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRRORED_NAMESPACE(::test::feature)
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRRORED_NAMESPACE(::test::feature::detail)
	>::value );
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 03");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


