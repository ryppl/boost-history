/**
 * \file test/namespaces_ct_02.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mpl/assert.hpp>
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
	// define an alternative name for the global scope meta-data
	typedef BOOST_MIRRORED_GLOBAL_SCOPE() meta_gs;
	typedef BOOST_MIRRORED_NAMESPACE(::test) meta_ns_test;
	typedef BOOST_MIRRORED_NAMESPACE(::test::stuff) meta_ns_test_stuff;
	typedef BOOST_MIRRORED_NAMESPACE(::test::stuff::detail) meta_ns_test_stuff_detail;
	typedef BOOST_MIRRORED_NAMESPACE(::test::feature) meta_ns_test_feature;
	typedef BOOST_MIRRORED_NAMESPACE(::test::feature::detail) meta_ns_test_feature_detail;
	//
	//
	BOOST_MPL_ASSERT(( reflects_namespace<meta_gs> ));
	BOOST_MPL_ASSERT(( reflects_namespace<meta_ns_test> ));
	BOOST_MPL_ASSERT(( reflects_namespace<meta_ns_test_stuff> ));
	BOOST_MPL_ASSERT(( reflects_namespace<meta_ns_test_stuff_detail> ));
	BOOST_MPL_ASSERT(( reflects_namespace<meta_ns_test_feature> ));
	BOOST_MPL_ASSERT(( reflects_namespace<meta_ns_test_feature_detail> ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


