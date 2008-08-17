/**
 * \file test/types_ct_05.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing cooperation with BOOST_TYPEOF
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/or.hpp>
#include <boost/typeof/typeof.hpp>
//
#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"

int foo(short, long);

void test_main()
{
	using namespace ::boost;
	//
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(false || true)::reflected_type,
		bool
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF('c')::reflected_type,
		char
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(L'w')::reflected_type,
		wchar_t
	> ));
	BOOST_MPL_ASSERT(( mpl::or_<
		is_same<
			BOOST_MIRRORED_TYPEOF("a+b")::reflected_type,
			const char [4]
		>,
		is_same<
			BOOST_MIRRORED_TYPEOF("a+b")::reflected_type,
			char [4]
		>
	> ));
	BOOST_MPL_ASSERT(( mpl::or_<
		is_same<
			BOOST_MIRRORED_TYPEOF(L"v+w")::reflected_type,
			const wchar_t [4]
		>,
		is_same<
			BOOST_MIRRORED_TYPEOF(L"v+w")::reflected_type,
			wchar_t [4]
		>
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(1+2)::reflected_type,
		int
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(3L + 4L)::reflected_type,
		long
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(5.0f + 6.0f)::reflected_type,
		float
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(7.0 + 8.0)::reflected_type,
		double
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(::std::string("x+y"))::reflected_type,
		::std::string
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(::std::wstring(L"x+y"))::reflected_type,
		::std::wstring
	> ));
	//
	const int i = 10;
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(&i)::reflected_type,
		const int *
	> ));
	BOOST_MPL_ASSERT(( is_same<
		BOOST_MIRRORED_TYPEOF(&foo)::reflected_type,
		int (*)(short, long)
	> ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types compile test 05");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


