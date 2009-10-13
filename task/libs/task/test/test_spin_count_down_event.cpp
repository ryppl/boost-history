
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/task.hpp>

#include "test_functions.hpp"

namespace pt = boost::posix_time;
namespace tsk = boost::task;

namespace {
uint32_t wait_fn( uint32_t n, tsk::spin_count_down_event & ev)
{
	ev.wait();
	return n;
}
}

class test_spin_count_down_event
{
public:
	// check initial + current
	void test_case_1()
	{
		uint32_t n = 3;
		tsk::spin_count_down_event ev( n);
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), n);

		ev.set();
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), static_cast< uint32_t >( 2) );

		ev.set();
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), static_cast< uint32_t >( 1) );

		ev.set();
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), static_cast< uint32_t >( 0) );

		ev.set();
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), static_cast< uint32_t >( 0) );
	}

	// check wait in new thread
	void test_case_2()
	{
		uint32_t n = 3;
		tsk::spin_count_down_event ev( n);
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), n);

		tsk::handle< uint32_t > h(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );
		BOOST_CHECK( ! h.is_ready() );
		for ( uint32_t i = 0; i < n; ++i)
		{
			ev.set();
			BOOST_CHECK( ! h.is_ready() );
		}
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), static_cast< uint32_t >( 0) );
		BOOST_CHECK_EQUAL( h.get(), n);
	}

	// check wait in pool
	void test_case_3()
	{
		tsk::static_pool<
			tsk::unbounded_onelock_fifo
		> pool( tsk::poolsize( 3) );

		uint32_t n = 3;
		tsk::spin_count_down_event ev( n);
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), n);

		tsk::handle< uint32_t > h(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					pool) );
		BOOST_CHECK( ! h.is_ready() );
		for ( uint32_t i = 0; i < n; ++i)
		{
			ev.set();
			BOOST_CHECK( ! h.is_ready() );
		}
		BOOST_CHECK_EQUAL( ev.initial(), n);
		BOOST_CHECK_EQUAL( ev.current(), static_cast< uint32_t >( 0) );
		BOOST_CHECK_EQUAL( h.get(), n);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.Task: test suite") );

	boost::shared_ptr< test_spin_count_down_event > instance( new test_spin_count_down_event() );
	test->add( BOOST_CLASS_TEST_CASE( & test_spin_count_down_event::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_spin_count_down_event::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_spin_count_down_event::test_case_3, instance) );

	return test;
}
