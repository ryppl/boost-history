
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

class test_in_thread
{
public:
	// check submit
	void test_case_1()
	{
		tsk::async_handle< int > h(
			tsk::async_in_thread(
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check runs not in pool
	void test_case_2()
	{
		tsk::async_handle< bool > h(
			tsk::async_in_thread(
				tsk::make_task(
					runs_in_pool_fn) ) );
		BOOST_CHECK_EQUAL( h.get(), false);
	}

	// don't execute twice
	void test_case_3()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::async_in_thread( t);
		tsk::async_handle< int > h( tsk::async_in_thread( t) );
		BOOST_CHECK_EQUAL( h.get(), 55);
		bool thrown( false);
		try
		{ h.get(); }
		catch ( tsk::task_already_executed const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check runtime_error thrown inside the task
	void test_case_4()
	{
		tsk::async_handle< void > h(
			tsk::async_in_thread(
				tsk::make_task(
					throwing_fn) ) );
		bool thrown( false);
		try
		{ h.get(); }
		catch ( std::runtime_error const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check interruptation
	void test_case_5()
	{
		boost::barrier b( 2);
		tsk::async_handle< void > h(
			tsk::async_in_thread(
				tsk::make_task(
					barrier_fn,
					boost::ref( b) ) ) );
		h.interrupt();
		b.wait();
		bool thrown( false);
		try
		{ h.get(); }
		catch ( tsk::task_interrupted const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.Task: test suite") );

	boost::shared_ptr< test_in_thread > instance( new test_in_thread() );
	test->add( BOOST_CLASS_TEST_CASE( & test_in_thread::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_in_thread::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_in_thread::test_case_3, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_in_thread::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_in_thread::test_case_5, instance) );

	return test;
}
