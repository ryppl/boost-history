
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
uint32_t wait_fn( uint32_t n, tsk::spin_manual_reset_event & ev)
{
	ev.wait();
	return n;
}
}

class test_spin_manual_reset_event
{
public:
	// check wait in new thread
	void test_case_1()
	{
		uint32_t n = 3;
		tsk::spin_manual_reset_event ev;

		tsk::handle< uint32_t > h1(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );
		tsk::handle< uint32_t > h2(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( ! h1.is_ready() );
		BOOST_CHECK( ! h2.is_ready() );

		ev.set();

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( h1.is_ready() );
		BOOST_CHECK( h2.is_ready() );
		BOOST_CHECK_EQUAL( h1.get(), n);
		BOOST_CHECK_EQUAL( h2.get(), n);

		ev.reset();

		tsk::handle< uint32_t > h3(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );
		tsk::handle< uint32_t > h4(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( ! h3.is_ready() );
		BOOST_CHECK( ! h4.is_ready() );

		ev.set();

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( h3.is_ready() );
		BOOST_CHECK( h4.is_ready() );
		BOOST_CHECK_EQUAL( h3.get(), n);
		BOOST_CHECK_EQUAL( h4.get(), n);
	}

	// check wait in pool
	void test_case_2()
	{
		tsk::static_pool<
			tsk::unbounded_onelock_fifo
		> pool( tsk::poolsize( 3) );

		uint32_t n = 3;
		tsk::spin_manual_reset_event ev;

		tsk::handle< uint32_t > h1(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					pool) );
		tsk::handle< uint32_t > h2(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					pool) );

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( ! h1.is_ready() );
		BOOST_CHECK( ! h2.is_ready() );

		ev.set();

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( h1.is_ready() );
		BOOST_CHECK( h2.is_ready() );
		BOOST_CHECK_EQUAL( h1.get(), n);
		BOOST_CHECK_EQUAL( h2.get(), n);

		ev.reset();

		tsk::handle< uint32_t > h3(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );
		tsk::handle< uint32_t > h4(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( ! h3.is_ready() );
		BOOST_CHECK( ! h4.is_ready() );

		ev.set();

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( h3.is_ready() );
		BOOST_CHECK( h4.is_ready() );
		BOOST_CHECK_EQUAL( h3.get(), n);
		BOOST_CHECK_EQUAL( h4.get(), n);
	}

	void test_case_3()
	{
		uint32_t n = 3;
		tsk::spin_manual_reset_event ev( true);

		tsk::handle< uint32_t > h1(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );
		tsk::handle< uint32_t > h2(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( h1.is_ready() );
		BOOST_CHECK( h2.is_ready() );
		BOOST_CHECK_EQUAL( h1.get(), n);
		BOOST_CHECK_EQUAL( h2.get(), n);

		ev.reset();

		tsk::handle< uint32_t > h3(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );
		tsk::handle< uint32_t > h4(
				tsk::async(
					tsk::make_task(
						wait_fn,
						n, boost::ref( ev) ),
					tsk::new_thread() ) );

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( ! h3.is_ready() );
		BOOST_CHECK( ! h4.is_ready() );

		ev.set();

		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK( h3.is_ready() );
		BOOST_CHECK( h4.is_ready() );
		BOOST_CHECK_EQUAL( h3.get(), n);
		BOOST_CHECK_EQUAL( h4.get(), n);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.Task: test suite") );

	boost::shared_ptr< test_spin_manual_reset_event > instance( new test_spin_manual_reset_event() );
	test->add( BOOST_CLASS_TEST_CASE( & test_spin_manual_reset_event::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_spin_manual_reset_event::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_spin_manual_reset_event::test_case_3, instance) );

	return test;
}
