/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../word.hpp"
#include <boost/extension/factory_map.hpp>

class world : public word
{
public:
  virtual const char * get_val(){return "world! v2";}
};
class hello : public word
{
public:
  virtual const char * get_val(){return "| v2 hello";}
};
extern "C" void BOOST_EXTENSION_EXPORT_DECL extension_export_word(boost::extensions::factory_map & fm)
{
	fm.add<hello, word, int>(21);	// int could be used as version (v2 word 1)
	fm.add<world, word, int>(22);	// int could be used as version (v2 word 2)
}
