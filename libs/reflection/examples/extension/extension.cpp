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

#if defined(MSC_VER) || defined(WIN32)
#define BOOST_EXTENSION_DIR_START "..\bin\"
#else
#define BOOST_EXTENSION_DIR_START "../bin/"
#endif

int main(void)
{
  
  std::map<std::string, boost::reflections::reflection> reflection_map;
  boost::extensions::shared_library lib
  ((std::string(BOOST_EXTENSION_DIR_START) + "libcar_lib.extension").c_str());
  lib.open();
  lib.get<void, std::map<std::string, 
    boost::reflections::reflection> &>
    ("extension_export_car")(reflection_map);
  if (reflection_map.size() != size_t(2)) {
    std::cout << "Could not load reflections!";
    return 1;
  }
  // Let's create the reflection and add the methods
  boost::reflections::reflection & first_reflection =
    reflection_map["suv"];
  boost::reflections::reflection & second_reflection =
    reflection_map["compact"];
  
  boost::reflections::constructor<const char *> first_constructor =
    first_reflection.get_constructor<const char *>();
  boost::reflections::instance first_instance = 
    first_constructor("First Instance");
  boost::reflections::function<const char *> first_function =
    first_reflection.get_function<const char *>("get_type");
  std::cout << "First reflection: " << first_function(first_instance)
            << std::endl;
  
  boost::reflections::constructor<const char *> second_constructor =
    second_reflection.get_constructor<const char *>();
  boost::reflections::instance second_instance = 
    second_constructor("Second Instance");
  boost::reflections::function<const char *> second_function =
    second_reflection.get_function<const char *>("get_type");
  std::cout << "Second reflection: " << second_function(second_instance)
            << std::endl;
}
