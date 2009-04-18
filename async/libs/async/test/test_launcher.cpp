//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/async/typeof/launcher.hpp>
#include <boost/async/typeof/future.hpp>
#include <boost/async/algorithm.hpp>

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <libs/async/test/data_types.hpp>
#include <libs/async/test/test_ae.hpp>

using namespace boost::unit_test;
namespace basync = boost::async;
namespace bfus = boost::fusion;

#if 0
void do_test_member_lazy_fork() {
    basync::shared_launcher ae;
    aetst::do_test_member_lazy_fork(ae);
}
#endif
void do_test_member_fork_move_unique() {
    basync::launcher ae;
    aetst::do_test_member_fork_m_fut(ae);
}

void do_test_member_fork() {
    basync::shared_launcher ae;
    aetst::do_test_member_fork(ae);
}
void do_test_member_fork_move() {
    basync::shared_launcher ae;
    aetst::do_test_member_fork_m_fut(ae);
}

void do_test_member_fork_bind() {
    basync::shared_launcher ae;
    aetst::do_test_member_fork_bind(ae);
}

void do_test_member_fork_bind_move() {
    basync::launcher ae;
    test_value=0;
    boost::unique_future<int> fut  = ae.fork(boost::bind(simple_thread_1, 2));
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}

void do_test_fork() {
    basync::shared_launcher ae;
    aetst::do_test_fork(ae);
}

void do_test_fork_move() {
    basync::launcher ae;
    test_value=0;
    boost::unique_future<int> fut = basync::fork(ae, simple_thread);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}

void do_test_fork_1() {
    basync::shared_launcher ae;
    aetst::do_test_fork_1(ae);
}
void do_test_fork_1_move() {
    basync::launcher ae;
    test_value=0;
    boost::unique_future<int> fut = fork(ae, simple_thread_1, 2);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}

void do_test_creation_through_functor()
{
    basync::shared_launcher ae;
    aetst::do_test_creation_through_functor(ae);
}

void do_test_creation_through_functor_move()
{
    basync::launcher ae;
    copyable_functor f;
    boost::unique_future<int> act=basync::fork(ae, f);

    int res = act.get();
    BOOST_CHECK_EQUAL(res, 999);
}

void do_test_creation_through_reference_wrapper()
{
    basync::shared_launcher ae;
    aetst::do_test_creation_through_reference_wrapper(ae);
}


void do_test_wait() {
    basync::shared_launcher ae;
    aetst::do_test_wait(ae);
}

void do_test_wait_until() {
    basync::shared_launcher ae;
    aetst::do_test_wait_until(ae);
}

void do_test_wait_for() {
    basync::shared_launcher ae;
    aetst::do_test_wait_for(ae);
}

void do_test_join() {
    basync::shared_launcher ae;
    aetst::do_test_join(ae);
}

void do_test_join_until() {
    basync::shared_launcher ae;
    aetst::do_test_join_until(ae);
}

void do_test_join_for() {
    basync::shared_launcher ae;
    aetst::do_test_join_for(ae);
}

void do_test_join_all() {
    basync::shared_launcher ae;
    aetst::do_test_join_all(ae);
}

void do_test_wait_all() {
    basync::shared_launcher ae;
    aetst::do_test_wait_all(ae);
}

void do_test_wait_for_any() {
    basync::shared_launcher ae;
    aetst::do_test_wait_for_any(ae);
}

void do_test_set_all() {
    basync::shared_launcher ae;
    aetst::do_test_set_all(ae);
}

void do_test_get() {
    basync::shared_launcher ae;
    aetst::do_test_get(ae);
}

void do_test_get_all() {
    basync::shared_launcher ae;
    aetst::do_test_set_all(ae);
}

void do_test_wait_for_all() {
    basync::shared_launcher ae;
    aetst::do_test_wait_for_all(ae);
}

void do_test_wait_all_until() {
    basync::shared_launcher ae;
    aetst::do_test_wait_all_until(ae);
}

void do_test_fork_after_wait() {
    basync::shared_launcher ae;
    aetst::do_test_fork_after_wait(ae);
}
void do_test_fork_after_get() {
    basync::shared_launcher ae;
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

