/**
 * \file test/classes_rt_02.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Tests attribute name getter on reflected boost::tuple and
 * the for_each run-time algorithm
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
//
#include "./test.hpp"

class cmp_att_base_names
{
public:
	template <class MetaAttribute>
	void operator()(MetaAttribute ma)
	{
		using namespace ::boost;
		const int i = MetaAttribute::position::value;
		all_match &= (
			ma.base_name() == 
			cts::bstring(BOOST_CTS_LIT("_")) + 
			lexical_cast<cts::bstring>(i)
		);
	}

	cmp_att_base_names(void)
	 : all_match(true)
	{ }

	bool result(void) const {return all_match;}
private:
	bool all_match;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef tuples::tuple< int, int, int, int, int, int, int, int, int > T;
	typedef BOOST_MIRRORED_CLASS(T) meta_T;

	try
	{
		// create the comparating functor
		cmp_att_base_names test_op;
		// and call it for every reflected attribute
		// of the T tuple class
		for_each<meta_T::all_attributes>(ref(test_op));
		// check the result of the operation
		BOOST_CHECK(( test_op.result() ));
	}
	catch(...){BOOST_CHECK(false);}
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


