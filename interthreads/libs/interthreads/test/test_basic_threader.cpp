//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/interthreads/typeof/basic_threader.hpp"
#include "boost/interthreads/algorithm.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>

#include <libs/interthreads/test/data_types.hpp>
#include <libs/interthreads/test/test_ae.hpp>

using namespace boost::unit_test;
namespace bith = boost::interthreads;
namespace bfus = boost::fusion;


void do_test_member_fork_move() {
    bith::basic_threader ae;
    #if 0
    aetst::do_test_member_fork_move(ae);
    return;
    #endif
    test_value=0;
	boost::thread act(ae.fork(simple_thread));
    act.join();
    BOOST_CHECK_EQUAL(test_value, 999);
}    

void do_test_member_fork_bind() {
    bith::basic_threader ae;
    #if 0
    aetst::do_test_member_fork_bind(ae);
    return;
    #endif
    test_value=0;
	boost::thread act(ae.fork(boost::bind(simple_thread_1, 2)));
    act.join();
    BOOST_CHECK_EQUAL(test_value, 2);
}    

void do_test_fork() {
    bith::basic_threader ae;
    #if 0
    aetst::do_test_fork(ae);
    return;
    #endif
    test_value=0;
	boost::thread act = bith::fork(ae, simple_thread);
    act.join();
    BOOST_CHECK_EQUAL(test_value, 999);
}    

void do_test_fork_1() {
    bith::basic_threader ae;
    #if 0
    aetst::do_test_fork_1(ae);
    return;
    #endif
    test_value=0;
	boost::thread act = bith::fork(ae, simple_thread_1, 2);
    act.join();
    BOOST_CHECK_EQUAL(test_value, 2);
}    

#if 0
// this do not works because boost::thread is not movable-only and boost::fusion::tuple works only with CopyContructible types
void do_test_join_all() {
    bith::basic_threader ae;
    typedef bith::result_of::fork_all<bith::basic_threader,bfus::tuple<void(*)(),void(*)()> >::type type;
    type handles = bith::fork_all(ae, simple_thread, simple_thread);
    //BOOST_AUTO(handles,bith::fork_all(ae, simple_thread, simple_thread));
    bith::join_all(tple);
    
}
#endif

test_suite* init_unit_test_suite(int, char*[])
{
    test_suite* test = BOOST_TEST_SUITE("basic_threader");
    test->add(BOOST_TEST_CASE(&do_test_member_fork_move));
    test->add(BOOST_TEST_CASE(&do_test_member_fork_bind));
    test->add(BOOST_TEST_CASE(&do_test_fork));
    test->add(BOOST_TEST_CASE(&do_test_fork_1));
    //test->add(BOOST_TEST_CASE(&do_test_join_all));
  return test;
}

