
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
	// check id
	void test_case_1()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::handle< int > h(
			tsk::async(
				tsk::default_pool(),
				t) );
		BOOST_CHECK_EQUAL( h.get_id(), t.get_id() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// check runs in pool
	void test_case_2()
	{
		tsk::handle< bool > h(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					runs_in_pool_fn) ) );
		BOOST_CHECK_EQUAL( h.get(), true);
	}

	// executed twice
	void test_case_3()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::handle< int > h1(
			tsk::async(
				tsk::default_pool(),
				t) );
		BOOST_CHECK_EQUAL( h1.get(), 55);
		tsk::handle< int > h2(
			tsk::async(
				tsk::default_pool(),
				t) );
		BOOST_CHECK_EQUAL( h2.get(), 55);
	}

	// check runtime_error throw inside task
	void test_case_4()
	{
		tsk::handle< void > h(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					throwing_fn) ) );
		BOOST_CHECK_THROW( h.get(), std::runtime_error);
	}

	// check interrupt
	void test_case_5()
	{
		tsk::handle< void > h(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					delay_fn,
					pt::seconds( 3) ) ) );
		h.interrupt();
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check interrupt_and_wait
	void test_case_6()
	{
		bool finished( false);
		tsk::handle< void > h(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					interrupt_fn,
					pt::seconds( 3),
					boost::ref( finished) ) ) );
		h.interrupt_and_wait();
		BOOST_CHECK( finished);
		BOOST_CHECK( h.is_ready() );
		BOOST_CHECK( h.interruption_requested() );
		BOOST_CHECK_THROW( h.get(), tsk::task_interrupted);
	}

	// check waitfor_all()
	void test_case_7()
	{
		std::vector< tsk::handle< int > > vec;
		for ( int i = 0; i <= 5; ++i)
			vec.push_back(
				tsk::async(
					tsk::default_pool(),
					tsk::make_task(
						fibonacci_fn,
						i) ) );
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
	void test_case_8()
	{
		tsk::handle< void > h1(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					delay_fn,
					pt::seconds( 3) ) ) );
		tsk::handle< int > h2(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					fibonacci_fn,
					10) ) );
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

	return test;
}
