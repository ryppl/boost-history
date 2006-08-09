//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/coroutine/coroutine.hpp>
#include <boost/coroutine/move.hpp>
#include <boost/test/unit_test.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<void(void)> coroutine_type;

void coroutine_body(coroutine_type::self&) {}

void sink(coroutine_type) {}

coroutine_type source() {
  return coroutine_type(coroutine_body);
}

void sink_ref(boost::coroutines::move_from<coroutine_type>){}

void test_move() {
  std::cout << "test 1\n";
  coroutine_type coro (source());
  std::cout << "test 2\n";
  coroutine_type coro2 = coroutine_type(coroutine_body);
  std::cout << "test 3\n";
  coroutine_type coro3;
  std::cout << "test 4\n";
  coro3 = coroutine_type(coroutine_body);
  std::cout << "test 5\n";
  coroutine_type coro4 = source();
  std::cout << "test 6\n";
  coroutine_type coro5 (source());
  std::cout << "test 7\n";
  sink(coroutine_type(coroutine_body));
  std::cout << "test 8\n";
  sink(move(coro5));
  std::cout << "test 9\n";
  coro3 = move(coro4);
  std::cout << "test 10\n";
  sink(source());

}

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("move coroutine test");

    test->add(BOOST_TEST_CASE(&test_move));

    return test;
}
