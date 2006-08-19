//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <algorithm>
#include <boost/coroutine/shared_coroutine.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;
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

void fsm_structured(coroutine_type::self& self, char) {
  while(true) {
    if(self.result() != '0') {
      std::cout << '0';
      self.yield();
      if(self.result() != '0') {
	std::cout << '0';
	self.yield();
	if(self.result() == '0') {
	  std::cout << '1';
	  self.yield();
	} else {
	  std::cout << '0';
	  self.yield();
	}
      } else {
	std::cout << '0';
	self.yield();
      }
    } else { 
      std::cout << '0';
      self.yield();
    }
  }
}

typedef coroutine<char(char)> coroutine_type2;
typedef boost::function<void(coroutine_type2::self&)> action_type;

void terminator(coroutine_type2::self&) {}

void match(coroutine_type2::self& self, char match, char out1, char out2 , action_type act, action_type act2) {
  if(self.result() == match) {
    self.yield(out1);
    act(self);
  } else {
    self.yield(out2);
    act2(self);
  }
}

char fsm_match(coroutine_type2::self& self, char) {
  action_type s3 (boost::bind(match, _1, '0', '1', '0', terminator, terminator));
  action_type s2 (boost::bind(match, _1, '1', '0', '0', s3, terminator));
  action_type s1 (boost::bind(match, _1, '1', '0', '0', s2, terminator));
  while(true) {
    s1(self);
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
  coroutine_type2 coro (fsm_match);
  for(std::string::iterator i = input.begin();
      i != input.end();
      ++i)
    std::cout << coro(*i);
  std::cout <<"\n";
}
