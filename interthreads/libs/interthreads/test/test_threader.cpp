//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

    
#include "boost/thread/thread_time.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"
#include <boost/thread/xtime.hpp>
#include "boost/interthreads/threader.hpp"
#include "boost/interthreads/algorithm.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
namespace bith = boost::interthreads;

namespace
{
inline boost::xtime delay(int secs, int msecs=0, int nsecs=0)
{
    const int MILLISECONDS_PER_SECOND = 1000;
    const int NANOSECONDS_PER_SECOND = 1000000000;
    const int NANOSECONDS_PER_MILLISECOND = 1000000;

    boost::xtime xt;
    if (boost::TIME_UTC != boost::xtime_get (&xt, boost::TIME_UTC))
        BOOST_ERROR ("boost::xtime_get != boost::TIME_UTC");

    nsecs += xt.nsec;
    msecs += nsecs / NANOSECONDS_PER_MILLISECOND;
    secs += msecs / MILLISECONDS_PER_SECOND;
    nsecs += (msecs % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND;
    xt.nsec = nsecs % NANOSECONDS_PER_SECOND;
    xt.sec += secs + (nsecs / NANOSECONDS_PER_SECOND);

    return xt;
}
}

void sleep(int sec)
{
	boost::xtime t;
	boost::xtime_get(&t,1);	
    t.sec += sec; 
    boost::thread::sleep(t);
}

int test_value;
int simple_thread() {
    test_value=999;
    sleep(3);
    return test_value;
}

int simple_thread_1(unsigned i) {
    test_value=i;
    sleep(5);
    return test_value;
}


void test_dir_fork() {
    bith::threader ae;
    test_value=0;
	bith::joiner<int> act  = ae.fork(simple_thread);
    BOOST_CHECK_EQUAL(act.interruption_requested(), false);
    BOOST_CHECK_EQUAL(act.is_ready(), false);
    BOOST_CHECK_EQUAL(act.has_value(), false);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
    BOOST_CHECK_EQUAL(act.joinable(), true);
    act.detach();
    BOOST_CHECK_EQUAL(act.joinable(), false);
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}    

void test_dir_fork_1() {
    bith::threader ae;
    test_value=0;
	bith::joiner<int> act  = ae.fork(boost::bind(simple_thread_1, 2));
    act.join();
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}    

void test_indir_fork() {
    bith::threader ae;
    test_value=0;
	bith::joiner<int> act = bith::fork(ae, simple_thread);
    act.join_until(delay(6));
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}    

void test_indir_fork_1() {
    bith::threader ae;
    test_value=0;
	bith::joiner<int> act = bith::fork(ae, simple_thread_1, 2);
    //act.wait_until( boost::get_system_time()+boost::posix_time::seconds(6));
    act.wait_for( boost::posix_time::seconds(6));
    BOOST_CHECK_EQUAL(act.is_ready(), true);
    BOOST_CHECK_EQUAL(act.has_value(), true);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}    

struct non_copyable_functor
    :    boost::noncopyable
{
    unsigned value;
    typedef unsigned result_type;
    
    non_copyable_functor():
        value(0)
    {}
    
    unsigned operator()()
    {
        value=999;
        return value;
    }
};
void do_test_creation_through_reference_wrapper()
{
    bith::threader ae;
    non_copyable_functor f;
	bith::joiner<unsigned> act = bith::fork(ae, boost::bind(boost::ref(f)));
    
    unsigned res = act.get();
    BOOST_CHECK_EQUAL(res, 999u);
    BOOST_CHECK_EQUAL(f.value, 999u);
}

struct copyable_functor
//    :    std::nullary_function<int>
{
    unsigned value;
    typedef int result_type;
    
    copyable_functor():
        value(0)
    {}
    
    int operator()()
    {
        value=999;
        return value;
    }
};
void do_test_creation_through_functor()
{
    bith::threader ae;
    copyable_functor f;
	bith::joiner<int> act = bith::fork(ae, f);
    
    int res = act.get();
    BOOST_CHECK_EQUAL(res, 999);
}

bool interruption_point_thread(boost::mutex* m,bool* failed)
{
    boost::mutex::scoped_lock lk(*m);
    boost::this_thread::interruption_point();
    *failed=true;
    return failed;
}

void do_test_thread_interrupts_at_interruption_point() {
    bith::threader ae;
    boost::mutex m;
    bool failed=false;
    boost::mutex::scoped_lock lk(m);
	bith::joiner<bool> act = bith::fork(ae, interruption_point_thread, &m,&failed);
    act.interrupt();
    BOOST_CHECK_EQUAL(act.interruption_requested(), true);
    lk.unlock();
    act.join();
    BOOST_CHECK(!failed);
}    

void do_test_fork_all() {
    bith::threader ae;
//	bith::result_of::fork_all<bith::threader,simple_thread,simple_thread_1(unsigned)> tple = 
//        bith::fork_all(ae, simple_thread, boost::bind(simple_thread_1, 2));
    
}

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("threader");
  test->add(BOOST_TEST_CASE(&test_dir_fork));
  test->add(BOOST_TEST_CASE(&test_dir_fork_1));
  test->add(BOOST_TEST_CASE(&test_indir_fork));
  test->add(BOOST_TEST_CASE(&test_indir_fork_1));
  test->add(BOOST_TEST_CASE(&do_test_thread_interrupts_at_interruption_point));
  test->add(BOOST_TEST_CASE(&do_test_creation_through_functor));
  test->add(BOOST_TEST_CASE(&do_test_creation_through_reference_wrapper));
  test->add(BOOST_TEST_CASE(&do_test_fork_all));
  return test;
}
