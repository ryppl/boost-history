// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Bigint Library
//  Authors: Ronald Garcia
//           Andrew Lumsdaine

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
