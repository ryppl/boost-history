/**
 * \file test/classes_ct_03.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * this test focuses (indirectly) on the registering,
 * on the count of member attributes, the size algorithm
 * and cooperation with MPL.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL_ASSERT
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/intrinsic/at.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"


void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	::test::H h;
	typedef BOOST_MIRRORED_CLASS(::test::H) meta_H;
	//
	//
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<0> >::type::type *,
		BOOST_TYPEOF(&h.a1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<1> >::type::type *,
		BOOST_TYPEOF(&h.a2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<2> >::type::type *,
		BOOST_TYPEOF(&h.a3)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<3> >::type::type *,
		BOOST_TYPEOF(&h.b1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<4> >::type::type *,
		BOOST_TYPEOF(&h.b2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<5> >::type::type *,
		BOOST_TYPEOF(&h.c1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<6> >::type::type *,
		BOOST_TYPEOF(&h.c2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<7> >::type::type *,
		BOOST_TYPEOF(&h.d1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<8> >::type::type *,
		BOOST_TYPEOF(&h.d2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<9> >::type::type *,
		BOOST_TYPEOF(&h.e)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<10> >::type::type *,
		BOOST_TYPEOF(&h.f1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<11> >::type::type *,
		BOOST_TYPEOF(&h.f2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<12> >::type::type *,
		BOOST_TYPEOF(&h.g1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<13> >::type::type *,
		BOOST_TYPEOF(&h.g2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<14> >::type::type *,
		BOOST_TYPEOF(&h.g3)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<15> >::type::type *,
		BOOST_TYPEOF(&h.h1)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<16> >::type::type *,
		BOOST_TYPEOF(&h.h2)
	> ));
	BOOST_MPL_ASSERT(( is_same< 
		at<meta_H::all_attributes, mpl::int_<17> >::type::type *,
		BOOST_TYPEOF(&h.h3)
	> ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 03");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


