
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

class test_default_pool
{
public:
	// check assignment
	void test_case_1()
	{
		tsk::task< int > t(
			boost::bind(
				fibonacci_fn,
				10) );
		tsk::handle< int > h1;
		tsk::handle< int > h2(
			tsk::async( boost::move( t), tsk::default_pool() ) );
		h1 = h2;
		BOOST_CHECK_EQUAL( h1.get(), 55);
		BOOST_CHECK_EQUAL( h2.get(), 55);
	}

	// check swap
	void test_case_2()
	{
		tsk::task< int > t1(
			boost::bind(
				fibonacci_fn,
				5) );
		tsk::task< int > t2(
			boost::bind(
				fibonacci_fn,
				10) );
		tsk::handle< int > h1(
			tsk::async( boost::move( t1), tsk::default_pool() ) );
		tsk::handle< int > h2(
			tsk::async( boost::move( t2), tsk::default_pool() ) );
		BOOST_CHECK_EQUAL( h1.get(), 5);
		BOOST_CHECK_EQUAL( h2.get(), 55);
		BOOST_CHECK_NO_THROW( h1.swap( h2) );
		BOOST_CHECK_EQUAL( h1.get(), 55);
		BOOST_CHECK_EQUAL( h2.get(), 5);
	}

	// check runs in pool
	void test_case_3()
	{
		tsk::task< bool > t(
			boost::bind( runs_in_pool_fn) );
		tsk::handle< bool > h(
			tsk::async( boost::move( t), tsk::default_pool() ) );
		BOOST_CHECK_EQUAL( h.get(), true);
	}

	// check runtime_error throw inside task
	void test_case_4()
	{
		tsk::task< void > t(
			boost::bind( throwing_fn) );
		tsk::handle< void > h(
			tsk::async( boost::move( t), tsk::default_pool() ) );
		BOOST_CHECK_THROW( h.get(), std::runtime_error);
	}

	// check interrupt
	void test_case_5()
	{
		tsk::task< void > t(
			boost::bind(
				delay_fn,
				pt::seconds( 3) ) );
		tsk::handle< void > h(
			tsk::async( boost::move( t), tsk::default_pool() ) );
		h.interrupt();
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check interrupt_and_wait
	void test_case_6()
	{
		bool finished( false);
		tsk::task< void > t(
			boost::bind(
				interrupt_fn,
				pt::seconds( 3),
				boost::ref( finished) ) );
		tsk::handle< void > h(
			tsk::async( boost::move( t), tsk::default_pool() ) );
		h.interrupt_and_wait();
		BOOST_CHECK( finished);
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( ! h.has_value() );
		BOOST_CHECK( h.has_exception() );
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check wait
	void test_case_7()
	{
		tsk::task< int > t(
			boost::bind(
				fibonacci_fn,
				10) );
		tsk::handle< int > h(
			tsk::async( boost::move( t), tsk::default_pool() ) );
		h.wait();
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( h.has_value() );
		BOOST_CHECK( ! h.has_exception() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check waitfor_all()
	void test_case_8()
	{
		std::vector< tsk::handle< int > > vec;
		for ( int i = 0; i <= 5; ++i)
		{
			tsk::task< int > t(
				boost::bind(
					fibonacci_fn,
					i) );
			vec.push_back(
				tsk::async( boost::move( t), tsk::default_pool() ) );
		}
		tsk::waitfor_all( vec.begin(), vec.end() );
		BOOST_CHECK( vec[0].is_ready() );
		BOOST_CHECK( vec[1].is_ready() );
		BOOST_CHECK( vec[2].is_ready() );
		BOOST_CHECK( vec[3].is_ready() );
		BOOST_CHECK( vec[4].is_ready() );
		BOOST_CHECK( vec[5].is_ready() );
		BOOST_CHECK_EQUAL( vec[0].get(), 0);
		BOOST_CHECK_EQUAL( vec[1].get(), 1);
		BOOST_CHECK_EQUAL( vec[2].get(), 1);
		BOOST_CHECK_EQUAL( vec[3].get(), 2);
		BOOST_CHECK_EQUAL( vec[4].get(), 3);
		BOOST_CHECK_EQUAL( vec[5].get(), 5);
	}

	// check waitfor_any()
	void test_case_9()
	{
		tsk::task< void > t1(
			boost::bind(
				delay_fn,
				pt::seconds( 3) ) );
		tsk::task< int > t2(
			boost::bind(
				fibonacci_fn,
				10) );
		tsk::handle< void > h1(
			tsk::async( boost::move( t1), tsk::default_pool() ) );
		tsk::handle< int > h2(
			tsk::async( boost::move( t2), tsk::default_pool() ) );
		tsk::waitfor_any( h1, h2);
		BOOST_CHECK( ! h1.is_ready() );
		BOOST_CHECK( h2.is_ready() );
		BOOST_CHECK_EQUAL( h2.get(), 55);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.Task: test suite") );

	boost::shared_ptr< test_default_pool > instance( new test_default_pool() );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_3, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_5, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_6, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_7, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_8, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_default_pool::test_case_9, instance) );

	return test;
}
