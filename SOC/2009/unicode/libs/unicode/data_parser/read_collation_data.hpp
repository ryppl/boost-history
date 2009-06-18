// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*
This file is required input:

UCA/allkeys.txt
*/

#error this file is deleted

#ifndef BOOST_UNICODE_READ_COLLATION_DATA_HPP_INCLUDED
#define BOOST_UNICODE_READ_COLLATION_DATA_HPP_INCLUDED

#include <string>
#include <vector>
#include <map>

#include <boost/unicode/ucd/detail/unichar_data.hpp>
#include "read_character_properties.hpp"

namespace boost
{

	namespace unicode
	{

		namespace ucd
		{
			struct collation_element
			{
				uint16_t weights [3];
				bool variable;
			};

			void read_collation_data (
				std::map <char32, character_properties> & props, 
                                const char* filename, bool required);

		}	// namespace ucd

	}	// namespace unicode

}	// namespace boost

#endif	// BOOST_UNICODE_READ_COLLATION_DATA_HPP_INCLUDED
