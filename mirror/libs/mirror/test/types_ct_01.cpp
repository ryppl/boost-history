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

#define BOOST_MIRROR_TEST_TYPES_CT_01_MSG \
	"The reflected and the real type must be the same"

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef BOOST_MIRRORED_TYPE(int) meta_int;
	typedef BOOST_MIRRORED_TYPE(::std::string) meta_std_string;
	typedef BOOST_MIRRORED_TYPE(::boost::cts::bstring) meta_boost_cts_bstring;
	typedef BOOST_MIRRORED_TYPE(::test::feature::foo) meta_test_feature_foo;
	typedef BOOST_MIRRORED_TYPE(baz) meta_baz;
	//
	typedef is_same<meta_int, meta_std_string>::type result_00;
	BOOST_MIRROR_ASSERT_NOT(
		(result_00),
		"Meta types reflecting an int and a ::std::string "\
		"should no be the same"
	);
	BOOST_MIRROR_ASSERT(
		(reflects_global_scope<meta_int::scope>),
		"The int type should be the member of the global scope"
	);
	BOOST_MIRROR_ASSERT(
		(reflects_namespace<meta_std_string::scope>),
		"The scope of ::std::string should be a namespace"
	);
	BOOST_MIRROR_ASSERT_NOT(
		(reflects_global_scope<meta_std_string::scope>),
		"The scope of ::std::string should not be the global scope"
	);
	BOOST_MIRROR_ASSERT(
		(reflects_global_scope<meta_std_string::scope::scope>),
		"::std::string should be defined in a namespace defined "\
		"directly in the global scope"
	);
	//
	typedef is_same<meta_int::reflected_type, int>::type result_01;
	BOOST_MIRROR_ASSERT((result_01), BOOST_MIRROR_TEST_TYPES_CT_01_MSG);
	//
	typedef is_same<
		meta_std_string::reflected_type, 
		::std::string
	>::type result_02;
	BOOST_MIRROR_ASSERT((result_02), BOOST_MIRROR_TEST_TYPES_CT_01_MSG);
	//
	typedef is_same<
		meta_boost_cts_bstring::reflected_type,
		::boost::cts::bstring
	>::type result_03;
	BOOST_MIRROR_ASSERT((result_03), BOOST_MIRROR_TEST_TYPES_CT_01_MSG);
	//
	typedef is_same<
                meta_test_feature_foo::reflected_type,
                ::test::feature::foo
        >::type result_04;
	BOOST_MIRROR_ASSERT((result_04), BOOST_MIRROR_TEST_TYPES_CT_01_MSG);
	//
	typedef is_same<
                meta_baz::reflected_type,
                baz
        >::type result_05;
	BOOST_MIRROR_ASSERT((result_05), BOOST_MIRROR_TEST_TYPES_CT_01_MSG);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types compile test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


