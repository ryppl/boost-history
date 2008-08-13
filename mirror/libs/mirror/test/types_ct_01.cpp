/**
 * \file test/types_ct_01.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// not operator
#include <boost/mpl/assert.hpp>
// namespace registering 
#include <boost/mirror/traits/reflects_global_scope.hpp>
#include <boost/mirror/traits/reflects_namespace.hpp>
#include <boost/mirror/traits/reflects_type.hpp>
//
// pre-registered types
#include <boost/mirror/meta_type.hpp>
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"

void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef BOOST_MIRRORED_TYPE(int) meta_int;
	typedef BOOST_MIRRORED_TYPE(::std::string) meta_std_string;
	typedef BOOST_MIRRORED_TYPE(::boost::cts::bstring) meta_boost_cts_bstring;
	typedef BOOST_MIRRORED_TYPE(::test::feature::foo) meta_test_feature_foo;
	typedef BOOST_MIRRORED_TYPE(baz) meta_baz;
	//
	BOOST_MPL_ASSERT_NOT(( is_same<meta_int, meta_std_string> ));
	BOOST_MPL_ASSERT(( reflects_global_scope<meta_int::scope> ));
	BOOST_MPL_ASSERT(( reflects_namespace<meta_std_string::scope > ));
	BOOST_MPL_ASSERT_NOT(( reflects_global_scope<meta_std_string::scope> ));
	BOOST_MPL_ASSERT(( reflects_global_scope<meta_std_string::scope::scope> ));
	//
	BOOST_MPL_ASSERT(( is_same<meta_int::reflected_type, int> ));
	BOOST_MPL_ASSERT(( is_same<meta_std_string::reflected_type, ::std::string> ));
	BOOST_MPL_ASSERT(( is_same<
		meta_boost_cts_bstring::reflected_type, 
		::boost::cts::bstring
	> ));
	BOOST_MPL_ASSERT(( is_same<
		meta_test_feature_foo::reflected_type, 
		::test::feature::foo
	> ));
	BOOST_MPL_ASSERT(( is_same<
		meta_baz::reflected_type, 
		baz
	> ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types compile test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


