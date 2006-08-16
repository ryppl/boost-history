//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>

// Producer driven producer/consumer pattern.
namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<void(const std::string&)> coroutine_type;

template<typename Consumer>
void producer(Consumer consumer, std::string base) {
  std::sort(base.begin(), base.end());
  do {
    consumer(base);
  } while (std::next_permutation(base.begin(), base.end()));
}

void consumer(coroutine_type::self& self, const std::string& value) {
  std::cout << value << "\n";
  while(true) {
    std::cout << self.yield()<< "\n";
  } 
}

int main() {
  producer(coroutine_type(consumer),
	   std::string("hello"));

  
}
