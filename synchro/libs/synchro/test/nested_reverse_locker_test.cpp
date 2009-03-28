//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/lockers/nested_reverse_locker.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/locks.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::unit_test;
using namespace boost::synchro;

    #define BOOST_SYNCHRONIZED_VAR(VARS_DECLARATION) if (volatile bool stop_ = false) {} else \
    for (VARS_DECLARATION; !stop_; stop_ = true)

    #define BOOST_SYNCHRONIZED(MUTEX) BOOST_SYNCHRONIZED_VAR(guard __lock(MUTEX))

    #define synchronized(MUTEX) BOOST_SYNCHRONIZED(MUTEX)

    #define atomic BOOST_SYNCHRONIZED(atomic_globla_mutex)
    
    
static boost::mutex atomic_globla_mutex;
struct guard {
    boost::mutex& m_;
    bool locked_;
    guard(boost::mutex& m): m_(m) {
        locked_ = m_.try_lock();
        std::cout << "guard:" << &m_ << std::endl;
    };
    ~guard() {
        std::cout << "~guard:" << &m_ << std::endl;
        m_.unlock();
    };
    operator bool(/*synchronized _=mtx*/) const
    {
        return locked_;
    }
};

struct non_copyable_class
    :    boost::noncopyable
{
    int value;
    boost::mutex mtx;
    typedef const guard &synchronized;
    typedef guard synchronizer;

    non_copyable_class():
        value(0)
    {}

    void inc()
    {
        synchronized (mtx) ++value;
        atomic ++value;
    }
    void dec()
    {
        synchronized(mtx) --value;
    }
    int get()
    {
        synchronized(mtx) return value;
        return 0; // not reachable
    }
};

//boost::mutex non_copyable_class::mtx;
void test_xxx() {
 
    {
    typedef const guard &try_locker;
    boost::mutex mtx;
    bool res=false;
    if (try_locker locker = mtx) {
        res=true;
        std::cout << "try_locker:OK=" << int(locker) << std::endl;
    }
    BOOST_CHECK(res);
    }
    {
    typedef const try_unique_locker<thread_mutex> &try_locker;
    boost::mutex mtx;
    bool res=false;
    try_unique_locker<boost::mutex> locker(mtx);
    if (locker) {
        res=true;
        std::cout << "try_unique_locker:OK=" << std::endl;
    }
    BOOST_CHECK(res);
    }
    non_copyable_class cnt;
    BOOST_CHECK(cnt.get()==0);
    cnt.inc();
    BOOST_CHECK(cnt.get()==2);
    std::cout << "non_copyable_class " << cnt.get() << std::endl;
    cnt.dec();
    BOOST_CHECK(cnt.get()==1);
    std::cout << "non_copyable_class " << cnt.get() << std::endl;
    
}
void test_locked() {
    thread_mutex mtx;
    unique_lock_type<thread_mutex>::type lock(mtx);
    BOOST_CHECK(lock);
    {
        boost::synchro::nested_reverse_locker<unique_lock_type<thread_mutex>::type > rlock(lock);
        BOOST_CHECK(!lock);
    }
    BOOST_CHECK(lock);
}

void test_unlocked() {
    thread_mutex mtx;
    unique_lock_type<thread_mutex>::type lock(mtx);
    BOOST_CHECK(lock);
    lock.unlock();
    BOOST_CHECK(!lock);
    {
        synchro::nested_reverse_locker<unique_lock_type<thread_mutex>::type > rlock(lock);
        BOOST_CHECK(!lock);
    }
    BOOST_CHECK(!lock);
}

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("nested_reverse_locker_tests");
  test->add(BOOST_TEST_CASE(&test_xxx));
  test->add(BOOST_TEST_CASE(&test_locked));
  test->add(BOOST_TEST_CASE(&test_unlocked));
  return test;
}

