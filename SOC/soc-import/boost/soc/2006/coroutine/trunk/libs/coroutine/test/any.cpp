//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/assert.hpp>
#include <boost/any.hpp>
#include <boost/coroutine/coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<boost::any()> coroutine_type;

struct first_type{};
struct second_type{};
struct third_type{};
struct fourth_type{};
struct fifth_type{};

boost::any coro_body(coroutine_type self)  {
  self.yield(first_type());
  self.yield(second_type());
  self.yield(third_type());
  self.yield(fourth_type());
  return fifth_type();  
}

int main() {
  coroutine_type coro(coro_body);
  BOOST_ASSERT(coro().type() == typeid(first_type));
  BOOST_ASSERT(coro().type() == typeid(second_type));
  BOOST_ASSERT(coro().type() == typeid(third_type));
  BOOST_ASSERT(coro().type() == typeid(fourth_type));
  BOOST_ASSERT(coro().type() == typeid(fifth_type));
}
