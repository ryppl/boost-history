/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "word_description.hpp"
#include <boost/extension/factory_map.hpp>


class world : public word
{
public:
	virtual const char * get_val(){return "world!";}
};

class mundo : public word
{
public:
	virtual const char * get_val(){return "mundo!";}
};

class monde : public word
{
public:
	virtual const char * get_val(){return "monde!";}
};

class mondo : public word
{
public:
	virtual const char * get_val(){return "mondo!";}
};


class hello : public word
{
public:
	virtual const char * get_val(){return "hello";}
};

class hola : public word
{
public:
	virtual const char * get_val(){return "hola";}
};

class bonjour : public word
{
public:
	virtual const char * get_val(){return "bonjour";}
};

class buonasera : public word
{
public:
	virtual const char * get_val(){return "buonasera";}
};


extern "C" void BOOST_EXTENSION_EXPORT_DECL extension_export_multilanguage_word(boost::extensions::factory_map & fm)
{
	fm.add<hola, word, word_description>(word_description("spanish", "hello"));
	fm.add<mundo, word, word_description>(word_description("spanish", "world!"));

	fm.add<bonjour, word, word_description>(word_description("french", "hello"));
	fm.add<monde, word, word_description>(word_description("french", "world!"));

	fm.add<buonasera, word, word_description>(word_description("italian", "hello"));
	fm.add<mondo, word, word_description>(word_description("italian", "world!"));

	fm.add<hello, word, word_description>(word_description("english", "hello"));
	fm.add<world, word, word_description>(word_description("english", "world!"));
}
