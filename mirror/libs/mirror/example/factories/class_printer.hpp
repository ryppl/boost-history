/**
 * \file examples/factories/class_printer.cpp
 *
 *  This example shows how to use the generic function callers.
 *
 *  Copyright 2008-2009 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_EXAMPLES_FACTORIES_CLASS_PRINTER_HPP
#define BOOST_MIRROR_EXAMPLES_FACTORIES_CLASS_PRINTER_HPP

#include <boost/char_type_switch/iostream.hpp>
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>

/** Prints the types, names and values of
 *  class' attribute
 */
template <class Class>
struct attrib_printer
{
	const Class& inst;

	attrib_printer(const Class& _inst)
	 : inst(_inst)
	{ }
	
	template <class MetaAttribute>
	void operator()(MetaAttribute ma) const
	{
		::boost::cts::bcout() << 
			BOOST_CTS_LIT("\t") << 
			MetaAttribute::type::base_name() << 
			BOOST_CTS_LIT(" ") << 
			MetaAttribute::base_name() <<
			BOOST_CTS_LIT(": '") << 
			MetaAttribute::get(inst) << 
			BOOST_CTS_LIT("'") << 
			::std::endl;		
	}
};

/** Prints the type name of a class and 
 *  it's attributes
 */
struct class_printer
{
	template <class Class>
	void operator()(const Class& inst)
	{
		::boost::cts::bcout() << 
			BOOST_MIRRORED_CLASS(Class)::base_name() <<
			::std::endl;		
		
		::boost::mirror::for_each<
			typename BOOST_MIRRORED_CLASS(Class)::all_attributes
		>(attrib_printer<Class>(inst));
	}
};

#endif 

