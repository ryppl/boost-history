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

#include <boost/mirror/adv_func_call.hpp>
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


int main(void)
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	using namespace ::test;
	//
	// a list of persons
	::std::list< person > persons;
	// 
	const cts::bchar yes = BOOST_CTS_LIT('y');
	const cts::bchar no = BOOST_CTS_LIT('n');
	cts::bchar insert_more = yes;
	cts::bchar change_address = no;
	// keep inserting while insert_more == 'y'
	while(insert_more == yes)
	{
		factory<input_ui, person> fact;
		persons.push_back(fact());
		//
		// check whether to insert more persons
		do
		{
			cts::bcout() << 
				BOOST_CTS_LIT("Change persons address ? (y/n) ") << 
				::std::flush;
			cts::bcin() >> change_address;
			cts::bcin().ignore();
		} while(change_address != yes && change_address != no);
		if(change_address == yes)
		{
			person& p(persons.back());
#ifdef BOOST_NO_VARIADIC_TEMPLATES
			member_function_caller<input_ui, person, 0> func;
#else
			member_function_caller<input_ui, person, 0> func(
				0,
				p.street,
				p.number,
				p.city,
				p.postal_code
			);
#endif
			func(p);
			//
			// TODO: test
			//
			typedef BOOST_MIRRORED_CLASS(person)::member_functions meta_fns;
			advanced_functor_caller<
				input_ui, 
				meta_fns, 
				mpl::int_<0>, 
				mpl::vector4<mpl::int_<0>, mpl::int_<0>, mpl::int_<1>, mpl::int_<2> >
			> adv_func;
			//
			adv_func(p, BOOST_CTS_LIT("Zilina"), BOOST_CTS_LIT("010 07"));
			//
			// TODO:
			//
		}
		// check whether to insert more persons
		do
		{
			cts::bcout() << 
				BOOST_CTS_LIT("Insert more ? (y/n) ") << 
				::std::flush;
			cts::bcin() >> insert_more;
			cts::bcin().ignore();
		} while(insert_more != yes && insert_more != no);
	}
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

