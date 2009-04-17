/**
 * \file test/namespaces_ct_04.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp> 
// namespace registering 
#include <boost/mirror/traits/reflects_namespace.hpp>
// base meta-types
#include <boost/mirror/meta_type.hpp>
//
#include "./namespaces.hpp"
#include "./test.hpp"

#define BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_1(TYPE) \
	"The " #TYPE " type should be defined on the global scope"

#define BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(TYPE) \
        BOOST_MIRROR_ASSERT( \
                (reflects_namespace< \
                        BOOST_MIRRORED_TYPE(TYPE) :: scope \
                >), \
		BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_1(TYPE) \
         );

#define BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_2(MODAL) \
	"The types " MODAL " be defined in the same scope"

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(bool)
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(char)
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(int)
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(double)
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(void*)
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(int const * const *)
	BOOST_MIRROR_TEST_NAMESPACES_CT_04_TYPE_GS(char volatile * const &)
	//
	typedef is_same<
                BOOST_MIRRORED_TYPE(const char* []) :: scope,
                BOOST_MIRRORED_TYPE(int volatile &) :: scope
        >::type result_01;
	BOOST_MIRROR_ASSERT(
		(result_01),
		BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_2("should")
	);
	
	typedef is_same<
                BOOST_MIRRORED_TYPEDEF(::boost::cts, bchar) :: scope,
                BOOST_MIRRORED_TYPEDEF(::boost::cts, bstring) :: scope
        >::type result_02;
	BOOST_MIRROR_ASSERT(
		(result_02),
		BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_2("should")
	);

	typedef is_same<
                BOOST_MIRRORED_TYPEDEF(::boost::cts, bchar) :: scope,
                BOOST_MIRRORED_TYPE(char) :: scope
        >::type result_03;
	BOOST_MIRROR_ASSERT_NOT(
		(result_03),
		BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_2("should not")
	);
	
	typedef is_same<
                BOOST_MIRRORED_TYPEDEF(::boost::cts, bchar) :: scope,
                BOOST_MIRRORED_TYPE(wchar_t) :: scope
        >::type result_04;
	BOOST_MIRROR_ASSERT_NOT(
		(result_04),
		BOOST_MIRROR_TEST_NAMESPACES_CT_04_MSG_2("should not")
	);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 04");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


