
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
		> pool( tsk::poolsize( 3) );
		tsk::handle< int > h(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check id
	void test_case_3()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::handle< int > h(
			tsk::async(
				t,
				pool) );
		BOOST_CHECK_EQUAL( h.get_id(), t.get_id() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check assignment
	void test_case_4()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< int > h1;
		tsk::handle< int > h2(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		h1 = h2;
		BOOST_CHECK_EQUAL( h1.get_id(), h2.get_id() );
		BOOST_CHECK_EQUAL( h1.get(), 55);
		BOOST_CHECK_EQUAL( h2.get(), 55);
	}

	// check swap
	void test_case_5()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< int > h1(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					5),
				pool) );
		tsk::handle< int > h2(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		BOOST_CHECK_EQUAL( h1.get(), 5);
		BOOST_CHECK_EQUAL( h2.get(), 55);
		BOOST_CHECK_NO_THROW( h1.swap( h2) );
		BOOST_CHECK_EQUAL( h1.get(), 55);
		BOOST_CHECK_EQUAL( h2.get(), 5);
	}

	// check runs in pool
	void test_case_6()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< bool > h(
			tsk::async(
				tsk::make_task( runs_in_pool_fn),
				pool) );
		BOOST_CHECK_EQUAL( h.get(), true);
	}

	// check shutdown
	void test_case_7()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< int > h(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		pool.shutdown();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check runtime_error throw inside task
	void test_case_8()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task( throwing_fn),
				pool) );
		pool.shutdown();
		BOOST_CHECK_THROW( h.get(), std::runtime_error);
	}

	// check shutdown with task_rejected exception
	void test_case_9()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		pool.shutdown();
		BOOST_CHECK( pool.closed() );
		BOOST_CHECK_THROW(
			tsk::async(
				tsk::make_task(
					boost::bind(
						fibonacci_fn,
						10) ),
				pool),
			tsk::task_rejected);
	}

	// check shutdown_now with thread_interrupted exception
	void test_case_10()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					delay_fn,
					pt::millisec( 500) ),
				pool) );
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
	void test_case_11()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		tsk::handle< void > h1(
			tsk::async(
				tsk::make_task(
					barrier_fn,
					boost::ref( b) ),
				pool) );
		boost::this_thread::sleep( pt::millisec( 250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
		tsk::handle< int > h2(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 1) );
		tsk::handle< int > h3(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		boost::this_thread::sleep( pt::millisec(250) );
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 2) );
		b.wait();
		h1.get();
		BOOST_CHECK_EQUAL( h2.get(), 55);
		BOOST_CHECK_EQUAL( h3.get(), 55);
		BOOST_CHECK_EQUAL( pool.pending(), std::size_t( 0) );
	}

	// check wait
	void test_case_12()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< int > h(
			tsk::async(
				tsk::make_task(
					fibonacci_fn,
					10),
				pool) );
		h.wait();
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( h.has_value() );
		BOOST_CHECK( ! h.has_exception() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check wait_for
	void test_case_13()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					delay_fn,
					pt::seconds( 1) ),
				pool) );
		BOOST_CHECK( h.wait_for( pt::seconds( 3) ) );
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( h.has_value() );
		BOOST_CHECK( ! h.has_exception() );
	}

	// check wait_for
	void test_case_14()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					delay_fn,
					pt::seconds( 3) ),
				pool) );
		BOOST_CHECK( ! h.wait_for( pt::seconds( 1) ) );
		BOOST_CHECK( ! h.is_ready() );
		BOOST_CHECK( ! h.has_value() );
		BOOST_CHECK( ! h.has_exception() );
	}

	// check wait_until
	void test_case_15()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					delay_fn,
					pt::seconds( 1) ),
				pool) );
		BOOST_CHECK( h.wait_until( boost::get_system_time() + pt::seconds( 3) ) );
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( h.has_value() );
		BOOST_CHECK( ! h.has_exception() );
	}

	// check wait_until
	void test_case_16()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					delay_fn,
					pt::seconds( 3) ),
				pool) );
		BOOST_CHECK( ! h.wait_until( boost::get_system_time() + pt::seconds( 1) ) );
		BOOST_CHECK( ! h.is_ready() );
		BOOST_CHECK( ! h.has_value() );
		BOOST_CHECK( ! h.has_exception() );
	}

	// check interrupt
	void test_case_17()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					delay_fn,
					pt::seconds( 3) ),
				pool) );
		h.interrupt();
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check interrupt_and_wait
	void test_case_18()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		bool finished( false);
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					interrupt_fn,
					pt::seconds( 1),
					boost::ref( finished) ),
				pool) );
		h.interrupt_and_wait();
		BOOST_CHECK( finished);
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( ! h.has_value() );
		BOOST_CHECK( h.has_exception() );
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check interrupt_and_wait_for
	void test_case_19()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		bool finished( false);
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					interrupt_fn,
					pt::seconds( 1),
					boost::ref( finished) ),
				pool) );
		BOOST_CHECK( h.interrupt_and_wait_for( pt::seconds( 3) ) );
		BOOST_CHECK( finished);
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( ! h.has_value() );
		BOOST_CHECK( h.has_exception() );
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check interrupt_and_wait_for
	void test_case_20()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					non_interrupt_fn,
					3),
				pool) );
		BOOST_CHECK( ! h.interrupt_and_wait_for( pt::seconds( 1) ) );
	}

	// check interrupt_and_wait_until
	void test_case_21()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		bool finished( false);
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					interrupt_fn,
					pt::seconds( 1),
					boost::ref( finished) ),
				pool) );
		BOOST_CHECK( h.interrupt_and_wait_until( boost::get_system_time() + pt::seconds( 3) ) );
		BOOST_CHECK( finished);
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( ! h.has_value() );
		BOOST_CHECK( h.has_exception() );
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check interrupt_and_wait_until
	void test_case_22()
	{
		tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 3) );
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					non_interrupt_fn,
					3),
				pool) );
		BOOST_CHECK( ! h.interrupt_and_wait_until( boost::get_system_time() + pt::seconds( 1) ) );
	}

	// check fifo scheduling
	void test_case_23()
	{
		typedef tsk::static_pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool_type;
		BOOST_CHECK( ! tsk::has_attribute< pool_type >::value);
		pool_type pool( tsk::poolsize( 1) );
		boost::barrier b( 2);
		tsk::async(
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ),
			pool);
		std::vector< int > buffer;
		tsk::async(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			pool);
		tsk::async(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			pool);
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 55);
		BOOST_CHECK_EQUAL( buffer[1], 0);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}

	// check priority scheduling
	void test_case_24()
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
			tsk::make_task(
				barrier_fn,
				boost::ref( b) ),
			0,
			pool);
		std::vector< int > buffer;
		tsk::async(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			1,
			pool);
		tsk::async(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			0,
			pool);
		b.wait();
		pool.shutdown();
		BOOST_CHECK_EQUAL( buffer[0], 0);
		BOOST_CHECK_EQUAL( buffer[1], 55);
		BOOST_CHECK_EQUAL( buffer.size(), std::size_t( 2) );
	}

	// check smart scheduling
	void test_case_25()
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
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				10),
			2,
			pool);
		tsk::async(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				0),
			1,
			pool);
		tsk::async(
			tsk::make_task(
				buffer_fibonacci_fn,
				boost::ref( buffer),
				1),
			2,
			pool);
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
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_5, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_6, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_7, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_8, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_9, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_10, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_11, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_12, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_13, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_14, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_15, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_16, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_17, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_18, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_19, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_20, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_21, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_22, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_23, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_24, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_unbounded_pool::test_case_25, instance) );

	return test;
}
