/*
 * Boost.Extension / multiple inheritance example (plane)
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
#define BOOST_EXTENSION_PLANE_DECL BOOST_EXTENSION_EXPORT_DECL


#include "plane.hpp"
#include <boost/extension/factory_map.hpp>

std::string plane::list_capabilities()
{
  return "\nIt flies in the air.";
}

extern "C" void BOOST_EXTENSION_EXPORT_DECL 
extension_export(boost::extensions::factory_map & z)
{
  z.get<vehicle, std::string>()["A plane exported as a vehicle"]
    .set<plane>();
  z.get<plane, std::string>()["A plane exported as a plane"]
    .set<plane>();
}
