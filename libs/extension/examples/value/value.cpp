/*
 * Boost.Extension
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See             
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#include <iostream>

#include <boost/extension/factory.hpp>

#include "shared.hpp"

class tiger {
 public:
  tiger () : name_("A ferocious tiger") {}
  const std::string& get_name() const {
    return name_;
  }
 private:
  std::string name_;
};

int main() {
  using namespace boost::extensions;
  factory<generic_animal_impl> f;
  f.set<animal_impl<tiger> >();
  any_animal animal(f.create());
  std::cout << animal.get_name() << std::endl;
}
