//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

#include "boost/async/typeof/launcher.hpp"
#include "boost/async/typeof/threader.hpp"
#include "boost/async/typeof/future.hpp"
#include "boost/async/algorithm.hpp"
#include "boost/async/fork_after.hpp"
#include <boost/typeof/typeof.hpp>

#include <libs/async/test/data_types.hpp>
#include <libs/async/test/test_ae.hpp>

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
namespace basync = boost::async;
namespace bfus = boost::fusion;



void do_test_member_fork_detach() {
    basync::shared_threader ae;
    aetst::do_test_member_fork_detach(ae);
}

void do_test_member_fork() {
    basync::shared_threader ae;
    aetst::do_test_member_fork(ae);
}

#if 0
void do_test_member_lazy_fork() {
    basync::shared_threader ae;
    aetst::do_test_member_lazy_fork(ae);
}
#endif

void do_test_member_fork_move_unique() {
    basync::unique_threader ae;
    aetst::do_test_member_fork_m_fut(ae);
}

void do_test_member_fork_bind() {
    basync::shared_threader ae;
    aetst::do_test_member_fork_bind(ae);
}

void do_test_fork() {
    basync::shared_threader ae;
    aetst::do_test_fork(ae);
}

void do_test_fork_1() {
    basync::shared_threader ae;
    aetst::do_test_fork_1(ae);
}

void do_test_creation_through_reference_wrapper()
{
    basync::shared_threader ae;
    aetst::do_test_creation_through_reference_wrapper(ae);
}

void do_test_creation_through_functor()
{
    basync::shared_threader ae;
    aetst::do_test_creation_through_functor(ae);
}

void do_test_wait() {
    basync::shared_threader ae;
    aetst::do_test_wait(ae);
}

void do_test_wait_until() {
    basync::shared_threader ae;
    aetst::do_test_wait_until(ae);
}

void do_test_wait_for() {
    basync::shared_threader ae;
    aetst::do_test_wait_for(ae);
}

void do_test_join() {
    basync::shared_threader ae;
    aetst::do_test_join(ae);
}

void do_test_join_until() {
    basync::shared_threader ae;
    aetst::do_test_join_until(ae);
}

void do_test_join_for() {
    basync::shared_threader ae;
    aetst::do_test_join_for(ae);
}


void do_test_thread_interrupts_at_interruption_point() {
    basync::shared_threader ae;
    aetst::do_test_thread_interrupts_at_interruption_point(ae);
}

void do_test_join_all() {
    basync::shared_threader ae;
    aetst::do_test_join_all(ae);
}

void do_test_join_all_until() {
    basync::shared_threader ae;
    aetst::do_test_join_all_until(ae);
}

void do_test_join_all_for() {
    basync::shared_threader ae;
    aetst::do_test_join_all_for(ae);
}

void do_test_wait_all() {
    basync::shared_threader ae;
    aetst::do_test_wait_all(ae);
}

void do_test_wait_all_until() {
    basync::shared_threader ae;
    aetst::do_test_wait_all_until(ae);
}

void do_test_wait_all_for() {
    basync::shared_threader ae;
    aetst::do_test_wait_all_for(ae);
}

void do_test_wait_for_any() {
    basync::shared_threader ae;
    aetst::do_test_wait_for_any(ae);
}

template<typename AE>
struct waiter_add {
    waiter_add(boost::detail::future_waiter& waiter) : waiter_(waiter) {}
    boost::detail::future_waiter& waiter_;
    typedef void result_type;
    template<typename ACT>
    void operator()(ACT& act) const {
        waiter_.add(basync::get_future<AE>()(act));
    }
};


template<typename AE, typename FusionSequence>
unsigned my_wait_for_any(FusionSequence& seq)  {
    boost::detail::future_waiter waiter;
    boost::fusion::for_each(seq, waiter_add<AE>(waiter));
    return waiter.wait();
}

