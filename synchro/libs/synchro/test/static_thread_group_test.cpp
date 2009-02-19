//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/conc/static_thread_group.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::unit_test;
//using namespace boost::synchro;

void th1() {
    std::cout<<"th1_begin"<<std::endl;
    boost::this_thread::sleep(boost::posix_time::seconds(2));
    std::cout<<"th1_end"<<std::endl;
}
void th2() {
    std::cout<<"th2_begin"<<std::endl;
    boost::this_thread::sleep(boost::posix_time::seconds(3));
    std::cout<<"th2_end"<<std::endl;
}

void test_join() {
    boost::static_thread_group<2> stg(th1, th2);
    stg.join();
}
void test_join_f() {
    join_all(th1, th2);
}

void test_join_first() {
    boost::static_thread_group<2> stg(th1, th2);
    BOOST_CHECK(0==stg.join_first());
    boost::this_thread::sleep(boost::posix_time::seconds(2));
}
void test_join_first_then_interrupt() {
    boost::static_thread_group<2> stg(th1, th2);
    unsigned end_id =stg.join_first();
    stg.interrupt();
    BOOST_CHECK(0==end_id);
}
void test_join_first_then_interrupt_f() {
    BOOST_CHECK(0==join_first_then_interrupt(th1, th2));
}
test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("static_thread_group_tests");
  test->add(BOOST_TEST_CASE(&test_join));
  test->add(BOOST_TEST_CASE(&test_join_f));
  test->add(BOOST_TEST_CASE(&test_join_first));
  test->add(BOOST_TEST_CASE(&test_join_first_then_interrupt));
  test->add(BOOST_TEST_CASE(&test_join_first_then_interrupt_f));
  return test;
}

