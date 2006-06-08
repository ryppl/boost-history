// (C) Copyright 2006 Anthony Williams
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/thread/mutex.hpp>

void test_mutex_scoped_lock_initially_locked()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m);
    
    BOOST_CHECK(lock);
    BOOST_CHECK(lock.locked());
}

void test_mutex_scoped_lock_initially_locked_with_bool_parameter_true()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m,true);
    
    BOOST_CHECK(lock);
    BOOST_CHECK(lock.locked());
}

void test_mutex_scoped_lock_initially_unlocked_with_bool_parameter_false()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m,false);
    
    BOOST_CHECK(!lock);
    BOOST_CHECK(!lock.locked());
}

void test_mutex_scoped_lock_unlocked_after_unlock_called()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m,true);
    lock.unlock();
    BOOST_CHECK(!lock);
    BOOST_CHECK(!lock.locked());
}

void test_mutex_scoped_lock_locked_after_lock_called()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m,false);
    lock.lock();
    BOOST_CHECK(lock);
    BOOST_CHECK(lock.locked());
}

void test_mutex_scoped_lock_throws_if_lock_called_when_already_locked()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m,true);
    
    BOOST_CHECK_THROW( lock.lock(), boost::lock_error );
}

void test_mutex_scoped_lock_throws_if_unlock_called_when_already_unlocked()
{
    boost::mutex m;
    boost::mutex::scoped_lock lock(m,true);
    lock.unlock();
    
    BOOST_CHECK_THROW( lock.unlock(), boost::lock_error );
}

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test_framework::test_suite* test =
        BOOST_TEST_SUITE("Boost.Threads: lock concept test suite");

    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_initially_locked));
    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_initially_locked_with_bool_parameter_true));
    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_initially_unlocked_with_bool_parameter_false));
    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_unlocked_after_unlock_called));
    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_locked_after_lock_called));
    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_throws_if_lock_called_when_already_locked));
    test->add(BOOST_TEST_CASE(&test_mutex_scoped_lock_throws_if_unlock_called_when_already_unlocked));

    return test;
}
