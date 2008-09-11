/**
 * \file test/classes_rt_01.cpp
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

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"

class cmp_att_base_names
{
public:
	template <class MetaAttribute>
	void operator()(MetaAttribute ma)
	{
		const int i = MetaAttribute::position::value;
		all_match &= (ma.base_name() == names[i+offs]);
	}

	cmp_att_base_names(const ::boost::cts::bchar** _names, int _offs)
	 : names(_names)
	 , offs(_offs)
	 , all_match(true)
	{ }

	bool result(void) const {return all_match;}
private:
	const ::boost::cts::bchar** names;
	const int offs;
	bool all_match;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	::test::H h;
	typedef BOOST_MIRRORED_CLASS(::test::H) meta_H;

	// a hardcoded list of H's attribute names
	const cts::bchar* att_base_names[] = {
		BOOST_CTS_LIT("a1"),
		BOOST_CTS_LIT("a2"),
		BOOST_CTS_LIT("a3"),
		BOOST_CTS_LIT("b1"),
		BOOST_CTS_LIT("b2"),
		BOOST_CTS_LIT("c1"),
		BOOST_CTS_LIT("c2"),
		BOOST_CTS_LIT("d1"),
		BOOST_CTS_LIT("d2"),
		BOOST_CTS_LIT("e"),
		BOOST_CTS_LIT("f1"),
		BOOST_CTS_LIT("f2"),
		BOOST_CTS_LIT("g1"),
		BOOST_CTS_LIT("g2"),
		BOOST_CTS_LIT("g3"),
		BOOST_CTS_LIT("h1"),
		BOOST_CTS_LIT("h2"),
		BOOST_CTS_LIT("h3")
	};
	{
		// create the comparating functor
		// 15 is the offset of the "h1" attrib which
		// is the first own attribute of the H class
		cmp_att_base_names test_op(att_base_names, 15);
		// and call it for reflected own attribute
		// of the H class
		for_each<meta_H::attributes>(ref(test_op));
		// check the result of the operation
		BOOST_CHECK(( test_op.result() ));
	}
	{
		// create the comparating functor
		cmp_att_base_names test_op(att_base_names, 0);
		// and call it for every reflected attribute
		// of the H class
		for_each<meta_H::all_attributes>(ref(test_op));
		// check the result of the operation
		BOOST_CHECK(( test_op.result() ));
	}
	
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


