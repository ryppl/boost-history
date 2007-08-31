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
BOOST_AUTO_TEST_CASE(argless)
{
  boost::reflections::reflection car_reflection;
  boost::reflections::reflector<car> car_reflector(&car_reflection);
  car_reflector.reflect_constructor();
  car_reflector.reflect<int>(&car::start, "start");
//  Check for argless constructor
  BOOST_CHECK(car_reflection.get_constructor().valid());
  boost::reflections::instance car_instance = 
    car_reflection.get_constructor().call();
  BOOST_CHECK(car_reflection.get_function<int>("start").valid());
  //  Make sure it doesn't have this undeclared method
  BOOST_CHECK(!car_reflection.get_function<int>("stop").valid());
  BOOST_CHECK_EQUAL
    (car_reflection.get_function<int>("start").call(car_instance), 3);
  boost::reflections::function<int> f =
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
  /*
  boost::reflections::reflector<porsche> * car_reflector =
    new boost::reflections::reflector<porsche>();
  boost::reflections::reflection car_reflection(car_reflector);
  car_reflector->reflect_constructor<int>();
  car_reflector->reflect(&car::start, "start");
  //  Check for argless constructor
  BOOST_CHECK(car_reflection.has_constructor<int>());
  BOOST_CHECK(!car_reflection.has_constructor());
  boost::reflections::instance car_instance = car_reflection.construct(1987);
  BOOST_CHECK(car_reflection.has_method<void, float>("start"));
  //  Make sure it doesn't have this undeclared method
  BOOST_CHECK(!car_reflection.has_method("stop"));
  car_reflection.call(car_instance, "start");
  BOOST_CHECK_EQUAL(car_reflection.call(car_instance, "get_year"), 1987);
  */
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

