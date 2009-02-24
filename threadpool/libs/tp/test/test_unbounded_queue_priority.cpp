//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/future/future.hpp>
#include <boost/ref.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/utility.hpp>

#include <boost/tp/bounded_channel.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/priority.hpp>
#include <boost/tp/pool.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/task.hpp>

#include "test_functions.hpp"

namespace pt = boost::posix_time;
namespace tp = boost::tp;

class fixed_unbounded_channel_priority
{
public:
	// check size, active, idle
	void test_case_1()
	{
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 3) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
	}

	// check submit
	void test_case_2()
	{
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 1) );
		tp::task< int > tsk(
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10),
			0) );
		BOOST_CHECK_EQUAL( tsk.get(), 55);
	}

	// check shutdown
	void test_case_3()
	{
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 1) );
		tp::task< int > t(
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10),
				0) );
		pool.shutdown();
		BOOST_CHECK( pool.terminated() );
		BOOST_CHECK_EQUAL( t.get(), 55);
	}

	// check runtime_error throw inside task
	void test_case_4()
	{
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 1) );
		tp::task< void > tsk(
			pool.submit(
				boost::bind(
					throwing_fn),
				0) );
		pool.shutdown();
		bool thrown( false);
		try
		{ tsk.get(); }
		catch ( std::runtime_error const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check shutdown with task_rejected exception
	void test_case_5()
	{
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 1) );
		pool.shutdown();
		BOOST_CHECK( pool.terminated() );
		bool thrown( false);
		try
		{
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10),
				0);
		}
		catch ( tp::task_rejected const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check shutdown_now with thread_interrupted exception
	void test_case_6()
	{
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 1) );
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		tp::task< int > tsk(
			pool.submit(
				boost::bind(
					( int ( *)( boost::function< int() > const&, pt::time_duration const&) ) delay_fn,
					fn,
					pt::millisec( 500) ),
				0) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 0) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 1) );
		pool.shutdown_now();
		BOOST_CHECK( pool.terminated() );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
		bool thrown( false);
		try
		{ tsk.get(); }
		catch ( boost::thread_interrupted const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check pending
	void test_case_7()
	{
		typedef tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool_type;
		pool_type pool( tp::poolsize( 1) );
		boost::barrier b( 2);
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		tp::task< int > tsk1(
			pool.submit(
				boost::bind(
					( int ( *)( boost::function< int() > const&, boost::barrier &) ) barrier_fn,
					fn,
					boost::ref( b) ),
				0) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
		tp::task< int > tsk2( pool.submit( fn, 0) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 1) );
		tp::task< int > tsk3( pool.submit( fn, 0) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 2) );
		b.wait();
		BOOST_CHECK_EQUAL( tsk1.get(), 55);
		BOOST_CHECK_EQUAL( tsk2.get(), 55);
		BOOST_CHECK_EQUAL( tsk3.get(), 55);
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
	}

	// check priority scheduling
	void test_case_8()
	{
		typedef tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool_type;
		pool_type pool( tp::poolsize( 1) );
		boost::barrier b( 2);
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		tp::task< int > tsk1(
			pool.submit(
				boost::bind(
					( int ( *)( boost::function< int() > const&, boost::barrier &) ) barrier_fn,
					fn,
					boost::ref( b) ),
				0) );
		std::vector< int > buffer;
		pool.submit(
			boost::bind(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			1);
		pool.submit(
			boost::bind(
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

	// check interruptation
	void test_case_9()
	{
		typedef tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool_type;
		pool_type pool( tp::poolsize( 1) );
		boost::barrier b( 2);
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		pool.submit(
			boost::bind(
				( int ( *)( boost::function< int() > const&, boost::barrier &) ) barrier_fn,
				fn,
				boost::ref( b) ),
			0);
		std::vector< int > buffer;
		tp::task< void > tsk(
			pool.submit(
				boost::bind(
					buffer_fibonacci_fn,
					boost::ref( buffer),
					10),
				0) );
		pool.submit(
			boost::bind(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			0);
		tsk.interrupt();
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 1) );
		bool thrown( false);
		try
		{ tsk.get(); }
		catch ( boost::thread_interrupted const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.ThreadPool: fixed unbounded_channel< priority< int > > pool test suite") );

	boost::shared_ptr< fixed_unbounded_channel_priority > instance( new fixed_unbounded_channel_priority() );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_3, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_5, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_6, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_7, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_8, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & fixed_unbounded_channel_priority::test_case_9, instance) );

	return test;
}

