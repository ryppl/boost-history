/**
 * \file test/classes_rt_05.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Tests the for_each algorithm and the attribute value 
 * getters/setters on all attributes of a reflected test class
 * and a heterogeneous boost::tuple
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//
#include <boost/tuple/tuple.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
#include <boost/mirror/intrinsic/size.hpp>
//
#include "./test.hpp"
#include "./classes.hpp"

template <typename T, typename S>
class tester
{
public:
	tester(T& _t, S& _s):t(_t),s(_s){ }

	template <class MetaAttribute>
	inline void operator()(MetaAttribute)
	{
		BOOST_CHECK(
			MetaAttribute::get(t) == 
			::boost::get<
				MetaAttribute::position::value
			>(s)
		);
	}
private:
	T& t;
	S& s;
};


void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	using namespace ::test;
	//
	// reflect the test::E class
	typedef BOOST_MIRRORED_CLASS(E) meta_E;
	// define a tuple that corresponds to the E class
	typedef tuple<
		long, long, long, int, int, double, double, short, short, float
	> T;
	// reflect the tuple
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	//
	BOOST_MPL_ASSERT_RELATION(
		size<meta_E::all_attributes>::value,
		==,
		size<meta_T::attributes>::value
	);
	//
	// create an instance of the E class 
	// the attributes are initialized by the constructors
	E e;
	// create an instace of the T tuple
	// and initialize the attribute values to correspond 
	// to those of the E class instance
	T t(1L, 2L, 3L, 4, 5, 6.0, 7.0, 8, 9, 10.0f);
	//
	// call the test functor for each attribute
	for_each<meta_E::all_attributes>(tester<E, T>(e, t));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 05");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


