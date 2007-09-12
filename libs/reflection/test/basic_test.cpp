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

#define BOOST_EXTENSION_USE_PP 1

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>
#include <boost/reflection/reflector.hpp>

class car {
public:
  int start() {
    return 3;
  }
};
using namespace boost::reflections;
BOOST_AUTO_TEST_CASE(argless)
{
  reflection car_reflection;
  reflector<car> car_reflector(&car_reflection);
  car_reflector.reflect_constructor();
  car_reflector.reflect<int>(&car::start, "start");
//  Check for argless constructor
  BOOST_CHECK(car_reflection.get_constructor().valid());
  instance car_instance = 
    car_reflection.get_constructor().call();
  BOOST_CHECK(car_reflection.get_function<int>("start").valid());
  //  Make sure it doesn't have this undeclared method
  BOOST_CHECK(!car_reflection.get_function<int>("stop").valid());
  BOOST_CHECK_EQUAL
    (car_reflection.get_function<int>("start").call(car_instance), 3);
  function<int> f =
    car_reflection.get_function<int>("start");
  BOOST_CHECK_EQUAL(f(car_instance), 3);
}
class porsche : protected car {
public:
  porsche(int year) : year_(year) {
  }
  int get_year() {
    return year_; 
  }
  void start(float speed) {
  }
private:
  int year_;
};
BOOST_AUTO_TEST_CASE(single_arg)
{
  reflection car_reflection;
  reflector<porsche> car_reflector(&car_reflection);
  car_reflector.reflect_constructor<int>();
  car_reflector.reflect(&porsche::start, "start");
  car_reflector.reflect(&porsche::get_year, "get_year");
  //  Check for argless constructor
  BOOST_CHECK(car_reflection.get_constructor<int>().valid());
  BOOST_CHECK(!car_reflection.get_constructor().valid());
  boost::reflections::instance car_instance =
    car_reflection.get_constructor<int>()(1987);
  function<void, float> f1(car_reflection.get_function<void, float>("start"));
  BOOST_CHECK(f1.valid());
  //  Make sure it doesn't have this undeclared method
  BOOST_CHECK(!car_reflection.get_function<void>("stop").valid());
  f1(car_instance, 21.0f);
  function<int> f2 = car_reflection.get_function<int>("get_year");
  BOOST_CHECK(f2.valid());
  int year = f2(car_instance);
  BOOST_CHECK_EQUAL(year, 1987);
}
porsche * get_porsche(float year) {
  return new porsche(static_cast<int>(year)); 
}
BOOST_AUTO_TEST_CASE(single_arg_factory)
{
  /*
  boost::reflections::reflector<porsche> * car_reflector =
  new boost::reflections::reflector<porsche>();
  boost::reflections::reflection car_reflection(car_reflector);
  car_reflector->reflect_constructor<int>();
  car_reflector->reflect_factory<float>(&get_porsche, "get_porsche");
  car_reflector->reflect(&car::start, "start");
  //  Check for argless constructor
  BOOST_CHECK(car_reflection.has_constructor());
  boost::reflections::instance car_instance = car_reflection.construct();
  BOOST_CHECK(car_reflection.has_method("start"));
  //  Make sure it doesn't have this undeclared method
  BOOST_CHECK(!car_reflection.has_method("stop"));
  car_reflector.call(car_reflection, "start");*/
}

