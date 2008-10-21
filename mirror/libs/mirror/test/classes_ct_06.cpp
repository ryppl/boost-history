/**
 * \file test/classes_ct_06.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * this test focuses (indirectly) on the registering,
 * on the count of base classes, the size algorithm
 * and cooperation with MPL.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/equal_to.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/intrinsic/size.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"

template <typename MetaClass>
struct get_base_classes
{
	typedef typename MetaClass::base_classes type;
};


void test_main()
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef mpl::vector<
		// < class, base_class_count >
		mpl::vector< ::test::A, mpl::int_< 0> >,
		mpl::vector< ::test::B, mpl::int_< 1> >,
		mpl::vector< ::test::C, mpl::int_< 1> >,
		mpl::vector< ::test::D, mpl::int_< 1> >,
		mpl::vector< ::test::E, mpl::int_< 3> >,
		mpl::vector< ::test::F, mpl::int_< 1> >,
		mpl::vector< ::test::G, mpl::int_< 1> >,
		mpl::vector< ::test::H, mpl::int_< 2> >,
		mpl::vector< ::test::I, mpl::int_< 0> >,
		mpl::vector< ::test::J, mpl::int_< 0> >,
		mpl::vector< ::test::K, mpl::int_< 2> >
	> some_classes;

	// check the counts of base classes
	BOOST_MPL_ASSERT(( mpl::accumulate<
		some_classes,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			mpl::equal_to<
				mirror::size< get_base_classes<
					BOOST_MIRRORED_CLASS(
						mpl::front<mpl::_2>
					)
				> >,
				mpl::at<mpl::_2, mpl::int_<1> >
			>
		>
	>::type ));

}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 06");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


