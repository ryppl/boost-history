/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "salute.hpp"
#include <boost/extension/factory_map.hpp>

class hello : public salute
{
public:
	virtual const char *say(void) {return "hello";}
};

class bye : public salute
{
public:
	virtual const char *say(void) {return "bye!";}
};


extern "C" void BOOST_EXTENSION_EXPORT_DECL extension_export_salute(boost::extensions::factory_map & fm)
{
	fm.add<hello, salute, int>(1);
	fm.add<bye, salute, int>(2);
}
