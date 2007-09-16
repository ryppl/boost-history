/*
 * Boost.Reflection / basic single parameter unit test
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#include <string>
#include <iostream>

#define BOOST_EXTENSION_USE_PP 1

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>
#include <boost/extension/factory_map.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/convenience.hpp>
#include <boost/reflection/reflection.hpp>
#include <iostream>
#include <boost/function.hpp>
#if defined(MSC_VER) || defined(WIN32)
#define BOOST_EXTENSION_DIR_START "..\bin\"
#else
#define BOOST_EXTENSION_DIR_START "../bin/"
#endif

BOOST_AUTO_TEST_CASE(shared_library_basic_test) {
  std::map<std::string, boost::reflections::reflection> reflection_map;
  boost::extensions::shared_library lib
    ((std::string(BOOST_EXTENSION_DIR_START) + "libcar_lib.extension").c_str());
  BOOST_CHECK(lib.open());
  lib.get<void, std::map<std::string, 
    boost::reflections::reflection> &>
    ("extension_export_car")(reflection_map);
  BOOST_CHECK_EQUAL(reflection_map.size(), size_t(2));
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
  BOOST_CHECK_EQUAL(first_function(first_instance), "It's an SUV.");
  
  boost::reflections::constructor<const char *> second_constructor =
    second_reflection.get_constructor<const char *>();
  boost::reflections::instance second_instance = 
    second_constructor("Second Instance");
  boost::reflections::function<const char *> second_function =
    second_reflection.get_function<const char *>("get_type");
  BOOST_CHECK_EQUAL(second_function(second_instance), "It's a compact.");
}
