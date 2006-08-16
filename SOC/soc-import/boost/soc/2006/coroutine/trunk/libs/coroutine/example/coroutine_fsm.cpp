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

enum state { A, B, C};

void fsm(coroutine_type::self& self, char input_) {
  state m_state = A;
  while(true) {
    bool input = input_ != '0';
    switch(m_state) {
    case A:
      std::cout << (input? '0' : '0');
      m_state = input? B : A;
      break;
    case B:
      std::cout << (input? '0' : '0');
      m_state = input? C : A;
      break;
    case C:
      std::cout << (input? '0' : '1');
      m_state = input? C : A;
      break;
    }
    input_ = self.yield();
  }
}

void fsm_goto(coroutine_type::self& self, char input) {
  while(true) {
  A:
    if(input != '0') {
      std::cout << '0';
      input = self.yield();
      goto B;
    } else {
      std::cout << '0';
      input = self.yield();
      goto A;
    }
  B:
    if(input != '0') {
      std::cout << '0';
      input = self.yield();
      goto C;
    } else {
      std::cout << '0';
      input = self.yield();
      goto A;
    }
  C:
    if(input != '0') {
      std::cout << '0';
      input = self.yield();
      goto C;
    } else {
      std::cout << '1';
      input = self.yield();
      goto A;
    }
  }
}

void fsm_structured(coroutine_type::self& self, char input) {
  std::cout << '0';
  while(true) {
    if(self.yield() != '0') {
      std::cout << '0';
      if(self.yield() != '0') {
	std::cout << '0';
	if(self.yield() == '0') 
	  std::cout << '1';
	else std::cout <<'0';
      } else std::cout <<'0';
    } else std::cout <<'0';
  }
}

int main() {
  std::string input ("0110100010010001101001000111110010011001");
  std::for_each(input.begin(), input.end(), coroutine_type(fsm));
  std::cout <<"\n";
  std::for_each(input.begin(), input.end(), coroutine_type(fsm_goto));
  std::cout <<"\n";
  std::for_each(input.begin(), input.end(), coroutine_type(fsm_structured));
  std::cout <<"\n";

}
