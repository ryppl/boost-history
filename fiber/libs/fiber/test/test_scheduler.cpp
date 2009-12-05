
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <string>

#include <boost/ref.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/utility.hpp>

#include <boost/fiber.hpp>

int value1 = 0;
int value2 = 0;
int value3 = 0;

void zero_args_fn() {}

void value1_fn()
{ value1 = 1; }

void value2_fn()
{ value2 = 1; }

void value3_fn()
{ value3 = 1; }

void sched1_fn()
{
	boost::fibers::scheduler<> sched;

	sched.make_fiber( value1_fn);
	sched.make_fiber( value2_fn);
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 2), sched.size() );
	BOOST_CHECK_EQUAL( 0, value1);
	BOOST_CHECK_EQUAL( 0, value2);

	for (;;)
	{
		while ( sched.run() );
		if ( sched.empty() ) break;
	}

	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

void sched2_fn( boost::fibers::scheduler<> & sched)
{
	sched.make_fiber( value1_fn);
	sched.make_fiber( value2_fn);
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 2), sched.size() );
	BOOST_CHECK_EQUAL( 0, value1);
	BOOST_CHECK_EQUAL( 0, value2);

	for (;;)
	{
		while ( sched.run() );
		if ( sched.empty() ) break;
	}

	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

void sched3_fn( boost::fibers::scheduler<> & sched)
{
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

	boost::this_fiber::yield();

	BOOST_CHECK( sched.run() );
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);

	BOOST_CHECK( ! sched.run() );
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

void test_case_1()
{
	value1 = 0;
	value2 = 0;

	boost::fibers::scheduler<> sched;

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
	value1 = 0;
	value2 = 0;

	boost::fibers::scheduler<> sched;

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

	BOOST_CHECK( ! sched.run() );
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

void test_case_3()
{
	value1 = 0;
	value2 = 0;

	boost::fibers::scheduler<> sched;

	sched.make_fiber( sched1_fn);
	BOOST_CHECK( ! sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 1), sched.size() );
	BOOST_CHECK_EQUAL( 0, value1);
	BOOST_CHECK_EQUAL( 0, value2);
	
	for (;;)
	{
		while ( sched.run() );
		if ( sched.empty() ) break;
	}
	
	BOOST_CHECK( ! sched.run() );
	BOOST_CHECK( sched.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

void test_case_4()
{
	value1 = 0;
	value2 = 0;

	boost::fibers::scheduler<> sched1, sched2;

	sched1.make_fiber( sched2_fn, boost::ref( sched2) );
	BOOST_CHECK( ! sched1.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 1), sched1.size() );
	BOOST_CHECK_EQUAL( 0, value1);
	BOOST_CHECK_EQUAL( 0, value2);

	for (;;)
	{
		while ( sched1.run() );
		if ( sched1.empty() ) break;
	}

	BOOST_CHECK( sched1.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched1.size() );
	BOOST_CHECK( sched2.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched2.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
}

void test_case_5()
{
	value1 = 0;
	value2 = 0;
	value3 = 0;

	boost::fibers::scheduler<> sched1, sched2;

	sched1.make_fiber( sched3_fn, boost::ref( sched2) );
	sched1.make_fiber( value3_fn);
	BOOST_CHECK( ! sched1.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 2), sched1.size() );
	BOOST_CHECK_EQUAL( 0, value1);
	BOOST_CHECK_EQUAL( 0, value2);
	BOOST_CHECK_EQUAL( 0, value3);

	BOOST_CHECK( sched1.run() );
	BOOST_CHECK( ! sched1.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 2), sched1.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 0, value2);
	BOOST_CHECK_EQUAL( 0, value3);

	BOOST_CHECK( sched1.run() );
	BOOST_CHECK( ! sched1.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 1), sched1.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 0, value2);
	BOOST_CHECK_EQUAL( 1, value3);

	BOOST_CHECK( sched1.run() );
	BOOST_CHECK( sched1.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched1.size() );
	BOOST_CHECK( sched2.empty() );
	BOOST_CHECK_EQUAL( std::size_t( 0), sched2.size() );
	BOOST_CHECK_EQUAL( 1, value1);	
	BOOST_CHECK_EQUAL( 1, value2);
	BOOST_CHECK_EQUAL( 1, value3);
}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test =
		BOOST_TEST_SUITE("Boost.Fiber: scheduler test suite");

	test->add( BOOST_TEST_CASE( & test_case_1) );
	test->add( BOOST_TEST_CASE( & test_case_2) );
	test->add( BOOST_TEST_CASE( & test_case_3) );
	test->add( BOOST_TEST_CASE( & test_case_4) );
	test->add( BOOST_TEST_CASE( & test_case_5) );

	return test;
}
