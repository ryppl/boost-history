/**
 * \file test/classes_ct_04.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Tests the accumulate algorithm on meta_class attributes
 * and checks the attribute types.
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL_ASSERT
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/transform_view.hpp>
//
#include <boost/type_traits/add_pointer.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/accumulate.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"

template <typename MetaAttribute>
struct get_attrib_type
{
	typedef typename 
		MetaAttribute::
		type::
		reflected_type type;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	::test::H h;
	typedef BOOST_MIRRORED_CLASS(::test::H) meta_H;
	// define a forward op
	typedef mpl::lambda<mpl::push_back<
			mpl::_1, 
			add_pointer<
				get_attrib_type<mpl::_2>
			>
	> >::type accumulate_op;
	//
	// use the accumulate algorithm to get the typelist
	typedef accumulate<
		meta_H::all_attributes,
		mpl::vector0<>,
		accumulate_op
	>::type attrib_types_1;
	//
	// use the typelist from the meta_attribs
	typedef mpl::transform_view<
		meta_H::all_attributes::type_list,
		::boost::add_pointer<mpl::_1>
	>::type attrib_types_2;
	//
	// make a 'hand-made'  list
	typedef mpl::vector<
		BOOST_TYPEOF(&h.a1),
		BOOST_TYPEOF(&h.a2),
		BOOST_TYPEOF(&h.a3),
		BOOST_TYPEOF(&h.b1),
		BOOST_TYPEOF(&h.b2),
		BOOST_TYPEOF(&h.c1),
		BOOST_TYPEOF(&h.c2),
		BOOST_TYPEOF(&h.d1),
		BOOST_TYPEOF(&h.d2),
		BOOST_TYPEOF(&h.e),
		BOOST_TYPEOF(&h.f1),
		BOOST_TYPEOF(&h.f2),
		BOOST_TYPEOF(&h.g1),
		BOOST_TYPEOF(&h.g2),
		BOOST_TYPEOF(&h.g3),
		BOOST_TYPEOF(&h.h1),
		BOOST_TYPEOF(&h.h2),
		BOOST_TYPEOF(&h.h3)
	> attrib_types_3;
	//
	// and compare them
	BOOST_MPL_ASSERT(( mpl::equal<attrib_types_1, attrib_types_2> ));
	BOOST_MPL_ASSERT(( mpl::equal<attrib_types_2, attrib_types_3> ));
	BOOST_MPL_ASSERT(( mpl::equal<attrib_types_1, attrib_types_3> ));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 04");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


