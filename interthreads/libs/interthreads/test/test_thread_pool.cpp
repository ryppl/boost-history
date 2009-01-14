//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

    
#include "boost/fusion/include/vector_tie.hpp"
#include "boost/thread/thread_time.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"
#include <boost/thread/xtime.hpp>
#include "boost/interthreads/typeof/launcher.hpp"
#include "boost/interthreads/typeof/threader.hpp"
#include "boost/interthreads/typeof/scheduler.hpp"
#include "boost/interthreads/typeof/future.hpp"
#include "boost/interthreads/algorithm.hpp"
#include <boost/typeof/typeof.hpp>
#include <libs/interthreads/test/data_types.hpp>
#include <libs/interthreads/test/test_ae.hpp>

#include <iostream>
#include <boost/test/unit_test.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/tp/unbounded_channel.hpp>
#include <boost/tp/fifo.hpp>


using namespace boost::unit_test;
namespace bith = boost::interthreads;
namespace bfus = boost::fusion;

#define SCHEDULER

#if SCHEDULER
typedef bith::scheduler<
  boost::tp::unbounded_channel< boost::tp::fifo >
> pool_type;
#else
typedef boost::tp::pool<
  boost::tp::unbounded_channel< boost::tp::fifo >
> pool_type;
#endif

void do_test_member_fork() {  
    boost::tp::poolsize s(2);
    pool_type ae(s);
    aetst::do_test_member_fork(ae);
}    


void do_test_member_fork_bind() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_member_fork_bind(ae);
}    
void do_test_fork() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_fork(ae);
}    

void do_test_fork_1() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_fork_1(ae);
}    

void do_test_creation_through_reference_wrapper()
{
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_creation_through_reference_wrapper(ae);
}

void do_test_creation_through_functor()
{
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_creation_through_functor(ae);
}


void do_test_thread_interrupts_at_interruption_point() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_thread_interrupts_at_interruption_point(ae);
}    

void do_test_wait_all() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_wait_all(ae);
}

void do_test_wait_for_any() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_wait_for_any(ae);
}

void do_test_set_all() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_set_all(ae);
}

void do_test_wait_for_all() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_wait_for_all(ae);
}

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("shared_threader");
    
    test->add(BOOST_TEST_CASE(&do_test_member_fork));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
    test->add(BOOST_TEST_CASE(&do_test_fork));
    test->add(BOOST_TEST_CASE(&do_test_fork_1));
    test->add(BOOST_TEST_CASE(&do_test_thread_interrupts_at_interruption_point));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_functor));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_reference_wrapper));
    test->add(BOOST_TEST_CASE(&do_test_wait_all));
    //test->add(BOOST_TEST_CASE(&do_test_wait_for_any));  FAILS
    test->add(BOOST_TEST_CASE(&do_test_set_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_all));
  return test;
}
