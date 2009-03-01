//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_INTERTHREADS_TEST_AE__HPP
#define BOOST_INTERTHREADS_TEST_AE__HPP

#include <boost/interthreads/algorithm.hpp>
#include <libs/interthreads/test/data_types.hpp>
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"
#include "boost/thread/thread.hpp"
#include <boost/typeof/typeof.hpp>

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace boost::unit_test;
namespace bith = boost::interthreads;
namespace bfus = boost::fusion;

int test_value;
int test_value1;
int test_value2;
int test_value3;
int simple_thread() {
    //std::cout << ">> simple_thread" << std::endl;
    test_value=999;
    sleep(2);
    //std::cout << "<< simple_thread" << std::endl;
    return test_value;
}
static std::string test_string_value;

std::string simple_string_thread() {
    test_string_value="999";
    sleep(5);
    return test_string_value;
}

int simple_thread2() {
    //std::cout << ">>simple_thread2" << std::endl;
    test_value2=111;
    sleep(5);
    //std::cout << "<<simple_thread2" << std::endl;
    return test_value2;
}

int simple_thread_1(unsigned i) {
    test_value=i;
    sleep(5);
    return test_value;
}

bool interruption_point_thread(boost::mutex* m,bool* failed)
{
    boost::mutex::scoped_lock lk(*m);
    boost::this_thread::interruption_point();
    *failed=true;
    return failed;
}

int my_simple_thread() {
    test_value1=111;
    sleep(2);
    return test_value1;
}

int my_simple_thread2() {
    test_value2=222;
    sleep(3);
    return test_value2;
}

int my_simple_thread3() {
    test_value3=333;
    sleep(1);
    return test_value3;
}

int my_simple_thread4(int i, std::string s) {
    test_value3=333;
    sleep(1);
    return test_value3;
}

