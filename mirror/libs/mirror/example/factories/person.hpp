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

struct person
{
        ::boost::cts::bstring first_name;
        ::boost::cts::bstring family_name;
        ::boost::cts::bstring street;
        ::boost::cts::bstring number;
        ::boost::cts::bstring city;
        ::boost::cts::bstring postal_code;

        person(
                const ::boost::cts::bstring& _first_name,
                const ::boost::cts::bstring& _family_name,
                const ::boost::cts::bstring& _street,
                const ::boost::cts::bstring& _number,
                const ::boost::cts::bstring& _city,
                const ::boost::cts::bstring& _postal_code
        ): first_name(_first_name)
         , family_name(_family_name)
         , street(_street)
         , number(_number)
         , city(_city)
         , postal_code(_postal_code)
        { }

        void change_address(
                const ::boost::cts::bstring& _street,
                const ::boost::cts::bstring& _number,
                const ::boost::cts::bstring& _city,
                const ::boost::cts::bstring& _postal_code
        )
        {
                street.assign(_street);
                number.assign(_number);
                city.assign(_city);
                postal_code.assign(_postal_code);
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
	(first_name)(family_name)(street)(number)(city)(postal_code)
)
BOOST_MIRROR_QREG_CONSTRUCTORS(
        ::test::person, (
		((::boost::cts::bstring)(first_name))
		((::boost::cts::bstring)(family_name))
		((::boost::cts::bstring)(street))
		((::boost::cts::bstring)(number))
		((::boost::cts::bstring)(city))
		((::boost::cts::bstring)(postal_code))
	)
)

BOOST_MIRROR_REG_MEM_FUNCTIONS_BEGIN(::test::person)
BOOST_MIRROR_REG_MEM_FUNCTION(
	0, void, change_address, 
	((::boost::cts::bstring)(street))
	((::boost::cts::bstring)(number))
	((::boost::cts::bstring)(city))
	((::boost::cts::bstring)(postal_code))
)
BOOST_MIRROR_REG_MEM_FUNCTIONS_END 

} // namespace mirror
} // namespace boost



#endif

