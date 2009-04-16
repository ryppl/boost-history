/**
 * \file test/classes_ct_03.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * this test focuses (indirectly) on the registering,
 * on the count of member attributes, the size algorithm
 * and cooperation with MPL.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL_ASSERT
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/intrinsic/at.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"


#define BOOST_MIRROR_TEST_CLASSES_CT_03_MSG \
	"Reflected and real attribute type must match"

namespace boost {
namespace mirror {

template <class MetaAttributes, class Position>
struct add_pointer_to_type_at
{
	typedef typename add_pointer<
		typename at<
			MetaAttributes,
			Position
		>::type::type::reflected_type
	>::type type;
};

} // mirror 
} // boost

#define BOOST_MIRROR_TEST_CLASSES_CT_03(POSITION, ADDRESS) \
	typedef is_same< \
                add_pointer_to_type_at< \
                        meta_H::all_attributes, \
			mpl::int_< POSITION > \
                >::type, \
                BOOST_TYPEOF(ADDRESS) \
        >::type result_##POSITION; \
	BOOST_MIRROR_ASSERT( \
		result_##POSITION, \
		BOOST_MIRROR_TEST_CLASSES_CT_03_MSG \
	);

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	::test::H h;
	typedef BOOST_MIRRORED_CLASS(::test::H) meta_H;
	//
	//
	BOOST_MIRROR_TEST_CLASSES_CT_03( 0, &h.a1)
	BOOST_MIRROR_TEST_CLASSES_CT_03( 1, &h.a2)
	BOOST_MIRROR_TEST_CLASSES_CT_03( 2, &h.a3)

	BOOST_MIRROR_TEST_CLASSES_CT_03( 3, &h.b1)
	BOOST_MIRROR_TEST_CLASSES_CT_03( 4, &h.b2)

	BOOST_MIRROR_TEST_CLASSES_CT_03( 5, &h.c1)
	BOOST_MIRROR_TEST_CLASSES_CT_03( 6, &h.c2)

	BOOST_MIRROR_TEST_CLASSES_CT_03( 7, &h.d1)
	BOOST_MIRROR_TEST_CLASSES_CT_03( 8, &h.d2)

	BOOST_MIRROR_TEST_CLASSES_CT_03( 9, &h.e)

	BOOST_MIRROR_TEST_CLASSES_CT_03(10, &h.f1)
	BOOST_MIRROR_TEST_CLASSES_CT_03(11, &h.f2)

	BOOST_MIRROR_TEST_CLASSES_CT_03(12, &h.g1)
	BOOST_MIRROR_TEST_CLASSES_CT_03(13, &h.g2)
	BOOST_MIRROR_TEST_CLASSES_CT_03(14, &h.g3)

	BOOST_MIRROR_TEST_CLASSES_CT_03(15, &h.h1)
	BOOST_MIRROR_TEST_CLASSES_CT_03(16, &h.h2)
	BOOST_MIRROR_TEST_CLASSES_CT_03(17, &h.h3)

}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 03");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


