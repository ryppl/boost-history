/**
 * \file test/type_rt_01.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 * Simple tests of runtime features of the meta_type template.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mirror/traits/reflects_global_scope.hpp>
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"


void test_main()
{
	// 
	// base name getters for native C++ type and std::string
	//
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int)::base_name() ==
		BOOST_CTS_LIT("int")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::std::string)::base_name() ==
		BOOST_CTS_LIT("string")
	);
	//
	// base and full name getter
	// 
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::test::feature::foo)::base_name() ==
		BOOST_CTS_LIT("foo")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::test::feature::foo)::full_name() ==
		BOOST_CTS_LIT("::test::feature::foo")
	);
	//
	// scope full name getter
	//
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(
			::test::feature::detail::foo_impl
		)::scope::full_name() ==
		BOOST_CTS_LIT("::test::feature::detail")
	);
	//
	// difference between a typedef and it's source type
	//
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(
			::test::foobar
		)::full_name() ==
		BOOST_CTS_LIT("::test::feature::foo")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE_NS(
			::test, foobar
		)::full_name() ==
		BOOST_CTS_LIT("::test::feature::foo")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPEDEF(
			::test, foobar
		)::full_name() ==
		BOOST_CTS_LIT("::test::foobar")
	);
	//
	// the generic type name getter
	//
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::test::feature::detail::foo_impl)::get_name(
			::boost::mpl::false_(),
			::std::char_traits<char>() 
		) == "foo_impl"
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::test::feature::detail::foo_impl)::get_name(
			::boost::mpl::false_(),
			::std::char_traits<wchar_t>() 
		) == L"foo_impl"
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::test::feature::foo)::get_name(
			::boost::mpl::true_(),
			::std::char_traits<char>() 
		) == "::test::feature::foo"
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(::test::feature::foo)::get_name(
			::boost::mpl::true_(),
			::std::char_traits<wchar_t>() 
		) == L"::test::feature::foo"
	);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types run-time test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


