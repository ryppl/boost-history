//  Copyright (c) 2006, Giovanni P. Deretta
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/coroutine/detail/context_linux.hpp>
#include <boost/coroutine/continuation.hpp>
#include <boost/test/unit_test.hpp>

namespace coro = boost::coroutines; 

typedef coro::continuation_tpl<coro::detail::oslinux::ia32_gcc_context_interface> continuation;

continuation f(continuation x) {
  std::cerr << "**** in f (1)"<<std::endl;
  x = x();
  std::cerr << "**** in f (2)"<<std::endl;
  x = x();
  std::cerr << "**** in f (3)"<<std::endl;
  return x;
}

struct g {
  g(int n_) : n(n_){}
  continuation operator()(continuation x) {
    for(int i = 0; i != n; ++i) {
      std::cerr << "**** in g ("<<i<<')'<<std::endl;
      x = x();
    }
    return f(x);
  }
  int n;
};


void test_continuation() {
  std::cerr << "**** in main (1)"<<std::endl;
  continuation c = coro::callcc1(f);
  std::cerr << "**** in main (2)"<<std::endl;
  c = c();
  std::cerr << "**** in main (3)"<<std::endl;
  c = c();
  std::cerr << "**** in main (4)"<<std::endl;
  continuation c2 = coro::callcc1(g(10));
  int j = 0;
  while(!c2.fired()) {
    std::cerr << "**** in main ("<<j<<')'<<std::endl;
    j++;
    c2 = c2();
  }
}



boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
  std::cout << "starting test" <<std::endl;
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("continuation test");

    test->add(BOOST_TEST_CASE(&test_continuation));

    return test;
}
