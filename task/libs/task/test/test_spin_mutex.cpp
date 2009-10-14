
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// This test is based on the tests of Boost.Thread 

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
#include <boost/utility.hpp>

#include <boost/task.hpp>

namespace pt = boost::posix_time;
namespace tsk = boost::task;

namespace {

void test_lock()
{
	tsk::spin_mutex mtx;
	tsk::spin_condition cond;
	
	{
		tsk::spin_mutex::scoped_lock lk( mtx);
	    BOOST_CHECK( lk);
	}
	tsk::spin_mutex::scoped_lock lk( mtx);
	BOOST_CHECK( lk);
	
	BOOST_CHECK( ! cond.timed_wait( lk, pt::millisec( 250) ) );
	BOOST_CHECK( lk);
	
	lk.unlock();
	BOOST_CHECK( ! lk);
	lk.lock();
	BOOST_CHECK( lk);
};

}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
    boost::unit_test_framework::test_suite * test =
		BOOST_TEST_SUITE("Boost.Task: spin-mutex test suite");

    test->add( BOOST_TEST_CASE( & test_lock) );

	return test;
}
