// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*
This file is required as input:
UCA/allkeys.txt
*/

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

#include <boost/spirit/include/classic.hpp>
#include <boost/lambda/lambda.hpp>

#include "read_character_properties_files.hpp"

using namespace boost::spirit::classic;
using namespace boost::unicode;
using namespace boost::unicode::ucd;
using namespace boost::lambda;

using boost::char32;

namespace boost { namespace unicode { namespace ucd {

const int PROGRESS_INDICATION_EVERY  = 200;

struct push_back_collation_element
{
	collation_entry & entry;
public:
	push_back_collation_element (collation_entry & entry)
	: entry (entry) {}

	void operator()
		(const file_iterator<> & begin, const file_iterator<> & end) const
	{
		parse_info <file_iterator<> > info;

		collation_data element;

		info = parse (begin, end,
			(ch_p ('.') [assign_a (element.variable, false)] |
				ch_p ('*') [assign_a (element.variable, true)]) >>
			hex_p [assign_a (element.weight1)] >> '.' >>
			hex_p [assign_a (element.weight2)] >> '.' >>
			hex_p [assign_a (element.weight3)] >> '.' >>
			hex_p [assign_a (element.weight4)]
			, space_p);

		// The data have been syntax-checked by the caller.
		assert (info.hit);
		assert (info.stop == end);

		entry.data.push_back (element);
	}
};

void read_collation_data (
	std::map <char32, character_properties> & props, const char* filename, bool required)
{
	std::cout << "+Reading collation data from " << filename << "\n";

	parse_info <file_iterator<> > info;
	file_iterator<> curr, last;

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
			std::cout << "-Reading word break data from optional file - not present\n";
			return;
		}
	}
	last = curr.make_end();

	eat_comments (curr, last);

    info = parse (curr, last, "@version" >> *(anychar_p - eol_p) >> eol_p, blank_p);
	curr = info.stop;
	eat_comments (curr, last);

	/*
	Read the Default Unicode Collation Element Table from ./UCA/allkeys.txt.
	Every line in the table consists of one or more characters, and one or
	more collation elements the character(s) should be converted to.
	The latter consists of 4 numbers, the fourth of which is computable.
	For example:
	24D0  ; [.0F6C.0020.0006.24D0] # CIRCLED LATIN SMALL LETTER A; QQK
	0041  ; [.0F6C.0020.0008.0041] # LATIN CAPITAL LETTER A
	FF21  ; [.0F6C.0020.0009.FF21] # FULLWIDTH LATIN CAPITAL LETTER A; QQK
	1D400 ; [.0F6C.0020.000B.1D400] # MATHEMATICAL BOLD CAPITAL A; QQK
	1D434 ; [.0F6C.0020.000B.1D434] # MATHEMATICAL ITALIC CAPITAL A; QQK
	*/

	int progress = PROGRESS_INDICATION_EVERY;
	std::cout << " ";

    do
	{
        char32 cp;
        collation_entry entry;

        info = parse (curr, last,
            hex_p [assign_a (cp)] >>
		    // optional additional char32s which we call following_chars
            *hex_p [push_back_a (entry.following_chars)] >> ';' >>
		    // collation elements
		    + ('[' >> (
			    (ch_p ('.') | '*') >>
			    hex_p >> '.' >>
			    hex_p >> '.' >>
			    hex_p >> '.' >> 
                hex_p
		    ) [push_back_collation_element (entry)] >> ']') >>

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

        if (progress-- <= 0)
		{
			progress = PROGRESS_INDICATION_EVERY;
			std::cout << '.' << std::flush;
		}

        std::map <char32, character_properties>::iterator prop = props.find (cp);
		if (prop == props.end())
		{
			std::stringstream ss;
			ss << "codepoint 0x" << std::hex << cp
				<< " is in AllKeys, but not in UnicodeData.txt while processing "
				<< filename;
			throw std::runtime_error (ss.str());
		} 

        assert(entry.data.size() > 0);  // check for internal error !

        if (prop->second.sort_data.empty())
        {
            prop->second.sort_type = sort_type::is_index;
            // we cannot set the index during this pass so we leave it till later
            prop->second.sort_index_or_data1 = (uint16_t)-1;

            // need to set variable flag in character data for consistency
            prop->second.sort_variable = entry.data[0].variable;
        }

        // merge the collation entries
        prop->second.sort_data.push_back(entry);
	} 
    while (curr != last);
    std::cout << "\n";

	std::cout << "-Reading blocks from " << filename << "\n";
}

}}}	// namespaces
