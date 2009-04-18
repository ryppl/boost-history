//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////


#include "boost/async/typeof/scheduler.hpp"
#include "boost/async/algorithm.hpp"
#include <libs/async/test/data_types.hpp>
#include <libs/async/test/test_ae.hpp>

#include <iostream>
#include <boost/test/unit_test.hpp>

#include <boost/tp/unbounded_channel.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/default_pool.hpp>


using namespace boost::unit_test;
namespace basync = boost::async;
namespace bfus = boost::fusion;

//#define SCHEDULER

#ifdef SCHEDULER
typedef basync::scheduler<
  boost::tp::unbounded_channel< boost::tp::fifo >
> pool_type;
#else
//typedef boost::tp::pool<
//  boost::tp::unbounded_channel< boost::tp::fifo >
//> pool_type;
typedef boost::tp::default_pool& pool_type;
#endif

#if 0
void do_test_member_fork_detach() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_member_fork_detach(ae);
}

void do_test_member_fork() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_member_fork(ae);
}

void do_test_member_fork_bind() {
    pool_type ae(boost::tp::poolsize(2));
    aetst::do_test_member_fork_bind(ae);
}
#endif
void do_test_fork() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_fork(ae);
}

void do_test_fork_1() {   
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_fork_1(ae);
}

void do_test_creation_through_reference_wrapper()
{
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_creation_through_reference_wrapper(ae);
}

void do_test_creation_through_functor()
{
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_creation_through_functor(ae);
}
void do_test_wait() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait(ae);
}

void do_test_wait_until() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_until(ae);
}

void do_test_wait_for() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_for(ae);
}

void do_test_join() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_join(ae);
}

void do_test_join_until() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_join_until(ae);
}

void do_test_join_for() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_join_for(ae);
}

void do_test_join_all() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_join_all(ae);
}

void do_test_join_all_until() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_join_all_until(ae);
}

void do_test_join_all_for() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_join_all_for(ae);
}

void do_test_thread_interrupts_at_interruption_point() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_thread_interrupts_at_interruption_point_m(ae);
}

void do_test_wait_all() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_all(ae);
}

void do_test_wait_all_until() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_all_until(ae);
}

void do_test_wait_all_for() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_all_for(ae);
}

void do_test_wait_for_any() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_for_any(ae);
}

void do_test_set_all() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_set_all(ae);
}

void do_test_get() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_get(ae);
}


void do_test_get_all() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_set_all(ae);
}

void do_test_wait_for_all() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_wait_for_all(ae);
}
#if 0
void do_test_fork_after_wait() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_fork_after_wait(ae);
}
void do_test_fork_after_get() {
    //pool_type ae(boost::tp::poolsize(2));
    pool_type ae(boost::tp::get_default_pool());
    aetst::do_test_fork_after_get(ae);
}
#endif

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("scheduler");

#if 0     // DO NOT WORK UNTIL tp::task has a fork member
    test->add(BOOST_TEST_CASE(&do_test_member_fork_detach));
    test->add(BOOST_TEST_CASE(&do_test_member_fork));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
#endif
    test->add(BOOST_TEST_CASE(&do_test_fork));
    test->add(BOOST_TEST_CASE(&do_test_fork_1));
    test->add(BOOST_TEST_CASE(&do_test_thread_interrupts_at_interruption_point));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_functor));
#if 0     // DO NOT WORK YET
    test->add(BOOST_TEST_CASE(&do_test_creation_through_reference_wrapper));
#endif

    test->add(BOOST_TEST_CASE(&do_test_get));
    test->add(BOOST_TEST_CASE(&do_test_wait));
    test->add(BOOST_TEST_CASE(&do_test_wait_until));
    test->add(BOOST_TEST_CASE(&do_test_wait_for));

    test->add(BOOST_TEST_CASE(&do_test_wait_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_all_until));
    test->add(BOOST_TEST_CASE(&do_test_wait_all_for));
    test->add(BOOST_TEST_CASE(&do_test_set_all));
    test->add(BOOST_TEST_CASE(&do_test_get_all));

    test->add(BOOST_TEST_CASE(&do_test_join));
    test->add(BOOST_TEST_CASE(&do_test_join_until));
    test->add(BOOST_TEST_CASE(&do_test_join_for));
    test->add(BOOST_TEST_CASE(&do_test_join_all));
    test->add(BOOST_TEST_CASE(&do_test_join_all_until));
    test->add(BOOST_TEST_CASE(&do_test_join_all_for));


    test->add(BOOST_TEST_CASE(&do_test_wait_for_all));

#if 0     // DO NOT WORK YET
    test->add(BOOST_TEST_CASE(&do_test_wait_for_any));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_wait));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_get));
#endif

  return test;
}