namespace aetst {
#if 0
template <typename AE>
void do_test_member_lazy_fork(AE& ae) {
    test_value=0;
    //BOOST_AUTO(act2, bith::lazy_fork(ae, simple_thread));
    //boost::packaged_task<int> act2(bith::lazy_fork(ae, simple_thread));
    //BOOST_AUTO(act, act2.get_future());
    boost::shared_ptr<boost::packaged_task<int> > tsk(new boost::packaged_task<int>(simple_thread));
    tsk->set_wait_callback(bith::detail::invoke_lazy_task<int>());
    boost::shared_ptr<boost::packaged_task<int> > tsk2(tsk);
    boost::unique_future<int> act=tsk2->get_future();
    int res_value = act.get();
    #if 1
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    BOOST_CHECK_EQUAL(act.is_ready(), true);
    BOOST_CHECK_EQUAL(act.has_value(), true);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
    #endif
}
#endif
template <typename AE>
void do_test_member_fork(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, ae.fork(simple_thread));
    BOOST_CHECK_EQUAL(bith::is_ready(act), false);
    BOOST_CHECK_EQUAL(bith::has_value(act), false);
    BOOST_CHECK_EQUAL(bith::has_exception(act), false);
    int res_value = bith::get(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    BOOST_CHECK_EQUAL(bith::is_ready(act), true);
    BOOST_CHECK_EQUAL(bith::has_value(act), true);
    BOOST_CHECK_EQUAL(bith::has_exception(act), false);
    std::cout << "<<do_test_member_fork" << std::endl;

}
template <typename AE>
void do_test_member_fork_thr(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, ae.fork(simple_thread));
    BOOST_CHECK_EQUAL(bith::interruption_requested(act), false);
    BOOST_CHECK_EQUAL(bith::joinable(act), true);
    bith::join(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(bith::interruption_requested(act), false);
    BOOST_CHECK_EQUAL(bith::joinable(act), false);
    std::cout << "<<do_test_member_fork" << std::endl;

}
template <typename AE>
void do_test_member_fork_m_fut(AE& ae) {
    test_value=0;
    typename AE::template handle<int>::type act = ae.fork(simple_thread);
    BOOST_CHECK_EQUAL(act.is_ready(), false);
    BOOST_CHECK_EQUAL(act.has_value(), false);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
    int res_value = bith::get(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    BOOST_CHECK_EQUAL(bith::is_ready(act), true);
    BOOST_CHECK_EQUAL(bith::has_value(act), true);
    BOOST_CHECK_EQUAL(bith::has_exception(act), false);
    std::cout << "<<do_test_member_fork_m_fut" << std::endl;
}
template <typename AE>
void do_test_member_fork_m_thr(AE& ae) {
    test_value=0;
    typename AE::template handle<int>::type act;
    act = ae.fork(simple_thread);
    bith::join(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    std::cout << "<<do_test_member_fork_m_thr" << std::endl;
}

template <typename AE>
void do_test_member_fork_bind(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, ae.fork(boost::bind(simple_thread_1, 2)));
    bith::wait(act);
    BOOST_CHECK_EQUAL(test_value, 2);
    std::cout << "<<do_test_member_fork_bind" << std::endl;
}

template <typename AE>
void do_test_fork(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, bith::fork(ae, simple_thread));
    int res_value = bith::get(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    std::cout << "<<do_test_fork" << std::endl;
}

template <typename AE>
void do_test_fork_thr(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, bith::fork(ae, simple_thread));
    bith::join(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    std::cout << "<<do_test_fork_thr" << std::endl;
}

template <typename AE>
void do_test_fork_1(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, bith::fork<AE>(ae, simple_thread_1, 2));
    int res_value = bith::get(act);
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
    std::cout << "<<do_test_fork_1" << std::endl;
}

template <typename AE>
void do_test_fork_1_thr(AE& ae) {
    test_value=0;
    BOOST_AUTO(act, bith::fork(ae, simple_thread_1, 2));
    bith::wait(act);
    BOOST_CHECK_EQUAL(test_value, 2);
    std::cout << "<<do_test_fork_1" << std::endl;
}

template <typename AE>
void do_test_creation_through_functor(AE& ae)
{
    copyable_functor f;
    BOOST_AUTO(act,bith::fork(ae, f));
    int res_value = bith::get(act);
    BOOST_CHECK_EQUAL(res_value, 999);
    std::cout << "<<do_test_creation_through_functor" << std::endl;
}

template <typename AE>
void do_test_creation_through_functor_thr(AE& ae)
{
    copyable_functor f;
    BOOST_AUTO(act,bith::fork(ae, f));
    bith::wait(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    std::cout << "<<do_test_creation_through_functor_thr" << std::endl;
}

template <typename AE>
void do_test_creation_through_reference_wrapper(AE& ae)
{
    non_copyable_functor f;
    BOOST_AUTO(act,bith::fork(ae, boost::bind(boost::ref(f))));

    unsigned res_value = bith::get(act);
    BOOST_CHECK_EQUAL(res_value, 999u);
    BOOST_CHECK_EQUAL(f.value, 999u);
    std::cout << "<<do_test_creation_through_reference_wrapper" << std::endl;
}

template <typename AE>
void do_test_creation_through_reference_wrapper_thr(AE& ae)
{
    non_copyable_functor f;
    BOOST_AUTO(act,bith::fork(ae, boost::bind(boost::ref(f))));
    bith::wait(act);
    BOOST_CHECK_EQUAL(f.value, 999u);
    std::cout << "<<do_test_creation_through_reference_wrapper_thr" << std::endl;
}

template <typename AE>
void do_test_wait(AE& ae) {
    BOOST_AUTO(act,bith::fork(ae, simple_thread));
    bith::wait(act);
    std::cout << "<<do_test_wait" << std::endl;
}

template <typename AE>
void do_test_wait_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bith::wait_all(tple);
    std::cout << "<<do_test_wait_all" << std::endl;
}

template <typename AE>
void do_test_wait_until(AE& ae) {
    BOOST_AUTO(act,bith::fork(ae, simple_thread));
    bool b = bith::wait_until(act,boost::get_system_time()+boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::wait_until(act,boost::get_system_time()+boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_wait_until" << std::endl;
}

template <typename AE>
void do_test_wait_all_until(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bool b = bith::wait_all_until(tple,boost::get_system_time()+boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::wait_all_until(tple,boost::get_system_time()+boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_wait_all_until" << std::endl;
}

template <typename AE>
void do_test_wait_for(AE& ae) {
    BOOST_AUTO(tple,bith::fork(ae, simple_thread));
    bool b = bith::wait_for(tple,boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::wait_for(tple,boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_wait_for" << std::endl;
}

template <typename AE>
void do_test_wait_all_for(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bool b = bith::wait_all_for(tple,boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::wait_all_for(tple,boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_wait_all_for" << std::endl;
}

template <typename AE>
void do_test_wait_for_any(AE& ae) {
    std::cout << ">>do_test_wait_for_any" << std::endl;
    BOOST_AUTO(res, bith::wait_for_any(ae, simple_thread2, simple_thread));
    BOOST_CHECK_EQUAL(res.first, 1u);
    BOOST_CHECK_EQUAL(res.second, 999);
    res = bith::wait_for_any(ae, simple_thread, simple_thread2);
    BOOST_CHECK_EQUAL(res.first, 0u);
    BOOST_CHECK_EQUAL(res.second, 999);
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    std::cout << "<<do_test_wait_for_any" << std::endl;


}

template <typename AE>
void do_test_set_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread2));
    bfus::tuple<int,int> res;
    bith::set_all(tple,res);
    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 999);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 111);

    //bfus::for_each(res, print_xml());
    //std::cout << std::endl;
    std::cout << "<<do_test_fork_1" << std::endl;

}
template <typename AE>
void do_test_get(AE& ae) {
    BOOST_AUTO(act,bith::fork(ae, simple_thread));
    bith::get(act);
    BOOST_AUTO(res_value,bith::get(act));
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    std::cout << "<<do_test_get" << std::endl;
}

template <typename AE>
void do_test_get_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread2));
//    bith::wait_all(tple);
    BOOST_AUTO(res,bith::get_all(tple));
    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 999);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 111);
    //bfus::for_each(res, print_xml());
    //std::cout << std::endl;
    std::cout << "<<do_test_get_all" << std::endl;
}

