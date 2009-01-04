//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/interthreads/basic_threader.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
namespace bith = boost::interthreads;

int test_value;
void simple_thread() {
    test_value=999;
}

void simple_thread_1(unsigned i) {
    test_value=i;
}

void test_dir_fork() {
    bith::basic_threader thr;
    test_value=0;
	boost::thread thrd = thr.fork(simple_thread);
    thrd.join();
    BOOST_CHECK_EQUAL(test_value, 999);
}    

void test_dir_fork_1() {
    bith::basic_threader thr;
    test_value=0;
	boost::thread thrd = thr.fork(boost::bind(simple_thread_1, 2));
    thrd.join();
    BOOST_CHECK_EQUAL(test_value, 2);
}    

void test_indir_fork() {
    bith::basic_threader thr;
    test_value=0;
	boost::thread thrd = bith::fork(thr, simple_thread);
    thrd.join();
    BOOST_CHECK_EQUAL(test_value, 999);
}    

void test_indir_fork_1() {
    bith::basic_threader thr;
    test_value=0;
	boost::thread thrd = fork(thr, simple_thread_1, 2);
    thrd.join();
    BOOST_CHECK_EQUAL(test_value, 2);
}    


test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("basic_threader");
  test->add(BOOST_TEST_CASE(&test_dir_fork));
  test->add(BOOST_TEST_CASE(&test_dir_fork_1));
  test->add(BOOST_TEST_CASE(&test_indir_fork));
  test->add(BOOST_TEST_CASE(&test_indir_fork_1));
  return test;
}

