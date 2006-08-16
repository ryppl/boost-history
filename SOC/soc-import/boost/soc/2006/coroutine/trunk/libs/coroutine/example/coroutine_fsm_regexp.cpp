//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <algorithm>
#include <boost/coroutine/shared_coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::shared_coroutine;

typedef shared_coroutine<void(char)> coroutine_type;

void fsm_regexp(coroutine_type::self& self, char) {
  while(true) {
    if(self.yield() == '0') {
      std::cout << '0';
      if(self.yield() == '1') {
	std::cout << '0';
	while(self.yield() == '1') {
	  std::cout << '0';
	}
	std::cout <<'0';
	if(self.yield() == '1') {
	  std::cout << '0';
	  if(self.yield() == '0') {
	    std::cout << '1';
	  } else std::cout <<'0';
	} else std::cout <<'0';
      } else std::cout << '0';
    } else std::cout <<'0';
  } 
}

template<typename F>
F swallow(F f) {
  f();
  return move(f);
}

int main() {
  {
    std::string input ("0110100010010001101001000111110010011001");
    std::for_each(input.begin(), input.end(), swallow(coroutine_type(fsm_regexp)));
    std::cout <<"\n";
  }
  {
    std::string input ("011011010");
    std::for_each(input.begin(), input.end(), swallow(coroutine_type(fsm_regexp)));
    std::cout <<"\n";
  }

}
