/*
 * Boost.Reflection / extension integration example
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#include <string>

#include <boost/extension/factory_map.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/convenience.hpp>
#include <boost/reflection/reflection.hpp>
#include <iostream>

int main(void)
{
  
  std::map<std::string, boost::reflections::reflection> reflection_map;
  boost::extensions::shared_library lib("libCarLib.extension");
  lib.open();
  lib.get<void, std::map<std::string, 
    boost::reflections::reflection> &>
    ("extension_export_car")(reflection_map);
  // Let's create the reflection and add the methods
  boost::reflections::reflection & first_reflection =
    reflection_map["suv"];
  boost::reflections::reflection & second_reflection =
    reflection_map["compact"];
  boost::reflections::instance first_instance =
    first_reflection.get_constructor<const char *>()("First Instance");
  boost::reflections::instance second_instance =
    second_reflection.get_constructor<const char *>()("Second Instance");
  std::cout <<
    first_reflection.get_function<const char *>("get_type")(first_instance)
    << std::endl;
  std::cout <<
    second_reflection.get_function<const char *>("get_type")(second_instance)
    << std::endl;
}
