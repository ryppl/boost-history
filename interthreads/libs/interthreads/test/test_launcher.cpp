//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/interthreads/launcher.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
namespace bith = boost::interthreads;

int test_value;
int simple_thread() {
    test_value=999;
    return test_value;
}

int simple_thread_1(unsigned i) {
    test_value=i;
    return test_value;
}

void test_dir_fork() {
    bith::launcher ae;
    test_value=0;
	boost::unique_future<int> fut  = ae.fork(simple_thread);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}    

void test_dir_fork_1() {
    bith::launcher ae;
    test_value=0;
	boost::unique_future<int> fut  = ae.fork(boost::bind(simple_thread_1, 2));
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}    

void test_indir_fork() {
    bith::launcher ae;
    test_value=0;
	boost::unique_future<int> fut = bith::fork(ae, simple_thread);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 999);
    BOOST_CHECK_EQUAL(res_value, 999);
}    

void test_indir_fork_1() {
    bith::launcher ae;
    test_value=0;
	boost::unique_future<int> fut = fork(ae, simple_thread_1, 2);
    int res_value = fut.get();
    BOOST_CHECK_EQUAL(test_value, 2);
    BOOST_CHECK_EQUAL(res_value, 2);
}    


test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("launcher");
  test->add(BOOST_TEST_CASE(&test_dir_fork));
  test->add(BOOST_TEST_CASE(&test_dir_fork_1));
  test->add(BOOST_TEST_CASE(&test_indir_fork));
  test->add(BOOST_TEST_CASE(&test_indir_fork_1));
  return test;
}

