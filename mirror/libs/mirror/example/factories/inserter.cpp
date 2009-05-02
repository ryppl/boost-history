/**
 * \file examples/factories/inserter.cpp
 *
 *  This example shows how to use the generic function caller.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <math.h>
#include <list>
#include <algorithm>

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/functor_call.hpp>
#include <boost/mirror/meta_mem_functions.hpp>
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>

#include "./input_ui.hpp"
#include "./person.hpp"

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


/** A functor with one parameter. This functor is called
 *  by the mirror::function_caller, which also constructs
 *  and supplies all the necessary arguments (the single 
 *  one in this case).
 */
template <class Class>
struct back_inserter
{
	::std::list< Class >& container;	

	back_inserter(::std::list< Class >& _container)
	 : container(_container)
	{ }

	void operator()(const Class& inst)
	{
		container.push_back(inst);
	}
};


int main(void)
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	using namespace ::test;
	//
	// a list of persons
	::std::list< person > persons;
	// 
	// keep inserting while insert_more == 'y'
	const cts::bchar yes = BOOST_CTS_LIT('y');
	const cts::bchar no = BOOST_CTS_LIT('n');
	cts::bchar insert_more = yes;
	while(insert_more == yes)
	{
		// the name of the function argument.
		// there is only one in this case
	        const cts::bchar* param_names[] = {BOOST_CTS_LIT("Person")};
		// create a functor caller, plug the input ui in
		// and supply the parameter types and parameter names
	        functor_caller<
	                input_ui,
	                mpl::vector1< person>
	        > caller(param_names);
		// call the back inserter by the means of the caller
		caller(back_inserter<person>(persons));
		// check whether to insert more persons
		do
		{
			cts::bcout() << 
				BOOST_CTS_LIT("Insert more ? (y/n) ") << 
				::std::flush;
			cts::bcin() >> insert_more;
		} while(insert_more != yes && insert_more != no);
	}
	//
	// TODO: remove this
	//
	typedef meta_member_functions<person> mem_fns;
	typedef mem_fns::function<mpl::int_<0> > fn_0;
	cts::bcout() <<
		fn_0::result_type::base_name() << " " <<
		fn_0::base_name() << "(" <<
		fn_0::params::param<mpl::int_<0> >::type::base_name() << ", " <<
		fn_0::params::param<mpl::int_<1> >::type::base_name() << ", " <<
		fn_0::params::param<mpl::int_<2> >::type::base_name() << ", " <<
		fn_0::params::param<mpl::int_<3> >::type::base_name() << ")"  <<
		::std::endl;
	//
	// TODO: 
	//
	//
	// print out all the persons in the list
	cts::bcout() << "Persons:" << ::std::endl;
	::std::for_each(
		persons.begin(),
		persons.end(),
		class_printer()
	);
	//
	return 0;
}

