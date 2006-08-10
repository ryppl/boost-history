//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/coroutine/coroutine.hpp>
#include <boost/test/unit_test.hpp>

//
// Test support for pass by non const reference.

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<int&(int&)> coroutine_type;

int& body(coroutine_type::self& self, int& x) {
  BOOST_CHECK(x == 0);
  x++;
  int a = 0;
  int &b = self.yield(a);
  (void)b; // quiet warning.
  BOOST_CHECK(a == 1);
  return x;
}

void test_reference() {
  coroutine_type coro(body);
  int a = 0;
  int &b = coro(a);
  BOOST_CHECK(a == 1);
  b++;
  coro(a);
}

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("reference coroutine test");

    test->add(BOOST_TEST_CASE(&test_reference));

    return test;
}
