/**
 * \file examples/factories/inserter.cpp
 *
 *  This example shows how to use the generic function callers.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <list>

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/adv_func_call.hpp>
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>

#include "./input_ui.hpp"
#include "./class_printer.hpp"
#include "./yes_no_dlg.hpp"
#include "./person.hpp"

template <typename String>
bool from_same_place(const String& a, const String& b, const String& c)
{
	return !(a.empty() || b.empty() || c.empty());
}

int main(void)
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	using namespace ::test;
	//
	// a list of persons
	::std::list< person > persons;
	bool insert_more = true;
	//
	cts::bstring city;
	cts::bstring postal_code;
	cts::bstring country;
	//
	if(yes_no_dlg(BOOST_CTS_LIT("Are all persons from the same city? (y/n)")))
	{
		cts::bcout() << BOOST_CTS_LIT("Enter city: ") << ::std::flush;
		getline(cts::bcin(), city);
		cts::bcout() << BOOST_CTS_LIT("Enter postal code: ") << ::std::flush;
		getline(cts::bcin(), postal_code);
		cts::bcout() << BOOST_CTS_LIT("Enter country: ") << ::std::flush;
		getline(cts::bcin(), country);
	}
	// 
	// keep inserting while insert_more == 'y'
	while(insert_more)
	{
		// if all the persons are from the same place
		// we can supply some of the params from the 
		// city/postal_code/country local variables
		if(from_same_place(city, postal_code, country))
		{
			// use the advanced constructor caller
			// to create a person
			advanced_constructor_caller<
				input_ui, // the user interface template
				person,   // the constructed class
				0,	// the index of the constructor
				mpl::vector_c<
					int,//the parameter mapping
					0, // enter name on console
					0, // enter surname on console
					0, // enter street on console
					0, // enter st. number on console
					1, // use the 1st param as city
					2, // use the 2nd param as postal code
					3  // use the 3rd param as country
				>
			> fact;
			// and push it back to the list
			persons.push_back(fact(city, postal_code, country));
		}
		else
		{
			// use the factory to get input data from the console
			// and to construct an instance of person
			factory<input_ui, person> fact;
			// and push it back to the list
			persons.push_back(fact());
		}
		//
		// ask whether the user wants to change the address
		// of the person which we just inserted.
		// its kinda strange to do it right after we entered 
		// the address, but we just want to show the functor_callers
		// anyway
		if(yes_no_dlg(BOOST_CTS_LIT("Change persons address ? (y/n) ")))
		{
			person& p(persons.back());
			//
			// if they're from the same place
			if(from_same_place(city, postal_code, country))
			{
				// use the advanced member function caller
				// so we can supply some of the pre-entered
				// values
				advanced_mem_func_caller<
					input_ui, // the user interface 
					person,  // the class
					0, // the index of the mem function
					mpl::vector_c<
						int, // the param mapping
						0, 
						0, 
						1, 
						2, 
						3
					>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
				// no variadic templates .. no defaults
				> func;
#else
				// if variadic templates are supported
				// we can supply default values for
				// the parameters before calling the
				// functor, so we use the current values
				> func(
					0,
					p.street,
					p.number,
					p.city,
					p.postal_code,
					p.country
				);
#endif
				// now call the functor on the instance
				// of person
				func(p, city, postal_code, country);
			}
			else 
			{
				// if we came here we want the user to 
				// enter all the values
#ifdef BOOST_NO_VARIADIC_TEMPLATES
				member_function_caller<input_ui, person, 0> 
					func;
#else
				member_function_caller<input_ui, person, 0> 
					func(
						0,
						p.street,
						p.number,
						p.city,
						p.postal_code,
						p.country
					);
#endif
				func(p);
			}
		}
		// check whether to insert more persons
		insert_more = yes_no_dlg(BOOST_CTS_LIT("Insert more ? (y/n) "));
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

