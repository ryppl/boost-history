//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>
namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<int(int)> coroutine_type;



int coro_body(coroutine_type self, 
	      int parm) {
  while(parm) {
    std::cout<<"in coroutine, parm: "<<parm<<"\n";
    --parm;
    parm = self.yield(parm); //_to(other, parm);
  }
  return parm;
}

int main() {
  coroutine_type coro(coro_body);
					
  int  t = 10;
  while(coro) {
    t = coro(t);
    std::cout<<"in main, t: "<<t<<"\n";
  }
}


