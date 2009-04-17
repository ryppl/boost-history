/**
 * \file test/other_rt_02.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Tests the internal global_list facility

 *  
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//
#include <boost/mirror/detail/global_list.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/at.hpp>

//
#include "./test.hpp"

namespace boost {
namespace mirror {


struct selector_1 { };
struct selector_2 { };
struct selector_3 { };
struct selector_4 { };

struct item_1_1 { };
struct item_1_2 { };
struct item_1_3 { };

struct item_2_1 { };
struct item_2_2 { };
struct item_2_3 { };
struct item_2_4 { };

struct item_3_1 { };
struct item_3_2 { };
struct item_3_3 { };
struct item_3_4 { };
struct item_3_5 { };

struct item_4_1 { };
struct item_4_2 { };
struct item_4_3 { };
struct item_4_4 { };
struct item_4_5 { };
struct item_4_6 { };


BOOST_MIRROR_REGISTER_GLOBAL_LIST_SELECTOR(selector_1)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_1, item_1_1, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_1, item_1_2, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_1, item_1_3, __LINE__)

BOOST_MIRROR_REGISTER_GLOBAL_LIST_SELECTOR(selector_2)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_2, item_2_1, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_2, item_2_2, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_2, item_2_3, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_2, item_2_4, __LINE__)

BOOST_MIRROR_REGISTER_GLOBAL_LIST_SELECTOR(selector_3)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_3, item_3_1, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_3, item_3_2, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_3, item_3_3, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_3, item_3_4, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_3, item_3_5, __LINE__)

BOOST_MIRROR_REGISTER_GLOBAL_LIST_SELECTOR(selector_4)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_4, item_3_1, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_4, item_4_2, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_4, item_4_3, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_4, item_4_4, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_4, item_4_5, __LINE__)
BOOST_MIRROR_ADD_TO_GLOBAL_LIST_BASE(selector_4, item_4_6, __LINE__)

} // namespace mirror
} // namespace boost

template <class Selector>
struct get_list_by_selector
{
	typedef BOOST_MIRROR_GET_GLOBAL_LIST_BASE(
		Selector,
		__LINE__,
		typename
	) type;
};

void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	using namespace ::boost::mirror::counter;
	
	typedef mpl::vector<
		mpl::vector2<selector_1, mpl::int_<3> >,
		mpl::vector2<selector_2, mpl::int_<4> >,
		mpl::vector2<selector_3, mpl::int_<5> >,
		mpl::vector2<selector_4, mpl::int_<6> >
	> selectors_and_sizes;

	BOOST_MIRROR_ASSERT(
		(mpl::accumulate<
			selectors_and_sizes,
			mpl::true_,
			mpl::and_<
				mpl::_1,
				mpl::equal_to<
					mpl::size<
						get_list_by_selector<	
							mpl::front< mpl::_2>
						>
					>,
					mpl::at< mpl::_2, mpl::int_<1> >
				>
			>
		>::type),
		"Sizes of the global lists must match the hardcoded values"
	);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: other run-time test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


