/**
 * \file test/classes_rt_03.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Test the attribute name getters with reflected ::std::pair
 * 
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/lexical_cast.hpp>
#include <utility>
//
#include <boost/mirror/intrinsic/at.hpp>
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/std/pair.hpp>
//
#include "./test.hpp"

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef ::std::pair<int, double> T;
	typedef BOOST_MIRRORED_CLASS(T) meta_T;

	BOOST_CHECK(( 
		at< meta_T::attributes, mpl::int_<0> >::type::base_name() == 
		BOOST_CTS_LIT("first")
	));
	BOOST_CHECK(( 
		at< meta_T::attributes, mpl::int_<1> >::type::base_name() == 
		BOOST_CTS_LIT("second")
	));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


