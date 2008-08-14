/**
 * \file test/types_ct_03.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing cooperation between mirror and mpl.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
//
#include <boost/mirror/traits/reflects_global_scope.hpp>
#include <boost/mirror/meta_namespace.hpp>
// pre-registered types
#include <boost/mirror/meta_type.hpp>
//
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"

template <typename MetaType>
struct get_scope
{
	typedef typename MetaType::scope type;
};

template <typename MetaType>
struct get_reflected_type
{
	typedef typename MetaType::reflected_type type;
};

void test_main()
{
	using namespace ::boost;

	typedef mpl::vector<
		bool, 
		char, 
		short, 
		unsigned short,
		int,
		unsigned int,
		long,
		float,
		double
	> some_types;

	BOOST_MPL_ASSERT(( mpl::accumulate<
		some_types,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			mirror::reflects_global_scope<
				get_scope<
					BOOST_MIRRORED_TYPE(mpl::_2)
				>
			>
		>
	>::type ));

	BOOST_MPL_ASSERT(( mpl::accumulate<
		some_types,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			is_same<
				get_reflected_type<BOOST_MIRRORED_TYPE(mpl::_2)>,
				mpl::_2
			>
		>
	>::type ));

	typedef mpl::vector<
		::std::string,
		::std::wstring,
		BOOST_MIRROR_TYPEDEF(::boost::cts, bstring)
	> other_types;

	BOOST_MPL_ASSERT_NOT(( mpl::accumulate<
		other_types,
		mpl::false_,
		mpl::or_<
			mpl::_1,
			mirror::reflects_global_scope<
				get_scope<
					BOOST_MIRRORED_TYPE(mpl::_2)
				>
			>
		>
	>::type ));

}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types compile test 03");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


