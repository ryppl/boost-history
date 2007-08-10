/*
 * Boost.Reflection / basic example
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

#include <iostream>

#define BOOST_EXTENSION_USE_PP 1

#include "car.hpp"
#include <boost/reflection/reflection.hpp>


int main(void)
{

  /* Here we declare that we are reflecting out a Car. The second
     template parameter declares that we will describe this reflection
     using a string. Any arbitrary type could be used here. */
  boost::extension::reflection<car, std::string> car_reflection("A Car!");

  /* Here we add two methods to the reflection. The library must
     correctly parse and remember the parameters and return type of these
     methods. Here, we elect to describe the first method using a number
     and the second using a string. For the second method, the library
     knows now that it is a function of Car called turn that takes a float
     named "turn_angle". */ 
  car_reflection.add<int, bool>(&car::start, 3);
  car_reflection.add<std::string, bool, float, 
    std::string>(&car::turn, "turn", "turn_angle");

  // create some instances to use our reflection
  car porsche_911("Porsche 911");
  car ferrari_f40("Ferrari F40");

  // call methods
  car_reflection.call<int, bool>(&porsche_911, 3);
  car_reflection.call<std::string, bool, 
    float, std::string>(&porsche_911, "turn", .5f);

  car_reflection.call<std::string, bool, 
    float, std::string>(&ferrari_f40, "turn", .10f);

  // prepare the parameter map for a call
  boost::extension::parameter_map pm;
  pm.add<float, std::string>(.15f, "turn_angle");
  pm.add<int, std::string>(50, "aceleration");

  return 0;
}
