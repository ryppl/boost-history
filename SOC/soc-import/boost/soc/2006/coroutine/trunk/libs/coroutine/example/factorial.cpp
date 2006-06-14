//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/coroutine/coroutine.hpp>
namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<double()> coroutine_type;
ssize_t factorial(coroutine_type& self) {
  double n = 0;
  double n_bang = 1;
  self.yield(n_bang);
  while(true) {
    n_bang = n_bang * ++n;  
    self.yield(n_bang);
  }
}

int main() {
  coroutine_type fact(factorial);
  for(int i = 0; i <100; i++)
    std::cout <<i<<"!= "<<fact()<<"\n";
}
