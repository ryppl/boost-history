
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/utility.hpp>

#include <boost/fiber.hpp>

void zero_args_fn() {}

// check fiber
void test_case_1()
{
	boost::fiber::attributes attribs;
	boost::fiber::detail::fiber f1( zero_args_fn, attribs);
	boost::fiber::detail::fiber f2( zero_args_fn, attribs);

	BOOST_CHECK( f1 != f2);

	std::ostringstream os1;
	os1 << f1.get_id();
	std::ostringstream os2;
	os2 << f2.get_id();

	std::string not_a_fiber("{not-a-fiber}");
	BOOST_CHECK( os1.str() != os2.str() );
	BOOST_CHECK( os1.str() != not_a_fiber);
	BOOST_CHECK( os2.str() != not_a_fiber);
}

// check swap
void test_case_2()
{
	boost::fiber::attributes attribs;
	boost::fiber::detail::fiber f1( zero_args_fn, attribs);
	boost::fiber::detail::fiber f2( zero_args_fn, attribs);

	boost::fiber::id id1 = f1.get_id();
	boost::fiber::id id2 = f2.get_id();

	f1.swap( f2);

	BOOST_CHECK_EQUAL( f1.get_id(), id2);
	BOOST_CHECK_EQUAL( f2.get_id(), id1);
}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test =
		BOOST_TEST_SUITE("Boost.Fiber: fiber test suite");

	test->add( BOOST_TEST_CASE( & test_case_1) );
	test->add( BOOST_TEST_CASE( & test_case_2) );

	return test;
}
