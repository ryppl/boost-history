// (C) Copyright Ronald Garcia 2002. Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#include "boost/bigint.hpp"
#include "boost/lexical_cast.hpp"
#include <iostream>

int main() {
  
  int x = boost::lexical_cast<int>(boost::bigint(5));
  std::cout << "x = " << x << '\n';

  try {
    int y = boost::lexical_cast<int>(boost::bigint("1524155677490"));
    std::cout << "y = " << y << '\n';
  } catch (boost::bad_lexical_cast&) {
    std::cout << "Uh oh! y overflowed!\n";
  }

  return 0;
}
