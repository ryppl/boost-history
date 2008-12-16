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
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>

#include "./input_ui.hpp"

namespace test {
	
struct person 
{
	::std::string first_name;
	::std::string family_name;
	::std::string street;
	::std::string number;
	::std::string city;
	::std::string postal_code;

	person(
		const ::std::string& _first_name,
		const ::std::string& _family_name,
		const ::std::string& _street,
		const ::std::string& _number,
		const ::std::string& _city,
		const ::std::string& _postal_code
	): first_name(_first_name)
	 , family_name(_family_name)
	 , street(_street)
	 , number(_number)
	 , city(_city)
	 , postal_code(_postal_code)
	{ }
};

} // namespace test 


namespace boost {
namespace mirror {

// register the ::test namespace
BOOST_MIRROR_REG_NAMESPACE( (test) )

// register the person type
BOOST_MIRROR_REG_TYPE( ::test, person)

// register the constructors of ::test::person
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( ::test::person )
	BOOST_MIRROR_REG_CONSTRUCTOR(0, 
		((::std::string)(first_name))
		((::std::string)(family_name))
		((::std::string)(street))
		((::std::string)(number))
		((::std::string)(city))
		((::std::string)(postal_code))
	)
BOOST_MIRROR_REG_CONSTRUCTORS_END

// register the attributes of person
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::person)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, first_name)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, family_name)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, street)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, number)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, city)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, postal_code)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END


} // namespace mirror
} // namespace boost

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
	::std::list< ::test::person > persons;
	// 
	// keep inserting while insert_more == 'y'
	char insert_more = 'y';
	while(insert_more == 'y')
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
			cts::bcout() << "Insert more ? (y/n) " << ::std::flush;
			cts::bcin() >> insert_more;
		} while(insert_more != 'y' && insert_more != 'n');
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

