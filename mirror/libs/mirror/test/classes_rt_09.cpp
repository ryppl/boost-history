/**
 * \file test/classes_ct_08.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
//
#include "./namespaces.hpp"
#include "./test.hpp"


namespace test {

struct X
{
	int _1;
	int _2;
	int _3;
	int _4;
	int _5;
	int _6;
	int _7;
	int _8;
	int _9;
};

} // namespace test

namespace boost {
namespace mirror {

BOOST_MIRROR_REG_TYPE(::test, X)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _1)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _2)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _3)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _4)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _5)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _6)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _7)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _8)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _9)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

} // namespace mirror 
} // namespace boost

class offset_of_tester
{
private:
	int current_offset;
	int max_offset;
public:
	offset_of_tester(int max)
	 : current_offset(0)
	 , max_offset(max)
	{ }

	template <class MetaAttribute>
	void operator()(MetaAttribute ma) 
	{
		BOOST_CHECK(current_offset <= ma.offset_of());
		BOOST_CHECK(ma.offset_of() <= max_offset);
		current_offset = ma.offset_of();
	}
};



void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef BOOST_MIRRORED_CLASS(::test::X) meta_X;
	//
	offset_of_tester t1(sizeof(::test::X));
	for_each<meta_X::attributes>(ref(t1));
	offset_of_tester t2(sizeof(::test::X));
	for_each<meta_X::all_attributes>(ref(t2));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 09");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


