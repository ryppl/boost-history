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

#include <iostream>

#define BOOST_EXTENSION_USE_PP 1

#include "../car.hpp"
#include <boost/reflection/reflection.hpp>


int main(void)
{
  // Let's create the reflection and add the methods
  boost::extension::reflection<car, std::string> car_reflection("A Car!");
  car_reflection.add<int, bool>(&car::start, 3);
  car_reflection.add<std::string, bool, float, 
    std::string>(&car::turn, "turn", "turn_angle");

  // extension tests
  using namespace boost::extensions;
  factory_map fm;

  // load the car library
  load_single_library(fm, "libcar_lib.extension", 
		      "extension_export_car");
  std::list<factory<car, std::string, std::string> > & factory_list = 
	  fm.get<car, std::string, std::string>();  

  // check if the factories loaded fine
  if(factory_list.size() < 2) {
    std::cout << "Error - the classes were not found (" 
	      << factory_list.size() << ").\n";
    return -1;
  }

  // create some instances and call the method "3" (start)
  for (std::list<factory<car, std::string, std::string> >
	 ::iterator current_car = factory_list.begin(); 
       current_car != factory_list.end(); 
       ++current_car)
  {
    if(current_car->get_info() == "Compact") {
       car *beetle(current_car->create("VW New Beetle"));
       car_reflection.call<int, bool>(beetle, 3);
       delete beetle;
    }
  }

  return 0;
}