void do_test_wait_for_any_fusion_sequence()
{
    typedef basync::shared_threader  AE;
    AE ae;
    typedef basync::result_of::fork_all<AE,bfus::tuple<int(*)(),std::string(*)()> >::type type;
    type tple = basync::fork_all(ae, simple_thread, simple_string_thread);
    unsigned r = my_wait_for_any<AE>(tple);
    basync::interrupt_all(tple);
    BOOST_CHECK_EQUAL(r, 0u);
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    std::cout << "<<do_test_wait_for_any_fusion_sequence" << std::endl;

}

void do_test_get() {
    basync::shared_launcher ae;
    aetst::do_test_get(ae);
}

void do_test_get_all() {
    basync::shared_threader ae;
    aetst::do_test_set_all(ae);
}

void do_test_set_all() {
    basync::shared_threader ae;
    aetst::do_test_set_all(ae);
}

void do_test_wait_for_all() {
    basync::shared_threader ae;
    aetst::do_test_wait_for_all(ae);
}

void do_test_fork_after_join() {
    basync::shared_threader ae;
    aetst::do_test_fork_after_join(ae);
}

void do_test_fork_after_wait() {
    basync::shared_threader ae;
    aetst::do_test_fork_after_wait(ae);
}

void do_test_fork_after_get() {
    basync::shared_threader ae;
    aetst::do_test_fork_after_get(ae);
}

void do_test_other() {
    boost::mutex mtx_;
    int i=0;
    for (boost::mutex::scoped_lock lock_(mtx_), *continue_hlp_,**continue_=&continue_hlp_; continue_; continue_=0) {
        i++;
    }
    BOOST_CHECK_EQUAL(i, 1);

    for (boost::mutex::scoped_lock lock_(mtx_), *continue_=&lock_; continue_; continue_=0) {
        i++;
    }
    BOOST_CHECK_EQUAL(i, 2);
    if (bool __stop = false) {} else
    for (boost::mutex::scoped_lock lock_(mtx_) ;  !__stop; __stop= true) {
        i++;
    }
    BOOST_CHECK_EQUAL(i, 3);
}

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("shared_threader");


    test->add(BOOST_TEST_CASE(&do_test_member_fork));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
    test->add(BOOST_TEST_CASE(&do_test_fork));
    test->add(BOOST_TEST_CASE(&do_test_fork_1));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_functor));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_reference_wrapper));

    test->add(BOOST_TEST_CASE(&do_test_get));
    test->add(BOOST_TEST_CASE(&do_test_wait));
    test->add(BOOST_TEST_CASE(&do_test_wait_until));
    test->add(BOOST_TEST_CASE(&do_test_wait_for));

    test->add(BOOST_TEST_CASE(&do_test_wait_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_all_until));
    test->add(BOOST_TEST_CASE(&do_test_wait_all_for));
    test->add(BOOST_TEST_CASE(&do_test_set_all));
    test->add(BOOST_TEST_CASE(&do_test_get_all));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_detach));
    test->add(BOOST_TEST_CASE(&do_test_thread_interrupts_at_interruption_point));
    test->add(BOOST_TEST_CASE(&do_test_join));
    test->add(BOOST_TEST_CASE(&do_test_join_until));
    test->add(BOOST_TEST_CASE(&do_test_join_for));
    test->add(BOOST_TEST_CASE(&do_test_join_all));
    test->add(BOOST_TEST_CASE(&do_test_join_all_until));
    test->add(BOOST_TEST_CASE(&do_test_join_all_for));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_any));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_any_fusion_sequence));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_join));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_wait));
    test->add(BOOST_TEST_CASE(&do_test_fork_after_get));

#if 0
    test->add(BOOST_TEST_CASE(&do_test_member_lazy_fork));
    test->add(BOOST_TEST_CASE(&do_test_other));
    test_suite* test = BOOST_TEST_SUITE("unique_threader");

    test->add(BOOST_TEST_CASE(&do_test_member_fork_move_unique));
#endif
  return test;
}
