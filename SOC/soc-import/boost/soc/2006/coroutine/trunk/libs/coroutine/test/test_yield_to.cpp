//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

struct my_result {};
struct my_parm {};

typedef coroutine<my_result(my_parm)> coroutine_type;

my_result coro(coroutine_type& other, 
	       int id,  
	       coroutine_type::self& self, 
	       my_parm parm) {
  int i = 10;
  my_result t;
  while(--i) {
    std::cout<<"in coroutine "<<id<<"\n";
    parm = self.yield_to(other, parm);
  }
  return t;
}

int main() {
  coroutine_type coro_a; 
  coroutine_type
    coro_b(boost::bind(coro, boost::ref(coro_a), 0, _1,  _2));
  coro_a = coroutine_type(boost::bind(coro, boost::ref(coro_b), 1, _1, _2)); 
					
  my_parm t;
  while(coro_a) {
    coro_a(t);
    std::cout<<"in main\n";
  }
}
