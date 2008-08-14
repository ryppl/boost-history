/**
 * \file test/type_rt_02.cpp
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
		BOOST_MIRRORED_TYPE(int *)::base_name() ==
		BOOST_CTS_LIT("int *")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int const *)::base_name() ==
		BOOST_CTS_LIT("int const *")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int const * volatile)::base_name() ==
		BOOST_CTS_LIT("int const * volatile")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int const volatile &)::base_name() ==
		BOOST_CTS_LIT("int const volatile &")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(const volatile int &)::base_name() ==
		BOOST_MIRRORED_TYPE(int const volatile &)::base_name()
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int [])::base_name() ==
		BOOST_CTS_LIT("int []")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int [123])::base_name() ==
		BOOST_CTS_LIT("int [123]")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int [123][456])::base_name() ==
		BOOST_CTS_LIT("int [123][456]")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int const [][123][456])::base_name() ==
		BOOST_CTS_LIT("int const [][123][456]")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(int ( *)())::base_name() ==
		BOOST_CTS_LIT("int ( *)()")
	);
	BOOST_CHECK(
		BOOST_MIRRORED_TYPE(
			int const volatile ( *)(int const &, int volatile *)
		)::base_name() == BOOST_CTS_LIT(
			"int const volatile ( *)(int const &, int volatile *)"
		)
	);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types run-time test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


