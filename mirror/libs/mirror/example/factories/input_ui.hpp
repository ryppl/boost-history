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
		template <class MetaFunctions, class FuncIndex, class ParamIndex>
		banner(int tabs, MetaFunctions mf, FuncIndex fi, ParamIndex pi)
		{
			::boost::cts::bcout() << 
				::boost::cts::bstring(tabs, BOOST_CTS_LIT('\t')) << 
				BOOST_CTS_LIT("Get ") << 
				BOOST_MIRRORED_TYPE(Product)::full_name() << 
				BOOST_CTS_LIT(" ") << 
				MetaFunctions::
				template function<FuncIndex>::params::
				template param<ParamIndex>::base_name() <<
				::std::endl;
		}
	} b;

	typename ::boost::mirror::make_factory< input_ui, Product >::type f;

	template <class MetaFunctions, class FuncIndex, class ParamIndex>
	input_ui(int tabs, MetaFunctions mf, FuncIndex fi, ParamIndex pi)
	 : b(tabs, mf, fi, pi)
	 , f(tabs)
	{ }

	inline Product operator()(void)
	{
		return f();
	}
};

/** Implementation of the input interface, used to produce
 *  values by prompting the user to enter a value on the 
 *  console.
 */
template <class Product> 
struct console_input_ui
{
	Product x;

	struct constr_param_name_printer
	{
		template <class MetaParams, class ParamIndex>
		inline void operator()(MetaParams* pmp, ParamIndex pi) const
		{
			if(ParamIndex::value > 0) 
				boost::cts::bcout() << BOOST_CTS_LIT(", ");
			::boost::cts::bcout() << 
				MetaParams::
				template param<ParamIndex>::base_name();
		}
	};
	
	template <class MetaFunctions, class FuncIndex, class ParamIndex>
	console_input_ui(
		int tabs, 
		MetaFunctions mf, 
		FuncIndex fi, 
		ParamIndex pi
	)
	{
			typedef typename MetaFunctions::
                        template function<FuncIndex> meta_function;
			//
			::boost::cts::bcout() <<
			::boost::cts::bstring(tabs, BOOST_CTS_LIT('\t')) << 
			BOOST_CTS_LIT("Enter ") << 
			BOOST_MIRRORED_TYPE(Product)::full_name() << 
			BOOST_CTS_LIT(" ") << 
			meta_function::params::
			template param<ParamIndex>::base_name() <<
			BOOST_CTS_LIT(" for ") <<
			meta_function::result_type::full_name() << (
				meta_function::is_constructor::value ?
				::boost::cts::bstring() :
				::boost::cts::bstring(BOOST_CTS_LIT(" "))
			) << (
				meta_function::is_constructor::value ?
				::boost::cts::bstring() :
				meta_function::base_name()
			) << BOOST_CTS_LIT("(");
			//
			meta_function::params::
			for_each(constr_param_name_printer());
			//
			::boost::cts::bcout() <<
			BOOST_CTS_LIT(") = ") <<
                        ::std::flush;
		::boost::cts::bcin() >> x;
	}

	inline Product operator()(void)
	{
		return x;
	}
};

#define BOOST_MIRROR_EXAMPLES_SPECIALIZE_CONSOLE_INPUT_UI(TYPE) \
template <>  \
struct input_ui< TYPE > : console_input_ui< TYPE > \
{ \
	template <class MetaFunctions, class FuncIndex, class ParamIndex> \
	input_ui(int tabs, MetaFunctions mf, FuncIndex fi, ParamIndex pi) \
	 : console_input_ui< TYPE >(tabs, mf, fi, pi) \
	{ } \
}; 

BOOST_MIRROR_EXAMPLES_SPECIALIZE_CONSOLE_INPUT_UI(double)
BOOST_MIRROR_EXAMPLES_SPECIALIZE_CONSOLE_INPUT_UI(::boost::cts::bstring)

/** A manager of this input user interface, which picks the 
 *  constructor that will be used by the means of the result
 *  of the index member function (i.e. the zero-th registered
 *  constructor will be always used with this manager).
 */
template <>
struct input_ui<void>
{
	int tabs;

	input_ui(int _tabs)
	 : tabs(_tabs)
	{ }

	template <class Product, class FunctionIndex>
	inline int param(Product* pp, FunctionIndex fi) const 
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

