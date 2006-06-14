//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<int(int)> coroutine_type;

coroutine_type coro[2];

int coro_body(coroutine_type& self, int parm, int id) {
  while(parm) {
    int next = id==0?1:0;
    std::cout<<"in coroutine "<<id<<", parm: "<<parm <<", next: " <<next<<"\n";
    --parm;
    parm = self.yield_to(coro[next], parm); //_to(coro[id]) ; //(coro[id == 1? 0:1], parm);
  }
  return parm;
}

int main() {
  coro[0] = boost::bind(coro_body, _1, _2, 0);
  coro[1] = boost::bind(coro_body, _1, _2, 1);
  int  t = 10;
  while(coro[0]) {
    t = coro[0](t);
    std::cout << "in main, t: " << t << "\n";
  }
}
