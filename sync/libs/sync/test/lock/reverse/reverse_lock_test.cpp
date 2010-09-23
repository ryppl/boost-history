//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/lockers/reverse_lock.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/locks.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::unit_test;
using namespace boost::synchro;

void test_locked() {
    boost::mutex mtx;
    boost::unique_lock<boost::mutex> lock(mtx);
    BOOST_CHECK(lock);
    {
        boost::synchro::reverse_lock<unique_lock<boost::mutex> > rlock(lock);
        BOOST_CHECK(!lock);
    }
    BOOST_CHECK(lock);
}

void test_unlocked() {
    boost::mutex mtx;
    boost::unique_lock<boost::mutex> lock(mtx);
    BOOST_CHECK(lock);
    lock.unlock();
    BOOST_CHECK(!lock);
    {
        boost::synchro::reverse_lock<unique_lock<boost::mutex> > rlock(lock);
        BOOST_CHECK(!lock);
    }
    BOOST_CHECK(!lock);
}

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("reverse_lock_tests");
  test->add(BOOST_TEST_CASE(&test_locked));
  test->add(BOOST_TEST_CASE(&test_unlocked));
  return test;
}

