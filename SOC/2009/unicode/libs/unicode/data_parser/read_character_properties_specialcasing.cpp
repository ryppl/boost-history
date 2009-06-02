// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*
This file is required as inputs:
UNIDATA/SpecialCasing.txt
*/

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

#include <boost/spirit/include/classic.hpp>
#include <boost/lambda/lambda.hpp>

#include "read_character_properties.hpp"
#include "read_character_properties_files.hpp"

using namespace boost::spirit::classic;
using namespace boost::unicode;
using namespace boost::unicode::ucd;
using namespace boost::lambda;

using boost::char32;

namespace boost { namespace unicode { namespace ucd {

const int PROGRESS_INDICATION_EVERY  = 50;

void read_character_properties_specialcasing
	(std::map <char32, character_properties> & props, const char* filename, bool required)
{
	std::cout << "+Reading specialcasing data from " << filename << "\n";

	parse_info <file_iterator<> > info;
	file_iterator<> curr, last;
	/*
	SpecialCasing data contains casing that is not 1-to-1.
	The standard example is German es-zett, whose uppercase is "SS".
	Some characters have simple case mappings in UnicodeData.txt but better
	mappings in SpecialCasing.txt.

	These complex mappings are available for use where changing the length when changing the case is permitted.
	Anything with conditions or locale specifiers is left out currently.
	*/

	try
	{
		curr = get_file_iterator (filename);
	}
	catch (file_read_error)
	{
		if (required)
		{
			throw;
		}
		else
		{
			std::cout << "-Reading special casing from optional file - not present\n";
			return;
		}
	}
	last = curr.make_end();

	eat_comments (curr, last);

	int progress = PROGRESS_INDICATION_EVERY;
	std::cout << " ";

	do
	{
		char32 code;
		complex_casing casing;
		std::string condition;

		/* format is:
		
		<code>; <lower> ; <title> ; <upper> ; (<condition_list> ;)? # <comment>
		<code>, <lower>, <title>, and <upper> provide character values in hex. If there is more
		than one character, they are separated by spaces. Other than as used to separate 
		elements, spaces are to be ignored.

		The <condition_list> is optional. Where present, it consists of one or more locale IDs
		or contexts, separated by spaces. In these conditions:
		 - A condition list overrides the normal behavior if all of the listed conditions are true.
		 - The context is always the context of the characters in the original string,
		   NOT in the resulting string.
		 - Case distinctions in the condition list are not significant.
		 - Conditions preceded by "Not_" represent the negation of the condition.

		 A locale ID is defined by taking any language tag as defined by
		 RFC 3066 (or its successor), and replacing '-' by '_'.

		*/

		info = parse (curr, last,
			hex_p [assign_a (code)] >> ';' >>
			* hex_p [push_back_a (casing.complex_lowercase)] >> ';' >>
			* hex_p [push_back_a (casing.complex_titlecase)] >> ';' >>
			* hex_p [push_back_a (casing.complex_uppercase)] >> ';' >>
			* (anychar_p - (*blank_p >> (';' | eol_p | '#')))
				[assign_a (condition)] >>
			//*(anychar_p - ('#' | eol_p))
            *(anychar_p - eol_p)
			, blank_p);

		if (!info.hit ||
			(casing.complex_lowercase.empty() && casing.complex_titlecase.empty() && 
														casing.complex_uppercase.empty()))
		{
			throw syntax_error (filename, curr, last);
		}
		curr = info.stop;

		eat_comments (curr, last);

		if (progress-- <= 0)
		{
			progress = PROGRESS_INDICATION_EVERY;
			std::cout << ".";
		}

		if (condition.empty())
		{
			// fake_element to feed into lower_bound
			std::map <char32, character_properties>::iterator prop = props.find (code);
			if (prop == props.end())
			{
				std::stringstream ss;
				ss << "codepoint " << std::hex << code
					<< " is in SpecialCasing, but not in UnicodeData.txt while processing "
					<< filename;
				throw std::runtime_error (ss.str());
			} 
            
            if (!prop->second.complex_case.empty())
			{
				std::stringstream ss;
				ss << "codepoint " << std::hex << code
					<< " appears twice in SpecialCasing source files while processing "
					<< filename;
				throw std::runtime_error (ss.str());
			}

			bool complex_reqd = false;
			
			if (casing.complex_uppercase.size() == 1)
			{
				if (prop->second.uppercase == 0)
				{
					if (true/* TODO no condition */)
					{
						// optimisation
						prop->second.uppercase = casing.complex_uppercase[0];
					}
					else
					{
						complex_reqd = true;
					}
				}
				else if (casing.complex_uppercase[0] != prop->second.uppercase)
				{
					std::stringstream ss;
					ss << "codepoint " << std::hex << code
						<< " has inconsistent uppercase data in SpecialCasing source files while processing "
						<< filename;
					throw std::runtime_error (ss.str());
				}
			}
			else
			{
				complex_reqd = true;
			}
			if (casing.complex_titlecase.size() == 1)
			{
				if (prop->second.titlecase == 0)
				{
					if (true/* TODO no condition */)
					{
						// optimisation
						prop->second.titlecase = casing.complex_titlecase[0];
					}
					else
					{
						complex_reqd = true;
					}
				}
				else if (prop->second.titlecase != 0 && casing.complex_titlecase[0] != prop->second.titlecase)
				{
					std::stringstream ss;
					ss << "codepoint " << std::hex << code
						<< " has inconsistent titlecase data in SpecialCasing source files while processing "
						<< filename;
					throw std::runtime_error (ss.str());
				}
			}
			else
			{
				complex_reqd = true;
			}
			if (casing.complex_lowercase.size() == 1)
			{
				if (prop->second.lowercase == 0)
				{
					if (true/* TODO no condition */)
					{
						// optimisation
						prop->second.lowercase = casing.complex_lowercase[0];
					}
					else
					{
						complex_reqd = true;
					}
				}
				else if (prop->second.lowercase != 0 && casing.complex_lowercase[0] != prop->second.lowercase)
				{
					std::stringstream ss;
					ss << "codepoint 0x" << std::hex << code
						<< " has inconsistent lowercase data in SpecialCasing source files while processing "
						<< filename;
					throw std::runtime_error (ss.str());
				}
			}
			else
			{
				complex_reqd = true;
			}

			if (complex_reqd)
			{
				prop->second.complex_case.push_back(casing);
			}
		}
		else
		{
			// TODO
		}
	} while (curr != last);

	std::cout << "\n";

	std::cout << "-Reading specialcasing data from " << filename << "\n";
}

}}}	// namespaces

