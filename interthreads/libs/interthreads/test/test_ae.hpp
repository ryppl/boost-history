//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
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
    test_value=999;
    sleep(2);
    return test_value;
}
static std::string test_string_value;

std::string simple_string_thread() {
    test_string_value="999";
    sleep(5);
    return test_string_value;
}

int simple_thread2() {
    test_value2=111;
    sleep(5);
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

template <typename AE>
void do_test_member_fork(AE& ae) {
    test_value=0;
	BOOST_AUTO(act, ae.fork(simple_thread));
    BOOST_CHECK_EQUAL(act.is_ready(), false);
    BOOST_CHECK_EQUAL(act.has_value(), false);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    BOOST_CHECK_EQUAL(act.is_ready(), true);
    BOOST_CHECK_EQUAL(act.has_value(), true);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
}    
template <typename AE>
void do_test_member_fork_move(AE& ae) {
    test_value=0;
	typename AE::template handle<int>::type act = ae.fork(simple_thread);
    BOOST_CHECK_EQUAL(act.is_ready(), false);
    BOOST_CHECK_EQUAL(act.has_value(), false);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
    BOOST_CHECK_EQUAL(act.is_ready(), true);
    BOOST_CHECK_EQUAL(act.has_value(), true);
    BOOST_CHECK_EQUAL(act.has_exception(), false);
}    

template <typename AE>
void do_test_member_fork_bind(AE& ae) {
    test_value=0;
	BOOST_AUTO(act, ae.fork(boost::bind(simple_thread_1, 2)));
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}    

template <typename AE>
void do_test_fork(AE& ae) {
    test_value=0;
	BOOST_AUTO(act, bith::fork(ae, simple_thread));
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}    

template <typename AE>
void do_test_fork_1(AE& ae) {
    test_value=0;
	BOOST_AUTO(act, bith::fork(ae, simple_thread_1, 2));
    int res_value = act.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}    

template <typename AE>
void do_test_creation_through_functor(AE& ae)
{
    copyable_functor f;
    BOOST_AUTO(act,bith::fork(ae, f));
    int res = act.get();
    BOOST_CHECK_EQUAL(res, 999);
}

template <typename AE>
void do_test_creation_through_reference_wrapper(AE& ae)
{
    non_copyable_functor f;
	BOOST_AUTO(act,bith::fork(ae, boost::bind(boost::ref(f))));
    
    unsigned res = act.get();
    BOOST_CHECK_EQUAL(res, 999u);
    BOOST_CHECK_EQUAL(f.value, 999u);
}

template <typename AE>
void do_test_wait_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bith::wait_all(tple);
}

template <typename AE>
void do_test_wait_for_any(AE& ae) {
    BOOST_AUTO(res, bith::wait_for_any(ae, simple_thread, simple_thread2));
    BOOST_CHECK_EQUAL(res.first, 0u);
    BOOST_CHECK_EQUAL(res.second, 999);  
    res = bith::wait_for_any(ae, simple_thread2, simple_thread);
    BOOST_CHECK_EQUAL(res.first, 1u);
    BOOST_CHECK_EQUAL(res.second, 999);  
}

template <typename AE>
void do_test_set_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread2));
    bfus::tuple<int,int> res;
    bith::set_all(tple,res);
    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 999);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 111);
    
    bfus::for_each(res, print_xml());
    
}

template <typename AE>
void do_test_wait_for_all(AE& ae) {
    BOOST_AUTO(res, bith::wait_for_all(ae, simple_thread, simple_thread2));
    bfus::for_each(res, print_xml());
    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 999);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 111);
    res = bith::wait_for_all(ae, simple_thread2, simple_thread);
    bfus::for_each(res, print_xml());
    BOOST_CHECK_EQUAL(bfus::at_c<0>(res), 111);
    BOOST_CHECK_EQUAL(bfus::at_c<1>(res), 999);
}


///////////////

template <typename AE>
void do_test_member_fork_detach(AE& ae) {
    test_value=0;

    BOOST_AUTO(act,ae.fork(simple_thread));
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

template <typename AE>
void do_test_join_all(AE& ae) {
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bith::join_all(tple);    
}



template <typename AE>
void do_test_thread_interrupts_at_interruption_point(AE& ae) {
    boost::mutex m;
    bool failed=false;
    boost::mutex::scoped_lock lk(m);
    BOOST_AUTO(act,bith::fork(ae, interruption_point_thread, &m,&failed));
    act.interrupt();
    BOOST_CHECK_EQUAL(act.interruption_requested(), true);
    lk.unlock();
    act.wait();
    BOOST_CHECK(!failed);
}    


template <typename AE>
void do_test_fork_after_get(AE& ae) {  
    test_value=0;
    test_value2=0;
    test_value3=0;
	BOOST_AUTO(actT, fork_all(ae, my_simple_thread, my_simple_thread2));
    
    #ifndef ACT_WRAPPER
    typename AE:: template handle<int>::type res;
	bith::fork_after(ae, my_simple_thread3, actT, res);  
    sleep(5);
    #else
	BOOST_AUTO(act,bith::fork_after(ae, my_simple_thread3, actT));
    #endif
    
    int res =act.get();
    BOOST_CHECK_EQUAL(test_value3, 333);
    BOOST_CHECK_EQUAL(res, 333);
}    
template <typename AE>
void do_test_fork_after_wait(AE& ae) {  
    test_value=0;
    test_value2=0;
    test_value3=0;
    BOOST_AUTO(actT, fork_all(ae, my_simple_thread, my_simple_thread2));
    
    #ifndef ACT_WRAPPER
    typename AE:: template handle<int>::type res;
	bith::fork_after(ae, my_simple_thread3, actT, res);    
    sleep(5);
    #else
	BOOST_AUTO(act,bith::fork_after(ae, my_simple_thread3, actT));
    #endif
    act.wait();
    
    BOOST_CHECK_EQUAL(test_value3, 333);

}    
template <typename AE>
void do_test_fork_after_join(AE& ae) {  
    test_value=0;
    test_value2=0;
    test_value3=0;
    BOOST_AUTO(actT, fork_all(ae, my_simple_thread, my_simple_thread2));
    
    #ifndef ACT_WRAPPER
    typename AE:: template handle<int>::type res;
	bith::fork_after(ae, my_simple_thread3, actT, res);    
    sleep(5);
    #else
	BOOST_AUTO(act,bith::fork_after(ae, my_simple_thread3, actT));
    #endif
    act.join();
    
    BOOST_CHECK_EQUAL(test_value3, 333);

}    

}
#endif
