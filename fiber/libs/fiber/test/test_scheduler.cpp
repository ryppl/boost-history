
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/utility.hpp>

#include <boost/fiber.hpp>

int value1 = 0;
int value2 = 0;

void zero_args_fn() {}

void value1_fn()
{ value1 = 1; }

void value2_fn()
{ value2 = 1; }

void test_case_1()
{
	boost::fibers::scheduler sched;
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK( ! sched.run() );

	boost::fiber f( boost::fibers::make_fiber( zero_args_fn) );
	BOOST_CHECK( ! f.is_alive() );
	sched.submit_fiber( f);
	BOOST_CHECK( f.is_alive() );
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 1), sched.size() );

	sched.submit_fiber(
		boost::fibers::make_fiber( zero_args_fn) );
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 2), sched.size() );

	BOOST_CHECK( sched.run() );
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 1), sched.size() );

	BOOST_CHECK( sched.run() );
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );

	BOOST_CHECK( ! f.is_alive() );
}

void test_case_2()
{
	boost::fibers::scheduler sched;

	sched.make_fiber( value1_fn);
	sched.make_fiber( value2_fn);
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 2), sched.size() );
	BOOST_CHECK_EQUAL( 0, value1);
	BOOST_CHECK_EQUAL( 0, value2);

	BOOST_CHECK( sched.run() );
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 1), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 0, value2);

	BOOST_CHECK( sched.run() );
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test =
		BOOST_TEST_SUITE("Boost.Fiber: rrp test suite");

	test->add( BOOST_TEST_CASE( & test_case_1) );
	test->add( BOOST_TEST_CASE( & test_case_2) );

	return test;
}
