
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
#include <boost/thread/barrier.hpp>
#include <boost/utility.hpp>

#include <boost/task.hpp>

#include "test_functions.hpp"

namespace pt = boost::posix_time;
namespace tsk = boost::task;

class test_pool_unbounded_channel
{
public:
	// check size, active, idle
	void test_case_1()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
	}

	// check submit
	void test_case_2()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		pool.submit( t);
		BOOST_CHECK_EQUAL( t.get(), 55);
	}

	// check shutdown
	void test_case_3()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		pool.submit( t);
		pool.shutdown();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_EQUAL( t.get(), 55);
	}

	// check runtime_error throw inside task
	void test_case_4()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::task< void > t(
			tsk::make_task(
				throwing_fn) );
		pool.submit( t);
		pool.shutdown();
		bool thrown( false);
		try
		{ t.get(); }
		catch ( std::runtime_error const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check shutdown with task_rejected exception
	void test_case_5()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		pool.shutdown();
		BOOST_CHECK( pool.closed() );
		bool thrown( false);
		try
		{
			pool.submit(
				tsk::make_task(
					boost::bind(
						fibonacci_fn,
						10) ) );
		}
		catch ( tsk::task_rejected const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check shutdown_now with thread_interrupted exception
	void test_case_6()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::task< void > t(
			tsk::make_task(
				delay_fn,
				pt::millisec( 500) ) );
		pool.submit( t);
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		pool.shutdown_now();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
		bool thrown( false);
		try
		{ t.get(); }
		catch ( tsk::task_interrupted const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check pending
	void test_case_7()
	{
		typedef tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		tsk::task< void > t1(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ) );
		pool.submit( t1);
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
		tsk::task< int > t2(
			tsk::make_task(
				fibonacci_fn,
				10) );
		pool.submit( t2);
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 1) );
		tsk::task< int > t3(
			tsk::make_task(
				fibonacci_fn,
				10) );
		pool.submit( t3);
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 2) );
		b.wait();
		t1.get();
		BOOST_CHECK_EQUAL( t2.get(), 55);
		BOOST_CHECK_EQUAL( t3.get(), 55);
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
	}

	// check interruptation
	void test_case_8()
	{
		typedef tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		tsk::task< void > t(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10) );
		pool.submit( t);
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0) );
		t.interrupt();
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 1) );
		bool thrown( false);
		try
		{ t.get(); }
		catch ( tsk::task_interrupted const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check fifo scheduling
	void test_case_9()
	{
		typedef tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10) );
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0) );
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 55);
		BOOST_CHECK_EQUAL( buffer[1], 0);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}

	// check lifo scheduling
	void test_case_10()
	{
		typedef tsk::pool<
			tsk::unbounded_channel< tsk::lifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10) );
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0) );
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer[1], 55);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}

	// check priority scheduling
	void test_case_11()
	{
		typedef tsk::pool<
			tsk::unbounded_channel< tsk::priority< int > >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ),
			0);
		std::vector< int > buffer;
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			1);
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			0);
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer[1], 55);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}

	// check smart scheduling
	void test_case_12()
	{
		typedef tsk::pool<
			tsk::unbounded_channel< tsk::smart< int, std::less< int >, tsk::replace_oldest, tsk::take_oldest > >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ),
			0);
		std::vector< int > buffer;
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			2);
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			1);
		pool.submit(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				1),
			2);
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer[1], 1);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.ThreadPool: pool (with unbounded channel) test suite") );

	boost::shared_ptr< test_pool_unbounded_channel > instance( new test_pool_unbounded_channel() );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_3, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_5, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_6, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_7, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_8, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_9, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_10, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_11, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_pool_unbounded_channel::test_case_12, instance) );

	return test;
}
