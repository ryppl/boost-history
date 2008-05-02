/**
 * \file test/namespaces_ct_04.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp> 
// namespace registering 
#include <boost/mirror/traits/reflects_namespace.hpp>
// base meta-types
#include <boost/mirror/meta_type.hpp>
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
		BOOST_MIRROR_REFLECT_TYPE(bool) :: scope
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRROR_REFLECT_TYPE(char) :: scope
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRROR_REFLECT_TYPE(int) :: scope
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRROR_REFLECT_TYPE(double) :: scope
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRROR_REFLECT_TYPE(void*) :: scope
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRROR_REFLECT_TYPE(int const * const *) :: scope
	>::value );
	BOOST_STATIC_ASSERT( reflects_namespace<
		BOOST_MIRROR_REFLECT_TYPE(char volatile * const &) :: scope
	>::value );
	//
	BOOST_STATIC_ASSERT(( is_same<
		BOOST_MIRROR_REFLECT_TYPE(const char* []) :: scope,
		BOOST_MIRROR_REFLECT_TYPE(int volatile &) :: scope
	>::value ));

	BOOST_STATIC_ASSERT(( is_same<
		BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, bchar) :: scope,
		BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, bstring) :: scope
	>::value ));
	BOOST_STATIC_ASSERT(( mpl::not_<is_same<
		BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, bchar) :: scope,
		BOOST_MIRROR_REFLECT_TYPE(char) :: scope
	> >::value ));
	BOOST_STATIC_ASSERT(( mpl::not_<is_same<
		BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, bchar) :: scope,
		BOOST_MIRROR_REFLECT_TYPE(wchar_t) :: scope
	> >::value ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 04");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


