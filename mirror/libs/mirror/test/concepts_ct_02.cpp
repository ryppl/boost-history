/**
 * \file test/concepts_ct_02.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_types/boost/optional.hpp>
#include <boost/mirror/meta_types/boost/any.hpp>
#include <boost/mirror/meta_types/boost/variant.hpp>
#include <boost/mirror/concept.hpp>
//
#include <boost/mpl/accumulate.hpp>
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"


template <class T>
struct reflect_type
{
	typedef BOOST_MIRRORED_TYPE(T) type;
};

template <class X>
struct test_MetaType
{
	typedef typename ::boost::mpl::and_<
		typename ::boost::mirror::is_MetaType<X>::type,
		typename ::boost::mirror::is_MetaScope<typename X::scope>::type
	>::type type;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
        typedef ::test::feature::detail::foo_impl X;
        typedef BOOST_MIRROR_TYPEDEF(::test, foobar) Y;
        typedef BOOST_MIRROR_TYPEDEF(::boost::cts, bstring) W;
        typedef ::boost::optional< ::boost::any> Z;
        typedef ::boost::variant< X, Y, W, Z> V;
        typedef X * const T1;
        typedef Y volatile * const T2;
        typedef const volatile W T3;
        typedef Z const * const T4[][1][2][3][4][5][6][7][8][9];
        typedef T1& (T5)(T2, T3, T4);
        typedef T5* (T6)(T5*, T5&, V);
	typedef T6* T;
	//
	// make a list of some types
	typedef mpl::vector16<
		void,
		bool,
		char,
		wchar_t,
		short,
		int,
		long,
		float,
		double,
		::std::string,
		::std::wstring,
		::boost::cts::bstring,
		::test::feature::foo,
		::test::bar,
		baz,
		T
	> some_types;
	//
	// reflect them using mirror and put them 
	// into a new list
	typedef mpl::accumulate<
		some_types,
		mpl::vector0<>,
		mpl::push_back<
			mpl::_1,
			reflect_type<mpl::_2>
		>
	>::type some_meta_types;
	//
	// test their compliance with the MetaType concept
	typedef mpl::accumulate<
		some_meta_types,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			test_MetaType<mpl::_2>
		>
	>::type all_passed;
	//
	BOOST_MPL_ASSERT(( all_passed ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: concept compliance test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


