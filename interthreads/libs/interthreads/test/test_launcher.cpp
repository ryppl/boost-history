//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/typeof/launcher.hpp>
#include <boost/interthreads/typeof/future.hpp>
#include <boost/interthreads/algorithm.hpp>

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <libs/interthreads/test/data_types.hpp>
#include <libs/interthreads/test/test_ae.hpp>

using namespace boost::unit_test;
namespace bith = boost::interthreads;
namespace bfus = boost::fusion;

#if 0
void do_test_member_lazy_fork() {
    bith::shared_launcher ae;
    aetst::do_test_member_lazy_fork(ae);
}
#endif
void do_test_member_fork_move_unique() {
    bith::launcher ae;
    aetst::do_test_member_fork_m_fut(ae);
}

void do_test_member_fork() {
    bith::shared_launcher ae;
    aetst::do_test_member_fork(ae);
}
void do_test_member_fork_move() {
    bith::shared_launcher ae;
    aetst::do_test_member_fork_m_fut(ae);
}

void do_test_member_fork_bind() {
    bith::shared_launcher ae;
    aetst::do_test_member_fork_bind(ae);
}

void do_test_member_fork_bind_move() {
    bith::launcher ae;
    test_value=0;
    boost::unique_future<int> fut  = ae.fork(boost::bind(simple_thread_1, 2));
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}

void do_test_fork() {
    bith::shared_launcher ae;
    aetst::do_test_fork(ae);
}

void do_test_fork_move() {
    bith::launcher ae;
    test_value=0;
    boost::unique_future<int> fut = bith::fork(ae, simple_thread);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}

void do_test_fork_1() {
    bith::shared_launcher ae;
    aetst::do_test_fork_1(ae);
}
void do_test_fork_1_move() {
    bith::launcher ae;
    test_value=0;
    boost::unique_future<int> fut = fork(ae, simple_thread_1, 2);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}

void do_test_creation_through_functor()
{
    bith::shared_launcher ae;
    aetst::do_test_creation_through_functor(ae);
}

void do_test_creation_through_functor_move()
{
    bith::launcher ae;
    copyable_functor f;
    boost::unique_future<int> act=bith::fork(ae, f);

    int res = act.get();
    BOOST_CHECK_EQUAL(res, 999);
}

void do_test_creation_through_reference_wrapper()
{
    bith::shared_launcher ae;
    aetst::do_test_creation_through_reference_wrapper(ae);
}


void do_test_wait() {
    bith::shared_launcher ae;
    aetst::do_test_wait(ae);
}

void do_test_wait_until() {
    bith::shared_launcher ae;
    aetst::do_test_wait_until(ae);
}

void do_test_wait_for() {
    bith::shared_launcher ae;
    aetst::do_test_wait_for(ae);
}

void do_test_join() {
    bith::shared_launcher ae;
    aetst::do_test_join(ae);
}

void do_test_join_until() {
    bith::shared_launcher ae;
    aetst::do_test_join_until(ae);
}

void do_test_join_for() {
    bith::shared_launcher ae;
    aetst::do_test_join_for(ae);
}

void do_test_join_all() {
    bith::shared_launcher ae;
    aetst::do_test_join_all(ae);
}

void do_test_wait_all() {
    bith::shared_launcher ae;
    aetst::do_test_wait_all(ae);
}

void do_test_wait_for_any() {
    bith::shared_launcher ae;
    aetst::do_test_wait_for_any(ae);
}

void do_test_set_all() {
    bith::shared_launcher ae;
    aetst::do_test_set_all(ae);
}

void do_test_get() {
    bith::shared_launcher ae;
    aetst::do_test_get(ae);
}

void do_test_get_all() {
    bith::shared_launcher ae;
    aetst::do_test_set_all(ae);
}

void do_test_wait_for_all() {
    bith::shared_launcher ae;
    aetst::do_test_wait_for_all(ae);
}

void do_test_wait_all_until() {
    bith::shared_launcher ae;
    aetst::do_test_wait_all_until(ae);
}

void do_test_fork_after_wait() {
    bith::shared_launcher ae;
    aetst::do_test_fork_after_wait(ae);
}
void do_test_fork_after_get() {
    bith::shared_launcher ae;
    aetst::do_test_fork_after_get(ae);
}

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("launcher");

    test->add(BOOST_TEST_CASE(&do_test_member_fork));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_move));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind_move));
    test->add(BOOST_TEST_CASE(&do_test_fork));
    test->add(BOOST_TEST_CASE(&do_test_fork_move));
    test->add(BOOST_TEST_CASE(&do_test_fork_1));
    test->add(BOOST_TEST_CASE(&do_test_fork_1_move));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_functor));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_reference_wrapper));

    test->add(BOOST_TEST_CASE(&do_test_get));
    test->add(BOOST_TEST_CASE(&do_test_wait));
    test->add(BOOST_TEST_CASE(&do_test_wait_until));
    test->add(BOOST_TEST_CASE(&do_test_wait_for));
    test->add(BOOST_TEST_CASE(&do_test_wait_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_all_until));
    test->add(BOOST_TEST_CASE(&do_test_set_all));
    test->add(BOOST_TEST_CASE(&do_test_get_all));

    test->add(BOOST_TEST_CASE(&do_test_join));
    test->add(BOOST_TEST_CASE(&do_test_join_until));
    test->add(BOOST_TEST_CASE(&do_test_join_for));
    test->add(BOOST_TEST_CASE(&do_test_join_all));
    //test->add(BOOST_TEST_CASE(&do_test_join_all_until));
    //test->add(BOOST_TEST_CASE(&do_test_join_all_for));


    test->add(BOOST_TEST_CASE(&do_test_wait_for_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_any));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_wait));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_get));

#if 0
    test->add(BOOST_TEST_CASE(&do_test_member_lazy_fork));
#endif
    return test;
}

