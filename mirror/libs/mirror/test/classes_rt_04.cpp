/**
 * \file test/classes_rt_04.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Tests the for_each algorithm and the attribute value 
 * getters/setters on reflected homogeneous boost::tuple
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/tuple/tuple.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/boost_tuple.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
//
#include "./test.hpp"

template <typename T>
class setter_tester
{
public:
	setter_tester(T& _t):t(_t){ }
	template <class MetaAttribute>
	inline void operator()(MetaAttribute ma)
	{
		// use the meta-attribute's setter
		// to set the position of the attribute as its
		// value
		ma.set(t, MetaAttribute::position::value);
	}
private:
	T& t;
};

template <typename T>
class getter_tester
{
public:
	getter_tester(T& _t):t(_t){ }

	template <class MetaAttribute>
	inline void operator()(MetaAttribute ma)
	{
		// use the meta-attribute's setter
		// to check if the position of the attribute 
		// is its value
		if(ma.get(t) != MetaAttribute::position::value)
			throw (int)MetaAttribute::position::value;

	}
private:
	T& t;
};

template <typename T>
class query_tester
{
public:
	query_tester(T& _t, T& _s):t(_t),s(_s){ }

	template <class MetaAttribute>
	inline void operator()(MetaAttribute ma)
	{
		// use the meta-attribute's value query 
		// to  copy the value of the t's attrib
		// to s's attrib
		ma.query(t, ::boost::get<MetaAttribute::position::value>(s));
	}
private:
	T& t;
	T& s;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// define the tuple 
	typedef tuples::tuple< 
		int, int, int, int, int, int, int, int, int, int 
	> T;
	// reflect it
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	// create an instance and initialize the attrib. values
	T t(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
	T s(t);
	//
	try
	{
		// execute the setter-tester functor on every attribute
		// - sets new values to t's attributes
		for_each<meta_T::attributes>(setter_tester<T>(t));
		// execute the query-tester
		// - copies t to s attribute-wise 
		for_each<meta_T::attributes>(query_tester<T>(t, s));
		// execute a getter-tester on every attribute
		// - gets and checks the s's attribute values
		// throws an (int) position of the attribute
		// where the check failed
		for_each<meta_T::attributes>(getter_tester<T>(s));
	}
	catch(int& i) 
	{
		// this is always false
		BOOST_CHECK(i == -1);
	}

}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 04");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


