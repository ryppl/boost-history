//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <string>
#include <algorithm>

// switch based fsm.
struct fsm {

  void operator() (char input_) {
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
    };
  }

  fsm() :
    m_state(A) {}

  enum state { A, B, C };

  state m_state;
};

int main() {
  std::string input ("0110100010010001101001000111110010011001");
  std::for_each(input.begin(), input.end(), fsm());
  std::cout <<"\n";
}
