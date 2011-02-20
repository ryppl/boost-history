//  Copyright (c) 2008 Oliver Kowalke.
//  Copyright (c) 2011 Vicente J. Botet Escriba.
//  Distributed under the Boost
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
#include <boost/thread/future.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/utility.hpp>

#include <boost/tp/bounded_channel.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/pool.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/watermark.hpp>

#include "test_functions.hpp"
#include <boost/detail/lightweight_test.hpp>
#define BOOST_CHECK_EQUAL(A,B) BOOST_TEST((A)==(B))
#define BOOST_CHECK(A) BOOST_TEST(A)

namespace pt = boost::posix_time;
namespace tp = boost::tp;

namespace fixed_bounded_channel_fifo
{
	// check size, active, idle
	void test_case_1()
	{
		tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool(
			tp::poolsize( 3),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
	}

	// check submit
	void test_case_2()
	{
		tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		tp::task< int > t(
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10) ) );
		BOOST_CHECK_EQUAL( t.result().get(), 55);
	}

	// check shutdown
	void test_case_3()
	{
		tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		tp::task< int > t(
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10) ) );
		pool.shutdown();
		BOOST_CHECK( pool.terminated() );
		BOOST_CHECK_EQUAL( t.result().get(), 55);
	}

	// check runtime_error throw inside task
	void test_case_4()
	{
		tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		tp::task< void > t(
			pool.submit(
				boost::bind<void>(
					throwing_fn
        )
      )
    );
		pool.shutdown();
		bool thrown( false);
		try
		{ t.result().get(); }
		catch ( std::runtime_error const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check shutdown with task_rejected exception
	void test_case_5()
	{
		tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		pool.shutdown();
		BOOST_CHECK( pool.terminated() );
		bool thrown( false);
		try
		{
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10) );
		}
		catch ( tp::task_rejected const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check shutdown_now with thread_interrupted exception
	void test_case_6()
	{
		tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool(
			tp::poolsize( 1),
			tp::high_watermark( 1),
			tp::low_watermark( 1) );
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		tp::task< int > t(
			pool.submit(
				boost::bind(
					( int ( *)( boost::function< int() > const&, pt::time_duration const&) ) delay_fn,
					fn,
					pt::millisec( 500) ) ) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		pool.shutdown_now();
		BOOST_CHECK( pool.terminated() );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
		bool thrown( false);
		try
		{ t.result().get(); }
		catch ( boost::thread_interrupted const&)
		{ thrown = true; }
		catch ( ... )
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check pending
	void test_case_7()
	{
		typedef tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool_type;
		pool_type pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		boost::barrier b( 2);
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		tp::task< int > t1(
			pool.submit(
				boost::bind(
					( int ( *)( boost::function< int() > const&, boost::barrier &) ) barrier_fn,
					fn,
					boost::ref( b) ) ) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
		tp::task< int > t2( pool.submit( fn) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 1) );
		tp::task< int > t3( pool.submit( fn) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 2) );
		b.wait();
		BOOST_CHECK_EQUAL( t1.result().get(), 55);
		BOOST_CHECK_EQUAL( t2.result().get(), 55);
		BOOST_CHECK_EQUAL( t3.result().get(), 55);
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
	}

	// check fifo scheduling
	void test_case_8()
	{
		typedef tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool_type;
		pool_type pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		boost::barrier b( 2);
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		pool.submit(
			boost::bind(
				( int ( *)( boost::function< int() > const&, boost::barrier &) ) barrier_fn,
				fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		pool.submit(
			boost::bind<void>(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10) );
		pool.submit(
			boost::bind<void>(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0) );
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 55);
		BOOST_CHECK_EQUAL( buffer[1], 0);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}

	// check cancelation
	void test_case_9()
	{
		typedef tp::pool<
			tp::bounded_channel< tp::fifo >
		> pool_type;
		pool_type pool(
			tp::poolsize( 1),
			tp::high_watermark( 10),
			tp::low_watermark( 10) );
		boost::barrier b( 2);
		boost::function< int() > fn(
			boost::bind(
				fibonacci_fn,
				10) );
		pool.submit(
			boost::bind(
				( int ( *)( boost::function< int() > const&, boost::barrier &) ) barrier_fn,
				fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		tp::task< void > t(
			pool.submit(
				boost::bind<void>(
					buffer_fibonacci_fn,
					boost::ref( buffer),
					10) ) );
		pool.submit(
			boost::bind<void>(
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
		{ t.result().get(); }
		catch ( boost::thread_interrupted const&)
		{ thrown = true; }
		catch ( ... )
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}
};

int main()
{

	fixed_bounded_channel_fifo::test_case_1();
	fixed_bounded_channel_fifo::test_case_2();
	fixed_bounded_channel_fifo::test_case_3();
	fixed_bounded_channel_fifo::test_case_4();
	fixed_bounded_channel_fifo::test_case_5();
	fixed_bounded_channel_fifo::test_case_6();
	fixed_bounded_channel_fifo::test_case_7();
	fixed_bounded_channel_fifo::test_case_8();
	fixed_bounded_channel_fifo::test_case_9();

    return boost::report_errors();
}