template <typename AE>
void do_test_wait_for_all(AE& ae) {
    BOOST_AUTO(res, bith::wait_for_all(ae, simple_thread, simple_thread2));
    //bfus::for_each(res, print_xml());
    //std::cout << std::endl;

    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 999);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 111);
    res = bith::wait_for_all(ae, simple_thread2, simple_thread);
    //bfus::for_each(res, print_xml());
    //std::cout << std::endl;

    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 111);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 999);
    std::cout << "<<do_test_wait_for_all" << std::endl;
}


///////////////

template <typename AE>
void do_test_member_fork_detach(AE& ae) {
    test_value=0;

    BOOST_AUTO(act,ae.fork(simple_thread));
    BOOST_CHECK_EQUAL(bith::interruption_requested(act), false);
    BOOST_CHECK_EQUAL(bith::joinable(act), true);
    bith::detach(act);
    BOOST_CHECK_EQUAL(bith::joinable(act), false);
    bith::join(act);
    BOOST_CHECK_EQUAL(test_value, 999);
    std::cout << "<<do_test_member_fork_detach" << std::endl;
}

template <typename AE>
void do_test_join(AE& ae) {
    BOOST_AUTO(act,bith::fork(ae, simple_thread));
    bith::join(act);
    std::cout << "<<do_test_join" << std::endl;
}

template <typename AE>
void do_test_join_m(AE& ae) {
    typename AE::template handle<int>::type act = ae.fork(simple_thread);
    bith::join(act);
    std::cout << "<<do_test_join_m" << std::endl;
}

template <typename AE>
void do_test_join_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bith::join_all(tple);
    std::cout << "<<do_test_join_all" << std::endl;
}

