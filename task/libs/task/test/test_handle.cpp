
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

class test_handle
{
public:
	// test id
	void test_case_1()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::handle< int > h( t.get_handle() );
		tsk::launch( t);
		BOOST_CHECK_EQUAL( t.get_id(), h.get_id() );
		BOOST_CHECK_EQUAL( h.get(), 55);
	}

	// test handle
	void test_case_2()
	{
		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::handle< int > h1( t.get_handle() );
		tsk::handle< int > h2( t.get_handle() );
		tsk::launch( t);
		BOOST_CHECK_EQUAL( t.get_id(), h1.get_id() );
		BOOST_CHECK_EQUAL( t.get_id(), h2.get_id() );
		BOOST_CHECK_EQUAL( h1.get_id(), h2.get_id() );
		BOOST_CHECK_EQUAL( h1 == h2, true);
	}
};

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
	boost::unit_test::test_suite * test( BOOST_TEST_SUITE("Boost.Task: test suite") );

	boost::shared_ptr< test_handle > instance( new test_handle() );
	test->add( BOOST_CLASS_TEST_CASE( & test_handle::test_case_1, instance) );
	test->add( BOOST_CLASS_TEST_CASE( & test_handle::test_case_2, instance) );

	return test;
}
