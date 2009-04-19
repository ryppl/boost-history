
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

class test_launch
{
public:
	// launch in default pool
	void test_case_1()
	{
		tsk::handle< int > h(
			tsk::launch(
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// launch in custom pool
	void test_case_2()
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );
		tsk::handle< int > h(
			tsk::launch(
				pool,
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// launch in current thread
	void test_case_3()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		t();
		BOOST_CHECK_EQUAL( t.get(), 55);
	}

	// don't execute twice
	void test_case_4()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		t();
		BOOST_CHECK_EQUAL( t.get(), 55);
		t();
		bool thrown( false);
		try
		{ t.get(); }
		catch ( tsk::task_already_executed const&)
		{ thrown = true; }
		BOOST_CHECK( thrown);
	}

	// check runs in pool
	void test_case_5()
	{
		tsk::handle< bool > h(
			tsk::launch(
				tsk::make_task(
					runs_in_pool_fn) ) );
		BOOST_CHECK_EQUAL( h.get(), true);
	}

	// check runs not in pool
	void test_case_6()
	{
		tsk::task< bool > t(
			tsk::make_task(
				runs_in_pool_fn) );
		t();
		BOOST_CHECK_EQUAL( t.get(), false);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.ThreadPool: pool (with unbounded channel) test suite") );

	boost::shared_ptr< test_launch > instance( new test_launch() );
	test->add( BOOST_CLASS_TEST_CASE( & test_launch::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_launch::test_case_2, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_launch::test_case_3, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_launch::test_case_4, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_launch::test_case_5, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_launch::test_case_6, instance) );

	return test;
}
