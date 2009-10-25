
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

struct dummy_mutex
{
	bool is_locked;
	
	dummy_mutex() :
	    is_locked( false)
	{}
	
	void lock()
	{ is_locked = true; }
	
	bool try_lock()
	{
	    if ( is_locked)
	        return false;
	    is_locked = true;
	    return true;
	}
	
	void unlock()
	{ is_locked = false; }
};

void test_lock()
{
	tsk::spin_mutex mtx;
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx);

	BOOST_CHECK( lk);
	BOOST_CHECK( lk.owns_lock() );

	lk.unlock();

	BOOST_CHECK( ! lk);
	BOOST_CHECK( ! lk.owns_lock() );
}

void test_defer_lock()
{
	tsk::spin_mutex mtx;
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx, boost::defer_lock);

	BOOST_CHECK( ! lk);
	BOOST_CHECK( ! lk.owns_lock() );

	lk.lock();

	BOOST_CHECK( lk);
	BOOST_CHECK( lk.owns_lock() );
}

void test_adopt_lock()
{
	tsk::spin_mutex mtx;
	mtx.lock();
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx, boost::adopt_lock);

	BOOST_CHECK( lk);
	BOOST_CHECK( lk.owns_lock() );
}

void test_try_lock()
{
	tsk::spin_mutex mtx;
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx, boost::defer_lock);

	BOOST_CHECK( ! lk);
	BOOST_CHECK( ! lk.owns_lock() );

	lk.try_lock();

	BOOST_CHECK( lk);
	BOOST_CHECK( lk.owns_lock() );
}

void test_lock_twice()
{
	tsk::spin_mutex mtx;
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx);

	BOOST_CHECK_THROW( lk.lock(), tsk::lock_error);
}

void test_try_lock_twice()
{
	tsk::spin_mutex mtx;
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx);

	BOOST_CHECK_THROW( lk.try_lock(), tsk::lock_error);
}

void test_unlock_twice()
{
	tsk::spin_mutex mtx;
	tsk::spin_unique_lock< tsk::spin_mutex > lk( mtx);
	lk.unlock();

	BOOST_CHECK_THROW( lk.unlock(), tsk::lock_error);
}

void test_default_ctor()
{
	tsk::spin_unique_lock< tsk::spin_mutex > lk;

	BOOST_CHECK( ! lk);
	BOOST_CHECK( ! lk.owns_lock() );
}

void test_lock_concept()
{
    tsk::spin_mutex mtx1, mtx2, mtx3;

    tsk::spin_mutex::scoped_lock lk1( mtx1, boost::defer_lock),
        lk2( mtx2, boost::defer_lock),
        lk3( mtx3, boost::defer_lock);

    BOOST_CHECK( ! lk1.owns_lock() );
    BOOST_CHECK( ! lk2.owns_lock() );
    BOOST_CHECK( ! lk3.owns_lock() );
    
    boost::lock( lk1, lk2, lk3);
    
    BOOST_CHECK( lk1.owns_lock() );
    BOOST_CHECK( lk2.owns_lock() );
    BOOST_CHECK( lk3.owns_lock() );
}

void test_try_lock_concept()
{
    dummy_mutex mtx1, mtx2;
    mtx2.lock();

    tsk::spin_unique_lock< dummy_mutex > lk1( mtx1, boost::defer_lock),
        lk2( mtx2, boost::defer_lock);

    int res = boost::try_lock( lk1, lk2);
    
    BOOST_CHECK( res == 1);
    BOOST_CHECK( ! mtx1.is_locked);
    BOOST_CHECK( mtx2.is_locked);
    BOOST_CHECK( ! lk1.owns_lock() );
    BOOST_CHECK( ! lk2.owns_lock() );
}

void test_swap()
{
	tsk::spin_mutex mtx1, mtx2;
	
	tsk::spin_unique_lock< tsk::spin_mutex > lk1( mtx1), lk2( mtx2);
	
	BOOST_CHECK_EQUAL( lk1.mutex(), & mtx1);
	BOOST_CHECK_EQUAL( lk2.mutex(), & mtx2);
	
	lk1.swap( lk2);
	
	BOOST_CHECK_EQUAL( lk1.mutex(), & mtx2);
	BOOST_CHECK_EQUAL( lk2.mutex(), & mtx1);
}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
    boost::unit_test_framework::test_suite * test =
		BOOST_TEST_SUITE("Boost.Task: spin-lock test suite");

    test->add( BOOST_TEST_CASE( & test_lock) );
    test->add( BOOST_TEST_CASE( & test_defer_lock) );
    test->add( BOOST_TEST_CASE( & test_adopt_lock) );
    test->add( BOOST_TEST_CASE( & test_try_lock) );
    test->add( BOOST_TEST_CASE( & test_lock_twice) );
    test->add( BOOST_TEST_CASE( & test_try_lock_twice) );
    test->add( BOOST_TEST_CASE( & test_unlock_twice) );
    test->add( BOOST_TEST_CASE( & test_default_ctor) );
    test->add( BOOST_TEST_CASE( & test_lock_concept) );
    test->add( BOOST_TEST_CASE( & test_try_lock_concept) );
    test->add( BOOST_TEST_CASE( & test_swap) );

	return test;
}
