/**
 * \file test/classes_ct_01.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * this test focuses (indirectly) on the registering,
 * inheritance of meta_class from meta_type, traits
 * and cooperation with MPL
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
#include <boost/type_traits/is_same.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/traits/reflects_global_scope.hpp>
#include <boost/mirror/traits/reflects_namespace.hpp>
#include <boost/mirror/traits/reflects_class.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"

template <typename MetaObject>
struct get_scope
{
	typedef typename MetaObject::scope type;
};

template <typename MetaType>
struct get_reflected_type
{
	typedef typename MetaType::reflected_type type;
};


void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef mpl::vector<
		::test::A,
		::test::B,
		::test::C,
		::test::D,
		::test::E,
		::test::F,
		::test::G,
		::test::H,
		::test::I,
		::test::J,
		::test::K
	> some_classes;

	// none of the classes is defined in the global scope
	typedef mpl::accumulate<
		some_classes,
		mpl::false_,
		mpl::or_<
			mpl::_1,
			mirror::reflects_global_scope<
				get_scope<
					BOOST_MIRRORED_CLASS(mpl::_2)
				>
			>
		>
	>::type result_01;
	BOOST_MIRROR_ASSERT_NOT(
		result_01,
		"No class in list should be from the global scope"
	);

	// all off the classes are defined in a namespace defined
	// (directly) under the global scope
	typedef mpl::accumulate<
		some_classes,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			mirror::reflects_namespace<
				get_scope<
					BOOST_MIRRORED_CLASS(mpl::_2)
				>
			>
		>
	>::type result_02;
	BOOST_MIRROR_ASSERT(
		result_02,
		"All classes in list should be from a named namespace"
	);

	typedef mpl::accumulate<
		some_classes,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			mirror::reflects_global_scope<
				get_scope<get_scope<
					BOOST_MIRRORED_CLASS(mpl::_2)
				> >
			>
		>
	>::type result_03;
	BOOST_MIRROR_ASSERT(
		result_03, 
		"All classes in list should be from a not-nested namespace"
	);

	typedef mpl::accumulate<
		some_classes,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			mirror::reflects_class<
				BOOST_MIRRORED_CLASS(mpl::_2)
			>
		>
	>::type result_04;
	BOOST_MIRROR_ASSERT(
		result_04,
		"Meta-classes for classes in list should be recognized"
	);

	typedef mpl::accumulate<
		some_classes,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			::boost::is_same<
				get_reflected_type<BOOST_MIRRORED_CLASS(mpl::_2)>,
				mpl::_2
			>
		>
	>::type result_05;
	BOOST_MIRROR_ASSERT(
		result_05, 
		"Base-level type and reflected type must be the same"
	);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


