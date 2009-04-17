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
#include <boost/mirror/meta_classes/std/pair.hpp>
#include <boost/mirror/intrinsic/by_name.hpp>
#include <boost/mirror/intrinsic/at.hpp>
//
#include "./simple_classes.hpp"
#include "./test.hpp"

template <class MetaAttribute1, class MetaAttribute2>
void test_attribs(void)
{
        typedef typename ::boost::is_same<
                typename MetaAttribute1::type::reflected_type,
                typename MetaAttribute2::type::reflected_type
        >::type result_01;
        BOOST_MIRROR_ASSERT(
                (result_01),
                "The attributes should have the same types"
        );
        //
        typedef typename ::boost::is_same<
                typename MetaAttribute1::scope::reflected_type,
                typename MetaAttribute2::scope::reflected_type
        >::type result_02;
        BOOST_MIRROR_ASSERT(
                (result_02),
                "The attributes should be declared in the same class"
        );
}

void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef ::std::pair<int, double> P;
	typedef BOOST_MIRRORED_CLASS(P) meta_P;
	//
	test_attribs<
		by_name<meta_P::attributes>::first,
		at<meta_P::attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_P::attributes>::second,
		at<meta_P::attributes, mpl::int_<1> >::type
	>();
	//
	test_attribs<
		by_name<meta_P::all_attributes>::first,
		at<meta_P::all_attributes, mpl::int_<0> >::type
	>();
	test_attribs<
		by_name<meta_P::all_attributes>::second,
		at<meta_P::all_attributes, mpl::int_<1> >::type
	>();

}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 11");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


