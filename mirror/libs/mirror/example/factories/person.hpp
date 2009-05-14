/**
 * \file examples/factories/person.hpp
 *
 *  Definition of classes used in some of the examples showing 
 *  the use of meta-constructors and factories
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_EXAMPLES_FACTORIES_PERSON_HPP
#define BOOST_MIRROR_EXAMPLES_FACTORIES_PERSON_HPP

#include <boost/mirror/meta_class.hpp>
	

namespace test {

typedef ::boost::cts::bstring string;

struct person
{
        string first_name;
        string family_name;
        string street;
        string number;
        string city;
        string postal_code;
	string country;

        person(
                const string& _first_name,
                const string& _family_name,
                const string& _street,
                const string& _number,
                const string& _city,
                const string& _postal_code,
		const string& _country
        ): first_name(_first_name)
         , family_name(_family_name)
         , street(_street)
         , number(_number)
         , city(_city)
         , postal_code(_postal_code)
	 , country(_country)
        { }

        void change_address(
                const string& _street,
                const string& _number,
                const string& _city,
                const string& _postal_code,
		const string& _country
        )
        {
                street.assign(_street);
                number.assign(_number);
                city.assign(_city);
                postal_code.assign(_postal_code);
                country.assign(_country);
        }

	inline string name_and_surname(void) const
	{
		string result(first_name);
		result.append(BOOST_CTS_LIT(" "));
		result.append(family_name);
		return result;
	}
};

} // namespace test

namespace boost {
namespace mirror {

// register the ::test namespace
BOOST_MIRROR_REG_NAMESPACE( (test) )

// tegister the class, it's attribs, constructors
BOOST_MIRROR_QREG_CLASS_NO_BASE(
	::test, person, 
	(first_name)(family_name)(street)(number)(city)(postal_code)(country)
)
BOOST_MIRROR_QREG_CONSTRUCTORS(
        ::test::person, (
		((const ::test::string&)(first_name))
		((const ::test::string&)(family_name))
		((const ::test::string&)(street))
		((const ::test::string&)(number))
		((const ::test::string&)(city))
		((const ::test::string&)(postal_code))
		((const ::test::string&)(country))
	)
)

BOOST_MIRROR_REG_MEM_FUNCTIONS_BEGIN(::test::person)
BOOST_MIRROR_REG_MEM_FUNCTION(
	0, void, change_address, 
	((const ::test::string&)(street))
	((const ::test::string&)(number))
	((const ::test::string&)(city))
	((const ::test::string&)(postal_code))
	((const ::test::string&)(country))
)
BOOST_MIRROR_REG_CONST_MEM_FUNCTION(
	1, ::test::string, name_and_surname, ((void))
)
BOOST_MIRROR_REG_MEM_FUNCTIONS_END 

} // namespace mirror
} // namespace boost



#endif

