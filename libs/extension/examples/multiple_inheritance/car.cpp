/*
 * Boost.Extension / multiple inheritance example (car)
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See             
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


/* The following lines are only necessary because when
are linking to this dll at compile time with another
dll on Windows. As such, standard __declspec stuff
is required.

This example is something of a special case - normally
these types of macros are not necessary for classes 
- see the FAQ.
  */

#include <boost/extension/extension.hpp>
#define BOOST_EXTENSION_CAR_DECL BOOST_EXTENSION_EXPORT_DECL


#include "car.hpp"
#include <boost/extension/factory_map.hpp>
std::string car::list_capabilities()
{
  return "\nIt travels on roads.";
}

extern "C" void BOOST_EXTENSION_EXPORT_DECL 
extension_export(boost::extensions::factory_map & z)
{
  z.get<vehicle, std::string>()["A car exported as a vehicle"].set<car>();
  z.get<car, std::string>()["A car exported as a car"].set<car>();
}
