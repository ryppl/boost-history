/**
 * \file examples/factories/input_ui.hpp
 *
 *  Simple console based user interface plug-in for mirror::factory
 *  used in the examples in this directory
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_EXAMPLES_FACTORIES_INPUT_UI_HPP
#define BOOST_MIRROR_EXAMPLES_FACTORIES_INPUT_UI_HPP

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/factory.hpp>
#include <boost/mirror/meta_type.hpp>

namespace test {
	
/** The general implementation of the input user interface template.
 *  Upon construction prints-out a banner and uses a factory
 *  configured to use the same input user interface to construct
 *  the Product.
 */
template <class Product>
struct input_ui
{
	struct banner
	{
		template <class Context, class ConstrIndex, class ParamIndex>
		banner(int tabs, Context* pc, ConstrIndex ci, ParamIndex pi)
		{
			::std::cout << 
				::boost::cts::bstring(tabs, BOOST_CTS_LIT('\t')) << 
				BOOST_CTS_LIT("Construct ") << 
				BOOST_MIRRORED_TYPE(Product)::full_name() << 
				BOOST_CTS_LIT(" ") << 
				boost::mirror::meta_constructors<
					Context
				>::base_param_name(ci, pi) <<
				::std::endl;
		}
	} b;

	typename ::boost::mirror::make_factory< input_ui, Product >::type f;

	template <class Context, class ConstrIndex, class ParamIndex>
	input_ui(int tabs, Context* pc, ConstrIndex ci, ParamIndex pi)
	 : b(tabs, pc, ci, pi)
	 , f(tabs)
	{ }

	inline Product operator()(void)
	{
		return f();
	}
};

/** Specialization of the input interface, used to produce
 *  doubles by prompting the user to enter a value on the 
 *  console.
 */
template <> 
struct input_ui<double>
{
	double x;

	template <class Context, class ConstrIndex, class ParamIndex>
	input_ui(int tabs, Context* pc, ConstrIndex ci, ParamIndex pi)
	{
                ::std::cout <<
			::boost::cts::bstring(tabs, BOOST_CTS_LIT('\t')) << 
                        BOOST_CTS_LIT("Enter ") << 
			BOOST_MIRRORED_TYPE(double)::full_name() << 
			BOOST_CTS_LIT(" ") << 
			boost::mirror::meta_constructors<
				Context
			>::base_param_name(ci, pi) <<
			BOOST_CTS_LIT(" = ") <<
                        ::std::flush;
		::std::cin >> x;
	}

	inline double operator()(void)
	{
		return x;
	}
};

/** A manager of this input user interface, which picks the 
 *  constructor that will be used by the means of the result
 *  of the index member function (i.e. the zero-th registered
 *  constructor will be always used with this manager).
 */
template <>
struct input_ui<void>
{
	int tabs;

	input_ui(int _tabs, int factory_index)
	 : tabs(_tabs)
	{ }

	input_ui(const ::boost::cts::bchar* names[], int factory_index)
	 : tabs(0)
	{
		::std::cout << 
			BOOST_CTS_LIT("Create ") << 
			names[factory_index]  << ::std::endl;
	}

	inline int param(void) const 
	{
		return tabs+1;
	}

	inline int index(void) 
	{
		return 0;
	}
};

} // namespace test 

#endif

