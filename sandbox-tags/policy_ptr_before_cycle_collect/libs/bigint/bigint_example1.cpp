// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Bigint Library
//  Authors: Ronald Garcia
//           Andrew Lumsdaine

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
