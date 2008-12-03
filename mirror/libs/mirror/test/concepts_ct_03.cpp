/**
 * \file test/concepts_ct_03.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/std/pair.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
#include <boost/mirror/concept.hpp>
//
#include <boost/mpl/accumulate.hpp>
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"


template <class T>
struct reflect_class
{
	typedef BOOST_MIRRORED_CLASS(T) type;
};

template <class X>
struct test_MetaClass
{
	typedef typename ::boost::mpl::and_<
		typename ::boost::mirror::is_MetaClass<X>::type,
		typename ::boost::mirror::is_MetaScope<typename X::scope>::type
	>::type type;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef ::std::pair< int, ::std::string> T1;
	typedef ::std::pair< ::std::string, T1> T2;
	typedef ::std::pair< T1, T2> T3;
	typedef ::boost::tuple< const T1*, volatile T2*, T3*> T4;
	// make a list of some classes
	typedef mpl::vector4<
		T1, T2, T3, T4 
	> some_classes;
	//
	// reflect them using mirror and put them 
	// into a new list
	typedef mpl::accumulate<
		some_classes,
		mpl::vector0<>,
		mpl::push_back<
			mpl::_1,
			reflect_class<mpl::_2>
		>
	>::type some_meta_classs;
	//
	// test their compliance with the MetaType concept
	typedef mpl::accumulate<
		some_meta_classs,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			test_MetaClass<mpl::_2>
		>
	>::type all_passed;
	//
	BOOST_MPL_ASSERT(( all_passed ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: concept compliance test 03");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


