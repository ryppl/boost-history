
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

struct condition_test
{
    condition_test() :
		notified( 0),
		awoken( 0)
	{}

    tsk::spin_mutex		mtx;
    tsk::spin_condition	condition;
    int notified;
    int awoken;
};

struct cond_predicate
{
    cond_predicate( int & var_, int val_) :
		var( var_),
		val( val_)
	{}

    bool operator()()
	{ return var == val; }

    int	&	var;
    int		val;

private:
    void operator=( cond_predicate&);
};

void condition_test_thread( condition_test * data)
{
	tsk::spin_mutex::scoped_lock lk( data->mtx);
    BOOST_CHECK( lk);
    while ( ! ( data->notified > 0) )
        data->condition.wait( lk);
    BOOST_CHECK( lk);
    data->awoken++;
}

void condition_test_waits( condition_test * data)
{
	tsk::spin_mutex::scoped_lock lk( data->mtx);
    BOOST_CHECK( lk);

    // Test wait.
    while ( data->notified != 1)
        data->condition.wait( lk);
    BOOST_CHECK( lk);
    BOOST_CHECK_EQUAL( data->notified, 1);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate wait.
    data->condition.wait( lk, cond_predicate( data->notified, 2));
    BOOST_CHECK( lk);
    BOOST_CHECK_EQUAL( data->notified, 2);
    data->awoken++;
    data->condition.notify_one();

    // Test timed_wait.
	pt::millisec xt( 250);
    while ( data->notified != 3)
        data->condition.timed_wait( lk, xt);
    BOOST_CHECK( lk);
    BOOST_CHECK_EQUAL( data->notified, 3);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate timed_wait.
    cond_predicate pred( data->notified, 4);
    BOOST_CHECK( data->condition.timed_wait( lk, xt, pred));
    BOOST_CHECK( lk);
    BOOST_CHECK( pred() );
    BOOST_CHECK_EQUAL( data->notified, 4);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate timed_wait with relative timeout
    cond_predicate pred_rel( data->notified, 5);
    BOOST_CHECK( data->condition.timed_wait( lk, pt::seconds( 10), pred_rel) );
    BOOST_CHECK( lk);
    BOOST_CHECK( pred_rel() );
    BOOST_CHECK_EQUAL( data->notified, 5);
    data->awoken++;
    data->condition.notify_one();
}

void test_wait()
{
    condition_test	data;

    boost::thread thrd(
			boost::bind(
				& condition_test_waits, & data) );

    {
		tsk::spin_mutex::scoped_lock lk( data.mtx);
        BOOST_CHECK( lk);

        boost::this_thread::sleep( pt::millisec( 250) );
        data.notified++;
        data.condition.notify_one();
        while ( data.awoken != 1)
            data.condition.wait( lk);
        BOOST_CHECK( lk);
        BOOST_CHECK_EQUAL( data.awoken, 1);

        boost::this_thread::sleep( pt::millisec( 250) );
        data.notified++;
        data.condition.notify_one();
        while ( data.awoken != 2)
            data.condition.wait( lk);
        BOOST_CHECK( lk);
        BOOST_CHECK_EQUAL( data.awoken, 2);

        boost::this_thread::sleep( pt::millisec( 250) );
        data.notified++;
        data.condition.notify_one();
        while ( data.awoken != 3)
            data.condition.wait( lk);
        BOOST_CHECK( lk);
        BOOST_CHECK_EQUAL( data.awoken, 3);

        boost::this_thread::sleep( pt::millisec( 250) );
        data.notified++;
        data.condition.notify_one();
        while ( data.awoken != 4)
            data.condition.wait( lk);
        BOOST_CHECK( lk);
        BOOST_CHECK_EQUAL( data.awoken, 4);

        boost::this_thread::sleep( pt::millisec( 250) );
        data.notified++;
        data.condition.notify_one();
        while ( data.awoken != 5)
            data.condition.wait( lk);
        BOOST_CHECK( lk);
        BOOST_CHECK_EQUAL( data.awoken, 5);
    }

    thrd.join();
    BOOST_CHECK_EQUAL( data.awoken, 5);
}


void test_interruption_point()
{
    condition_test	data;

    boost::thread thrd(
			boost::bind(
				& condition_test_thread, & data) );

    thrd.interrupt();
    thrd.join();

    BOOST_CHECK_EQUAL( data.awoken, 0);
}

}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
    boost::unit_test_framework::test_suite * test =
		BOOST_TEST_SUITE("Boost.Task: spin-condition test suite");

    test->add( BOOST_TEST_CASE( & test_wait) );
    test->add( BOOST_TEST_CASE( & test_interruption_point) );

	return test;
}
