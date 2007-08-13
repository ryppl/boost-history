/*
 * Boost.Extension / hello world implementations
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#include "../car.hpp"
#include <boost/extension/factory_map.hpp>

class suv : public car
{
public:
	suv(const std::string name) : car(name) {}
	virtual std::string get_type(void) { return "It's a SUV."; }
	virtual ~suv(void) {}
};

class compact : public car
{
public:
	compact(const std::string name) : car(name) {}
	virtual std::string get_type(void) { return "It's a compact."; }
	virtual ~compact(void) {}
};


extern "C" 
void BOOST_EXTENSION_EXPORT_DECL 
extension_export_car(boost::extensions::factory_map & fm)
{
  fm.add<suv, car, std::string, std::string>("SUV");
  fm.add<compact, car, std::string, std::string>("Compact");
}
