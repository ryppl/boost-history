/*
 * Boost.Reflection / basic unit test
 *
 * (C) Copyright Mariano G. Consoni and Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#include <string>
#include <iostream>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>
#include <boost/reflection/static_reflection.hpp>


int func1(int j) {
  return j * 2; 
}
int func2(int j) {
  return j + 2; 
}
int func3(int j, float f) {
  return j - 2;
}
using namespace boost::reflections;
BOOST_AUTO_TEST_CASE(argless) {
  static_reflection sr;
  sr.set<std::string, int, int>("First Function", &func1);
  sr.set<std::string, int, int>("Second Function", &func2);
  sr.set<std::string, int, int, float>("Third Function", &func3);
  int result = sr.get<std::string, int, int, float>("Third Function")(3, 2.0f);
  BOOST_CHECK_EQUAL(result, 1);
}