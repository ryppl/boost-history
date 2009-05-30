
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
#include <boost/type_traits/is_same.hpp>
#include <boost/utility.hpp>

#include <boost/task.hpp>

#include "test_functions.hpp"

namespace pt = boost::posix_time;
namespace tsk = boost::task;

class test_unbounded_pool
{
public:
	// check size, active, idle
	void test_case_1()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 3) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
	}

	// check submit
	void test_case_2()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< int > h(
			tsk::async(
				pool,
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check runs in pool
	void test_case_3()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< bool > h(
			tsk::async(
				pool,
				tsk::make_task(
					runs_in_pool_fn) ) );
		BOOST_CHECK_EQUAL( h.get(), true);
	}

	// executed twice
// 	void test_case_4()
// 	{
// 		tsk::static_pool<
// 			tsk::unbounded_channel< tsk::fifo >
// 		> pool( tsk::poolsize( 1) );
// 		tsk::task< int > t(
// 			tsk::make_task(
// 				fibonacci_fn,
// 				10) );
// 		tsk::handle< int > h1(
// 			tsk::async(
// 				pool,
// 				t) );
// 		BOOST_CHECK_EQUAL( h1.get(), 55);
// 		BOOST_CHECK_THROW(
// 			tsk::async(
// 				pool,
// 				t),
// 			tsk::task_already_executed);
// 	}

	// check shutdown
	void test_case_5()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< int > h(
			tsk::async(
				pool,
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		pool.shutdown();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check runtime_error throw inside task
	void test_case_6()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< void > h(
			tsk::async(
				pool,
				tsk::make_task(
					throwing_fn) ) );
		pool.shutdown();
		BOOST_CHECK_THROW( h.get(), std::runtime_error);
	}

	// check shutdown with task_rejected exception
	void test_case_7()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		pool.shutdown();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_THROW(
			tsk::async(
				pool,
				tsk::make_task(
					boost::bind(
						fibonacci_fn,
						10) ) ),
			tsk::task_rejected);
	}

	// check shutdown_now with thread_interrupted exception
	void test_case_8()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< void > h(
			tsk::async(
				pool,
				tsk::make_task(
					delay_fn,
					pt::millisec( 500) ) ) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		pool.shutdown_now();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_EQUAL( pool.size(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.idle(), std::size_t( 1) );
		BOOST_CHECK_EQUAL( pool.active(), std::size_t( 0) );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check pending
	void test_case_9()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		tsk::handle< void > h1(
			tsk::async(
				pool,
				tsk::make_task(
					barrier_fn,
					boost::ref( b) ) ) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
		tsk::handle< int > h2(
			tsk::async(
				pool,
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 1) );
		tsk::handle< int > h3(
			tsk::async(
				pool,
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 2) );
		b.wait();
		h1.get();
		BOOST_CHECK_EQUAL( h2.get(), 55);
		BOOST_CHECK_EQUAL( h3.get(), 55);
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
	}

	// check interruptation
	void test_case_10()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		tsk::handle< void > h(
			tsk::async(
				pool,
				tsk::make_task(
					buffer_fibonacci_fn,
					boost::ref( buffer),
					10) ) );
		tsk::async(
			pool,
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0) );
		h.interrupt();
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 1) );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check fifo scheduling
	void test_case_11()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		BOOST_CHECK( ! tsk::has_attribute< pool_type >::value);
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		tsk::async(
			pool,
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ) );
		std::vector< int > buffer;
		tsk::async(
			pool,
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10) );
		tsk::async(
			pool,
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

	// check priority scheduling
	void test_case_12()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::priority< int > >
		> pool_type;
		BOOST_CHECK( tsk::has_attribute< pool_type >::value);
		typedef boost::is_same< tsk::attribute_type< pool_type >::type, int > type;
		BOOST_CHECK( type::value);
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		tsk::async(
			pool,
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ),
			0);
		std::vector< int > buffer;
		tsk::async(
			pool,
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			1);
		tsk::async(
			pool,
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
	void test_case_13()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::smart< int, std::less< int >, tsk::replace_oldest, tsk::take_oldest > >
		> pool_type;
		BOOST_CHECK( tsk::has_attribute< pool_type >::value);
		typedef boost::is_same< tsk::attribute_type< pool_type >::type, int > type;
		BOOST_CHECK( type::value);
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		pool.submit(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ),
			0);
		std::vector< int > buffer;
		tsk::async(
			pool,
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			2);
		tsk::async(
			pool,
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			1);
		tsk::async(
			pool,
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
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.Task: test suite") );

	boost::shared_ptr< test_unbounded_pool > instance( new test_unbounded_pool() );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_3, instance) );
// 	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_5, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_6, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_7, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_8, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_9, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_10, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_11, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_12, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_13, instance) );

	return test;
}
