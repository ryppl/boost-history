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
#include "boost/interthreads/typeof/future.hpp"
#include "boost/interthreads/algorithm.hpp"
#include <boost/typeof/typeof.hpp>
#include <libs/interthreads/test/data_types.hpp>
#include <libs/interthreads/test/test_ae.hpp>

#include <iostream>
#include <boost/test/unit_test.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

using namespace boost::unit_test;
namespace bith = boost::interthreads;
namespace bfus = boost::fusion;



void do_test_member_fork_detach() {
    bith::shared_threader ae;
    aetst::do_test_member_fork_detach(ae);
}

void do_test_member_fork() {  
    bith::shared_threader ae;
    aetst::do_test_member_fork(ae);
}    

void do_test_member_fork_move_unique() {
    bith::unique_threader ae;
    aetst::do_test_member_fork_move(ae);
}

void do_test_member_fork_bind() {
    bith::shared_threader ae;
    aetst::do_test_member_fork_bind(ae);
}    

void do_test_fork() {
    bith::shared_threader ae;
    aetst::do_test_fork(ae);
}    

void do_test_fork_1() {
    bith::shared_threader ae;
    aetst::do_test_fork_1(ae);
}    

void do_test_creation_through_reference_wrapper()
{
    bith::shared_threader ae;
    aetst::do_test_creation_through_reference_wrapper(ae);
}

void do_test_creation_through_functor()
{
    bith::shared_threader ae;
    aetst::do_test_creation_through_functor(ae);
}


void do_test_thread_interrupts_at_interruption_point() {
    bith::shared_threader ae;
    aetst::do_test_thread_interrupts_at_interruption_point(ae);
}    

void do_test_join_all() {
    bith::shared_threader ae;
    aetst::do_test_join_all(ae);    
}
void do_test_wait_all() {
    bith::shared_threader ae;
    aetst::do_test_wait_all(ae);
}

void do_test_wait_for_any() {
    bith::shared_threader ae;
    aetst::do_test_wait_for_any(ae);
}

void do_test_wait_for_any2() {
    boost::packaged_task<int> tsk1(simple_thread);
    boost::unique_future<int> res1 = tsk1.get_future();
    boost::thread th1(boost::move(tsk1));
    boost::packaged_task<int> tsk2(simple_thread2);
    boost::unique_future<int> res2 = tsk2.get_future();
    boost::thread th2(boost::move(tsk2));
    unsigned res = boost::wait_for_any(res1, res2);
    
    BOOST_CHECK_EQUAL(res, 0u);
}

void do_test_wait_for_any3() {
    bith::shared_threader ae;
    typedef bith::shared_threader  AE;
    typedef bith::result_of::fork_all<bith::shared_threader,bfus::tuple<int(*)(),int(*)()> >::type type;   
    type tple = bith::fork_all(ae, simple_thread, simple_thread);
    unsigned r = boost::wait_for_any(
        bith::get_future<AE>()(bfus::at_c<0>(tple)), 
        bith::get_future<AE>()(bfus::at_c<1>(tple)));
    int val;
    switch (r) {
        case 0:
            val = bfus::at_c<0>(tple).get();
            break;
        case 1:
            val = bfus::at_c<1>(tple).get();
            break;
        default:;
            //throw std::range_error("");
    }
    bith::interrupt_all(tple);
    BOOST_CHECK_EQUAL(r, 0u);
}

void do_test_other() {
    boost::packaged_task<int> tsk1(simple_thread);
    boost::packaged_task<int> tsk2(simple_thread2);
    boost::unique_future<int> res1 = tsk1.get_future();
    //BOOST_AUTO(res1,tsk1.get_future());
    boost::unique_future<int> res2 = tsk2.get_future();
    boost::thread th1(boost::move(tsk1));
    boost::thread th2(boost::move(tsk2));

}

void do_test_get_all() {
    bith::shared_threader ae;
    typedef bith::result_of::fork_all<bith::shared_threader,bfus::tuple<int(*)(),int(*)()> >::type auto_type;
    //auto_type tple = bith::fork_all(ae, simple_thread, simple_thread);
    BOOST_AUTO(tple,bith::fork_all(ae, simple_thread, simple_thread));
    bith::wait_all(tple);
    BOOST_AUTO(res,bith::get_all(tple));   
    //bfus::for_each(res, print_xml());
}

void do_test_set_all() {
    bith::shared_threader ae;
    aetst::do_test_set_all(ae);
}

void do_test_wait_for_all() {
    bith::shared_threader ae;
    aetst::do_test_wait_for_all(ae);
}

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("shared_threader");
    
    test->add(BOOST_TEST_CASE(&do_test_member_fork_detach));
    test->add(BOOST_TEST_CASE(&do_test_member_fork));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
    test->add(BOOST_TEST_CASE(&do_test_fork));
    test->add(BOOST_TEST_CASE(&do_test_fork_1));
    test->add(BOOST_TEST_CASE(&do_test_thread_interrupts_at_interruption_point));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_functor));
    test->add(BOOST_TEST_CASE(&do_test_creation_through_reference_wrapper));
    test->add(BOOST_TEST_CASE(&do_test_wait_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_any2));
    test->add(BOOST_TEST_CASE(&do_test_join_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_any));
    test->add(BOOST_TEST_CASE(&do_test_set_all));
    test->add(BOOST_TEST_CASE(&do_test_wait_for_all));
    #if 0
    //test->add(BOOST_TEST_CASE(&do_test_get_all));
    #endif
  return test;
}
