// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


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

void read_character_properties_arabicshaping
	(std::map <char32, character_properties> & props, const char* filename, bool required)
{
	std::cout << "+Reading arabicshaping from " << filename << "\n";

	parse_info <file_iterator<> > info;
	file_iterator<> curr, last;
	/*
	Read file containing information about the join type of Arabic characters.
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
			std::cout << "-Reading arabicshaping from optional file - not present\n";
			return;
		}
	}

	last = curr.make_end();

	eat_comments (curr, last);

	do
	{
		char32 character;
		join_type::type joining;

		// The line being decoded is formatted as:
		// Field 0: the code point, in 4-digit hexadecimal form, of an Arabic or Syriac character.
		// Field 1: gives a short schematic name for that character, abbreviated from Unicode character name.
		// Field 2: defines the joining type (property name: Joining_Type) [RLDCUT]
		// Field 3: defines the joining group (property name: Joining_Group)
		// e.g.
		// 0629; TEH MARBUTA; R; TEH MARBUTA

		info = parse (curr, last,
			// hex character code
			hex_p [assign_a (character)] >> ';' >>
			// skip character name
			+(anychar_p - ';') >> ';' >>
			// joining type
			(
			ch_p('R') [assign_a (joining, join_type::right)] |
			ch_p('L') [assign_a (joining, join_type::left)] |
			ch_p('D') [assign_a (joining, join_type::dual)] |
			ch_p('C') [assign_a (joining, join_type::causing)] |
			ch_p('U') [assign_a (joining, join_type::none)] |
			ch_p('T') [assign_a (joining, join_type::transparent)]
			) >> ';' >>
			// TODO: Revisit this - Do we need the joining group ?
			// Skip any remaining entries, i.e., parse future versions
			// of the table as well.
            //*(anychar_p - ('#' | eol_p))
            *(anychar_p - eol_p)
			, blank_p);

		if (!info.hit)
		{
			throw syntax_error (filename, curr, last);
		}
		curr = info.stop;

		eat_comments (curr, last);

		std::map <char32, character_properties>::iterator prop = props.find (character);
		if (prop == props.end())
		{
			std::stringstream ss;
			ss << "codepoint " << std::hex << character
				<< " is in ArabicShaping.txt, but not in UnicodeData.txt.";
			throw std::runtime_error (ss.str());
		} 
        
        prop->second.joining = joining;

    } while (curr != last);

	std::cout << "-Reading arabicshaping from " << filename << "\n";
}

}}}	// namespaces

