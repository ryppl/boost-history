// (C) Copyright Ronald Garcia 2002. Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#include "boost/bigint.hpp"
#include <cassert>
#include <iostream>

int main() {


  // 40 factorial = 40!

  boost::bigint value(1);
  for(int i = 1; i <= 40; ++i)
    value *= i;

  std::cout << "40! = " << value << "\n";


}
