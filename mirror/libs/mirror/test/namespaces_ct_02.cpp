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
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_) meta_ns_;
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test) meta_ns_test;
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff) meta_ns_test_stuff;
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff_detail) meta_ns_test_stuff_detail;
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test_feature) meta_ns_test_feature;
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test_feature_detail) meta_ns_test_feature_detail;
	//
	//
	BOOST_STATIC_ASSERT( reflects_namespace<meta_ns_>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<meta_ns_test>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<meta_ns_test_stuff>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<meta_ns_test_stuff_detail>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<meta_ns_test_feature>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<meta_ns_test_feature_detail>::value );
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


