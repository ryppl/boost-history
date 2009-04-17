/**
 * \file test/classes_ct_05.cpp
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
#include <boost/preprocessor/repetition/enum.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_types/boost/mpl/int.hpp>
#include <boost/mirror/algorithm/accumulate.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
//
#include "./test.hpp"

#define BOOST_MIRROR_TEST_CL_CT_05_MPL_INT(Z, I, DATA) mpl::int_< I >

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
	typedef tuples::tuple<
		BOOST_PP_ENUM(9, BOOST_MIRROR_TEST_CL_CT_05_MPL_INT, 0)
	> T;
	T t;
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	//
	//
	// define a forward op
	typedef mpl::lambda<
		mpl::push_back<
			mpl::_1, 
			get_attrib_type<mpl::_2>
		> 
	>::type accumulate_op;
	//
	// use the accumulate algorithm to get the typelist
	typedef accumulate<
		meta_T::attributes,
		mpl::vector0<>,
		accumulate_op
	>::type attrib_types_1;
	//
	// use the typelist from the meta_attribs
	typedef meta_T::attributes::type_list
		attrib_types_2;
	//
	// make a 'hand-made'  list
	typedef mpl::vector<
		BOOST_PP_ENUM(9, BOOST_MIRROR_TEST_CL_CT_05_MPL_INT, 0)
	> attrib_types_3;
	//
	// and compare them
        typedef mpl::equal<attrib_types_1, attrib_types_2>::type result_01;
        typedef mpl::equal<attrib_types_2, attrib_types_3>::type result_02;
        typedef mpl::equal<attrib_types_1, attrib_types_3>::type result_03;
        BOOST_MIRROR_ASSERT((result_01), "Sequences 1 and 2 must be equal");
        BOOST_MIRROR_ASSERT((result_02), "Sequences 2 and 3 must be equal");
        BOOST_MIRROR_ASSERT((result_03), "Sequences 1 and 3 must be equal");
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 05");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