template <typename AE>
void do_test_join_until(AE& ae) {
    BOOST_AUTO(act,bith::fork(ae, simple_thread));
    bool b = bith::join_until(act,boost::get_system_time()+boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::join_until(act,boost::get_system_time()+boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_join_until" << std::endl;
}

template <typename AE>
void do_test_join_until_m(AE& ae) {
    //BOOST_AUTO(act,bith::fork(ae, simple_thread));
    typename AE::template handle<int>::type act = ae.fork(simple_thread);
    bool b = bith::join_until(act,boost::get_system_time()+boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::join_until(act,boost::get_system_time()+boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_join_until_m" << std::endl;
}

template <typename AE>
void do_test_join_all_until(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bool b = bith::join_all_until(tple,boost::get_system_time()+boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::join_all_until(tple,boost::get_system_time()+boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_join_all_until" << std::endl;
}

template <typename AE>
void do_test_join_for(AE& ae) {
    BOOST_AUTO(act,bith::fork(ae, simple_thread));
    bool b = bith::join_for(act,boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::join_for(act,boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_join_for" << std::endl;
}

template <typename AE>
void do_test_join_for_m(AE& ae) {
    //BOOST_AUTO(act,boost::move(bith::fork(ae, simple_thread)));
    typename AE::template handle<int>::type act = ae.fork(simple_thread);
    bool b = bith::join_for(act,boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::join_for(act,boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_join_for_m" << std::endl;
}

template <typename AE>
void do_test_join_all_for(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bool b = bith::join_all_for(tple,boost::posix_time::seconds(1));
    BOOST_CHECK_EQUAL(b, false);
    b = bith::join_all_for(tple,boost::posix_time::seconds(3));
    BOOST_CHECK_EQUAL(b, true);
    std::cout << "<<do_test_join_all_for" << std::endl;
}


template <typename AE>
void do_test_thread_interrupts_at_interruption_point(AE& ae) {
    boost::mutex m;
    bool failed=false;
    boost::mutex::scoped_lock lk(m);
    BOOST_AUTO(act,bith::fork(ae, interruption_point_thread, &m,&failed));
    bith::interrupt(act);
    //act.interrupt();
    BOOST_CHECK_EQUAL(bith::interruption_requested(act), true);
    lk.unlock();
    bith::wait(act);
    //boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

    BOOST_CHECK(!failed);
    std::cout << "<<do_test_thread_interrupts_at_interruption_point" << std::endl;
}

template <typename AE>
void do_test_thread_interrupts_at_interruption_point_m(AE& ae) {
    boost::mutex m;
    bool failed=false;
    boost::mutex::scoped_lock lk(m);
    BOOST_AUTO(act,bith::fork(ae, interruption_point_thread, &m,&failed));
    act.interrupt();
    BOOST_CHECK_EQUAL(bith::interruption_requested(act), true);
    lk.unlock();
    bith::wait(act);
    BOOST_CHECK(!failed);
    std::cout << "<<do_test_thread_interrupts_at_interruption_point_m" << std::endl;
}

template <typename AE>
void do_test_fork_after_get(AE& ae) {
    test_value=0;
    test_value2=0;
    test_value3=0;
    BOOST_AUTO(actT, bith::fork_all(ae, my_simple_thread, my_simple_thread2));

    #ifndef ACT_WRAPPER
    typename AE:: template handle<int>::type res;
    bith::fork_after(ae, my_simple_thread3, actT, res);
    sleep(5);
    #else
    BOOST_AUTO(act,bith::fork_after(ae, my_simple_thread3, actT));
    #endif

    int res =bith::get(act);
    BOOST_CHECK_EQUAL(test_value3, 333);
    BOOST_CHECK_EQUAL(res, 333);
    std::cout << "<<do_test_fork_after_get" << std::endl;
}

template <typename AE>
void do_test_fork_after_wait(AE& ae) {
    test_value=0;
    test_value2=0;
    test_value3=0;
    BOOST_AUTO(actT, bith::fork_all(ae, my_simple_thread, my_simple_thread2));

    #ifndef ACT_WRAPPER
    typename AE:: template handle<int>::type res;
    bith::fork_after(ae, my_simple_thread3, actT, res);
    sleep(5);
    #else
    BOOST_AUTO(act,bith::fork_after(ae, my_simple_thread3, actT));
    #endif
    bith::wait(act);

    BOOST_CHECK_EQUAL(test_value3, 333);
    std::cout << "<<do_test_fork_after_wait" << std::endl;

}

template <typename AE>
void do_test_fork_after_join(AE& ae) {
    test_value=0;
    test_value2=0;
    test_value3=0;
    BOOST_AUTO(actT, bith::fork_all(ae, my_simple_thread, my_simple_thread2));

    #ifndef ACT_WRAPPER
    typename AE:: template handle<int>::type res;
    bith::fork_after(ae, my_simple_thread3, actT, res);
    sleep(5);
    #else
    BOOST_AUTO(act,bith::fork_after(ae, my_simple_thread3, actT));
    #endif
    bith::join(act);

    BOOST_CHECK_EQUAL(test_value3, 333);
    std::cout << "<<do_test_fork_after_join" << std::endl;

}
}
#endif
