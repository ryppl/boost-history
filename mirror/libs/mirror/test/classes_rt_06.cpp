/**
 * \file test/classes_rt_06.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Tests the getter/setter functions of the meta-attribute sequences
 * on custom test classes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/at.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/intrinsic/get_type_list.hpp>
//
#include "./test.hpp"
#include "./classes.hpp"

template <typename T>
class increment_value
{
public:
	increment_value(T& _t) : t(_t) { }

	template <class Position>
	inline void operator()(Position position)
	{
		using namespace ::boost;
		using namespace ::boost::mirror;
		// reflect the class 
		typedef BOOST_MIRRORED_CLASS(T) meta_T;
		// get the meta-attributes
		typedef typename meta_T::all_attributes meta_attribs;
		// get the type of this attribute
		typedef typename mpl::at<
			typename get_type_list<meta_attribs>::type,
			Position
		>::type attrib_type;
		// get the value of this attribute
		attrib_type value(meta_attribs::get(
			t, position
		));
		// increment by one and set the value of this 
		// attribute
		meta_attribs::set(
			t, position,
			attrib_type(value + 1)
		);
	}
private:
	T& t;
};

template <typename T, bool Equal>
class check_equality
{
public:
	check_equality(T& _t, T& _s) : t(_t), s(_s) { }

	template <class Position>
	inline void operator()(Position position)
	{
		using namespace ::boost;
		using namespace ::boost::mirror;
		// reflect the class 
		typedef BOOST_MIRRORED_CLASS(T) meta_T;
		// get the meta-attributes
		typedef typename meta_T::all_attributes meta_attribs;
		// get the type of this attribute
		typedef typename mpl::at<
			typename get_type_list<meta_attribs>::type,
			Position
		>::type attrib_type;
		//
		// get the equality of values
		BOOST_CHECK((
			meta_attribs::get(
				t, position
			) == meta_attribs::get(
				s, position
			)) == Equal
		);
	}
private:
	T& t;
	T& s;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	using namespace ::test;
	//
	H g, h;
	// create a list of several of H's attribute positions
	typedef mpl::vector<
		mpl::int_< 0>, // A::a1
		mpl::int_< 1>, // A::a2
		mpl::int_< 2>, // A::a3
		mpl::int_< 4>, // B::b2
		mpl::int_< 5>, // C::c1
		mpl::int_< 6>, // C::c2
		mpl::int_<12>, // G::g1
		mpl::int_<13>, // G::g2
		mpl::int_<14>, // G::g3
		mpl::int_<15>, // H::h1
		mpl::int_<16>, // H::h2
		mpl::int_<17>  // H::h3
	> mutable_attr_positions;

	mpl::for_each<mutable_attr_positions>(check_equality<H, true>(g, h));
	mpl::for_each<mutable_attr_positions>(increment_value<H>(g));
	mpl::for_each<mutable_attr_positions>(check_equality<H, false>(g, h));
	mpl::for_each<mutable_attr_positions>(increment_value<H>(h));
	mpl::for_each<mutable_attr_positions>(check_equality<H, true>(g, h));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 05");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


