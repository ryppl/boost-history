/**
 * \file test/classes_ct_10.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 *  Testing the by_name metafunction
 *
 *  This test focuses on the testing of the by_name intrinsic
 *  meta-function with meta-class-attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL
#include <boost/mpl/assert.hpp>
//
#include <boost/type_traits/is_same.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/intrinsic/by_name.hpp>
#include <boost/mirror/intrinsic/at.hpp>
//
#include "./simple_classes.hpp"
#include "./test.hpp"

template <class T1, class T2>
void test_type_of_attribs(T1*, T2*)
{
	typedef typename ::boost::is_same<T1, T2>::type 
		types_are_same;
	
	BOOST_MIRROR_ASSERT(
		(types_are_same),
		"The attributes should have the same types"
	);
}

template <class T>
void test_type_of_attribs(T*, T*) { }

template <class MetaAttribute1, class MetaAttribute2>
void test_attribs(void)
{
	test_type_of_attribs(
		(typename MetaAttribute1::type::reflected_type*)0,
		(typename MetaAttribute2::type::reflected_type*)0
	);
}

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef BOOST_MIRRORED_CLASS(::test::A) meta_A;
	typedef BOOST_MIRRORED_CLASS(::test::B) meta_B;
	typedef BOOST_MIRRORED_CLASS(::test::C) meta_C;
	typedef BOOST_MIRRORED_CLASS(::test::D) meta_D;
	typedef BOOST_MIRRORED_CLASS(::test::E) meta_E;
	typedef BOOST_MIRRORED_CLASS(::test::F) meta_F;
	typedef BOOST_MIRRORED_CLASS(::test::G) meta_G;
	typedef BOOST_MIRRORED_CLASS(::test::H) meta_H;
	//
	test_attribs<
		by_name<meta_A::attributes>::a,
		at<meta_A::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_B::attributes>::b,
		at<meta_B::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_C::attributes>::c,
		at<meta_C::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_D::attributes>::d,
		at<meta_D::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_E::attributes>::e,
		at<meta_E::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_F::attributes>::f,
		at<meta_F::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_G::attributes>::g,
		at<meta_G::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_H::attributes>::h,
		at<meta_H::attributes, mpl::int_<0> >::type
	>();
	//
	test_attribs<
		by_name<meta_H::all_attributes>::a,
		at<meta_H::all_attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::b,
		at<meta_H::all_attributes, mpl::int_<1> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::c,
		at<meta_H::all_attributes, mpl::int_<2> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::d,
		at<meta_H::all_attributes, mpl::int_<3> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::e,
		at<meta_H::all_attributes, mpl::int_<4> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::f,
		at<meta_H::all_attributes, mpl::int_<5> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::g,
		at<meta_H::all_attributes, mpl::int_<6> >::type
	>();
	test_attribs<
		by_name<meta_H::all_attributes>::h,
		at<meta_H::all_attributes, mpl::int_<7> >::type
	>();
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 10");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


