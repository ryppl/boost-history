//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/optional.hpp>
#include <boost/coroutine/coroutine.hpp>
namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<boost::optional<int>(int)> coroutine_type;

boost::optional<int> coro_body(coroutine_type::self& self, int parm)  {
  self.yield();
  self.yield(0);
  self.yield();
  self.yield(0);
  self.yield();
  self.yield(1);
  return boost::optional<int>();
}

int main() {
  coroutine_type coro(coro_body);
  while(coro)
    if(coro())
      std::cout<<"on.\n";
    else
      std::cout<<"off.\n";
}
