/**
 * \file test/namespaces_ct_01.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// not operator
#include <boost/mpl/assert.hpp>
// namespace registering 
#include <boost/mirror/traits/reflects_global_scope.hpp>
//
#include "./namespaces.hpp"
#include "./test.hpp"

#define BOOST_MIRROR_TEST_NAMESPACES_CT_01_MSG\
	"The meta-namespace should not reflect the global scope"

void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// define an alternative name for the global scope meta-data
	typedef BOOST_MIRRORED_GLOBAL_SCOPE() meta_gs;
	BOOST_STATIC_ASSERT( reflects_global_scope<meta_gs>::value );
	//
	// define an alternative name for the ::test namespace meta-data
	typedef BOOST_MIRRORED_NAMESPACE(::test) meta_ns_test;
	typedef BOOST_MIRRORED_NAMESPACE(::test::stuff) meta_ns_test_stuff;
	typedef BOOST_MIRRORED_NAMESPACE(::test::stuff::detail) meta_ns_test_stuff_detail;
	typedef BOOST_MIRRORED_NAMESPACE(::test::feature) meta_ns_test_feature;
	typedef BOOST_MIRRORED_NAMESPACE(::test::feature::detail) meta_ns_test_feature_detail;
	//
	BOOST_MIRROR_ASSERT_NOT(
		(reflects_global_scope<meta_ns_test>),
		BOOST_MIRROR_TEST_NAMESPACES_CT_01_MSG
	);
	BOOST_MIRROR_ASSERT_NOT(
		(reflects_global_scope<meta_ns_test_stuff>),
		BOOST_MIRROR_TEST_NAMESPACES_CT_01_MSG
	);
	BOOST_MIRROR_ASSERT_NOT(
		(reflects_global_scope<meta_ns_test_stuff_detail>),
		BOOST_MIRROR_TEST_NAMESPACES_CT_01_MSG
	);
	BOOST_MIRROR_ASSERT_NOT(
		(reflects_global_scope<meta_ns_test_feature>),
		BOOST_MIRROR_TEST_NAMESPACES_CT_01_MSG
	);
	BOOST_MIRROR_ASSERT_NOT(
		(reflects_global_scope<meta_ns_test_feature_detail>),
		BOOST_MIRROR_TEST_NAMESPACES_CT_01_MSG
	);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


