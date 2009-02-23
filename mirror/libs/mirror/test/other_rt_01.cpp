/**
 * \file test/other_rt_01.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Tests the internal counter facility

 *  
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//
#include <boost/mirror/detail/counter.hpp>
//
#include "./test.hpp"

namespace boost {
namespace mirror {
namespace counter {

struct test_selector_1 { };
struct test_selector_2 { };

#define BOOST_MIRROR_TEST_COUNTERS_WITH_SELECTOR(SELECTOR, VALUE) \
int test_value( \
	SELECTOR*, \
	BOOST_MIRROR_COUNTER_CURRENT_COUNT(SELECTOR, __LINE__) \
) \
{ \
	return VALUE;\
} \
BOOST_MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, __LINE__)

#define BOOST_MIRROR_TEST_COUNTERS(VALUE) \
BOOST_MIRROR_TEST_COUNTERS_WITH_SELECTOR(test_selector_1, VALUE) \
BOOST_MIRROR_TEST_COUNTERS_WITH_SELECTOR(test_selector_2, VALUE) 

BOOST_MIRROR_TEST_COUNTERS(1)
BOOST_MIRROR_TEST_COUNTERS(2)
BOOST_MIRROR_TEST_COUNTERS(3)
BOOST_MIRROR_TEST_COUNTERS(4)
BOOST_MIRROR_TEST_COUNTERS(5)
BOOST_MIRROR_TEST_COUNTERS(6)
BOOST_MIRROR_TEST_COUNTERS(7)
BOOST_MIRROR_TEST_COUNTERS(8)
BOOST_MIRROR_TEST_COUNTERS(9)

} // namespace counter
} // namespace mirror
} // namespace boost

template <class Number>
void test_value(Number number)
{
	BOOST_CHECK(
		Number::value ==
		::boost::mirror::counter::test_value(
			(::boost::mirror::counter::test_selector_1*)0,
			number
		)
	);
	BOOST_CHECK(
		Number::value ==
		::boost::mirror::counter::test_value(
			(::boost::mirror::counter::test_selector_2*)0,
			number
		)
	);
	BOOST_CHECK(
		::boost::mirror::counter::test_value(
			(::boost::mirror::counter::test_selector_1*)0,
			number
		) == 
		::boost::mirror::counter::test_value(
			(::boost::mirror::counter::test_selector_2*)0,
			number
		)
	);
}

void test_main()
{
	test_value(::boost::mpl::int_<1>());
	test_value(::boost::mpl::int_<2>());
	test_value(::boost::mpl::int_<3>());
	test_value(::boost::mpl::int_<4>());
	test_value(::boost::mpl::int_<5>());
	test_value(::boost::mpl::int_<6>());
	test_value(::boost::mpl::int_<7>());
	test_value(::boost::mpl::int_<8>());
	test_value(::boost::mpl::int_<9>());
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: other run-time test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


