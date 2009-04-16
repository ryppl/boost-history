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

struct X1
{
	int _1;
};

struct X2 : X1
{
	int _2;
};

struct X3 : X2
{
	int _3;
};

struct X4 : X3
{
	int _4;
};

struct X5 : X4
{
	int _5;
};

struct X6 : X5
{
	int _6;
};

struct X7 : X6
{
	int _7;
};

struct X8 : X7
{
	int _8;
};

struct X9 : X8
{
	int _9;
};

struct X : X9
{ };

} // namespace test

namespace boost {
namespace mirror {

BOOST_MIRROR_REG_TYPE(::test, X1)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X1)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _1)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X2)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X2)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X1)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X2)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _2)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X3)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X3)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X2)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X3)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _3)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X4)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X4)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X3)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X4)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _4)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X5)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X5)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X4)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X5)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _5)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X6)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X6)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X5)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X6)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _6)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X7)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X7)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X6)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X7)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _7)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X8)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X8)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X7)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X8)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _8)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X9)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X9)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X8)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::X9)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, _9)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, X)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::X)
        BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::X9)
BOOST_MIRROR_REG_BASE_CLASSES_END


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
	offset_of_tester t(sizeof(::test::X));
	for_each<meta_X::attributes>(ref(t));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes run-time test 10");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


