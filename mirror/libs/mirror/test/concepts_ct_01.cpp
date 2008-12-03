/**
 * \file test/concepts_ct_01.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_namespaces/boost/tuples.hpp>
#include <boost/mirror/meta_namespaces/boost/fusion.hpp>
#include <boost/mirror/concept.hpp>
//
#include <boost/mpl/accumulate.hpp>
//
#include "./namespaces.hpp"
#include "./test.hpp"


template <class X>
struct test_MetaNamespace
{
	typedef typename ::boost::mpl::and_<
		typename ::boost::mirror::is_MetaNamespace<X>::type,
		typename ::boost::mirror::is_MetaScope<typename X::scope>::type
	>::type type;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// make a list of some meta-namespaces
	typedef mpl::vector5<
		BOOST_MIRRORED_NAMESPACE(::std),
		BOOST_MIRRORED_NAMESPACE(::boost),
		BOOST_MIRRORED_NAMESPACE(::boost::tuples),
		BOOST_MIRRORED_NAMESPACE(::boost::fusion),
		BOOST_MIRRORED_NAMESPACE(::boost::mirror)
	> some_meta_namespaces;
	//
	// test their compliance with the MetaType concept
	typedef mpl::accumulate<
		some_meta_namespaces,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			test_MetaNamespace<mpl::_2>
		>
	>::type all_passed;
	//
	BOOST_MPL_ASSERT(( all_passed ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: concept compliance test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


