/*
 * Boost.Extension / car library implementations
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#include "../car.hpp"
#include <boost/extension/impl/decl.hpp>
#include <boost/reflection/reflection.hpp>
#include <boost/reflection/reflector.hpp>
using namespace boost::reflections;

// Although both of these classes are derived from a common
// base, this is certainly not necessary. If we were using
// Boost.Extension factories, this would be required.
class suv : public car
{
public:
        suv(const char * name) : car(name) {}
        virtual const char * get_type(void) { return "It's an SUV."; }
        virtual ~suv(void) {}
};

class compact : public car
{
public:
        compact(const char * name) : car(name) {}
        virtual const char * get_type(void) { return "It's a compact."; }
        virtual ~compact(void) {}
};


extern "C" 
void BOOST_EXTENSION_EXPORT_DECL
extension_export_car(std::map<std::string, reflection> reflection_map) {
  reflection & first = reflection_map["suv"];
  reflection & second = reflection_map["compact"];

  // Create a reflector for each type that is being reflected.
  reflector<suv> suv_reflector(&first);
  reflector<compact> compact_reflector(&second);

  // Reflect the constructor with a `const char*` arg
  suv_reflector.reflect_constructor<const char *>();
  compact_reflector.reflect_constructor<const char *>();

  // Reflect a function for each
  suv_reflector.reflect<const char *>(&suv::get_type, "get_type");
  compact_reflector.reflect<const char *>(&compact::get_type, "get_type");
}
