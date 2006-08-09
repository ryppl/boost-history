//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/mpl/assert.hpp>
#include <boost/coroutine/detail/is_callable.hpp>

namespace coroutines = boost::coroutines;

struct test_is_function_pointer {
  template<typename T>
  test_is_function_pointer(T) {
    BOOST_MPL_ASSERT((coroutines::detail::is_function_pointer<T>));
  }
};

struct test_is_functor {
  template<typename T>
  test_is_functor(T) {
    BOOST_MPL_ASSERT((coroutines::detail::is_functor<T>));
  }
};

struct test_is_callable {
  template<typename T>
  test_is_callable(T) {
    BOOST_MPL_ASSERT((coroutines::detail::is_callable<T>));
  }
};

void foo() {}

struct bar {
  typedef void result_type;
} a_bar;

int main() {
  test_is_function_pointer t1 (foo);
  test_is_functor t2 (a_bar);
  test_is_callable t3 (foo);
  test_is_callable t4 (a_bar);
}

