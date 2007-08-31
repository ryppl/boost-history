/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_CONSTRUCTOR_HPP
#define BOOST_EXTENSION_CONSTRUCTOR_HPP
#include <boost/reflection/instance.hpp>
namespace boost {namespace reflections {
class constructor {
public:
  constructor(instance (*func)() = 0)
  : func_(func)
  {
  }
  instance call() {
    return (*func_)();
  }
  instance operator()() {
    return call(); 
  }
  bool valid() {return func_ != 0;}
private:
  instance (*func_)();
};
}}
#endif