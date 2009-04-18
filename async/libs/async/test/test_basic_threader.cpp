//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/async/typeof/basic_threader.hpp"
#include "boost/async/algorithm.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>

#include <libs/async/test/data_types.hpp>
#include <libs/async/test/test_ae.hpp>

using namespace boost::unit_test;
namespace basync = boost::async;
namespace bfus = boost::fusion;


#if 0
void do_test_member_fork_detach() {
    basync::basic_threader ae;
    aetst::do_test_member_fork_detach(ae);
}
void do_test_member_fork_thr() {
    basync::basic_threader ae;
    aetst::do_test_member_fork_thr(ae);
}

void do_test_member_fork_m() {
    basync::basic_threader ae;
    #if 1
    aetst::do_test_member_fork_m_thr(ae);
    #endif
    test_value=0;
    boost::thread act(ae.fork(simple_thread));
    act.join();
    BOOST_CHECK_EQUAL(test_value, 999);
}


void do_test_member_fork_bind() {
    basync::basic_threader ae;
    aetst::do_test_member_fork_bind(ae);
}

void do_test_member_fork_bind2() {
    basync::basic_threader ae;
    #if 0
    aetst::do_test_member_fork_bind(ae);
    return;
    #endif
    test_value=0;
    boost::thread act(ae.fork(boost::bind(simple_thread_1, 2)));
    act.join();
    BOOST_CHECK_EQUAL(test_value, 2);
}

void do_test_fork_thr() {
    basync::basic_threader ae;
    aetst::do_test_fork_thr(ae);
    //test_value=0;
    //boost::thread act = basync::fork(ae, simple_thread);
    //act.join();
    //BOOST_CHECK_EQUAL(test_value, 999);
}

void do_test_fork_1_thr() {
    basync::basic_threader ae;
    aetst::do_test_fork_1_thr(ae);
    //test_value=0;
    //boost::thread act = basync::fork(ae, simple_thread_1, 2);
    //act.join();
    //BOOST_CHECK_EQUAL(test_value, 2);
}

void do_test_creation_through_reference_wrapper_thr()
{
    basync::basic_threader ae;
    aetst::do_test_creation_through_reference_wrapper_thr(ae);
}

void do_test_creation_through_functor_thr()
{
    basync::basic_threader ae;
    aetst::do_test_creation_through_functor_thr(ae);
}

void do_test_thread_interrupts_at_interruption_point() {
    basync::basic_threader ae;
    aetst::do_test_thread_interrupts_at_interruption_point(ae);
}


#if 0
// this do not works because boost::thread is movable-only and boost::fusion::tuple works only with CopyContructible types
void do_test_join_all() {
    basync::basic_threader ae;
    typedef basync::result_of::fork_all<basync::basic_threader,bfus::tuple<void(*)(),void(*)()> >::type type;
    type handles = basync::fork_all(ae, simple_thread, simple_thread);
    //BOOST_AUTO(handles,basync::fork_all(ae, simple_thread, simple_thread));
    basync::join_all(tple);

}
#endif


void do_test_wait() {
    basync::basic_threader ae;
    aetst::do_test_wait(ae);
}

void do_test_wait_until() {
    basync::basic_threader ae;
    aetst::do_test_wait_until(ae);
}

void do_test_wait_for() {
    basync::basic_threader ae;
    aetst::do_test_wait_for(ae);
}

void do_test_join() {
    basync::basic_threader ae;
    aetst::do_test_join_m(ae);
}

void do_test_join_until() {
    basync::basic_threader ae;
    aetst::do_test_join_until_m(ae);
}
#endif
void do_test_join_for_m() {
    basync::basic_threader ae;
    aetst::do_test_join_for_m(ae);
}

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("basic_threader");
    test->add(BOOST_TEST_CASE(&do_test_join_for_m));

#if 0
    //test->add(BOOST_TEST_CASE(&do_test_member_fork));
    //test->add(BOOST_TEST_CASE(&do_test_member_fork_m));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
    test->add(BOOST_TEST_CASE(&do_test_fork_thr));
    test->add(BOOST_TEST_CASE(&do_test_fork_1_thr));

    test->add(BOOST_TEST_CASE(&do_test_wait));
    test->add(BOOST_TEST_CASE(&do_test_wait_until));
    test->add(BOOST_TEST_CASE(&do_test_wait_for));

    //test->add(BOOST_TEST_CASE(&do_test_member_fork_detach));
    test->add(BOOST_TEST_CASE(&do_test_join));
    test->add(BOOST_TEST_CASE(&do_test_join_until));

    test->add(BOOST_TEST_CASE(&do_test_join_all));
#endif
  return test;
}

