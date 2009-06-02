// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*
This file is required as inputs:
UNIDATA/Blocks.txt
*/

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

#include <boost/spirit/include/classic.hpp>
#include <boost/lambda/lambda.hpp>

#include "read_character_properties_files.hpp"

using namespace boost::spirit::classic;
using namespace boost::unicode;
using namespace boost::unicode::ucd;
using namespace boost::lambda;

using boost::char32;

namespace boost { namespace unicode { namespace ucd {

void read_blocks (std::vector <read_block> & blocks)
{
	const char* filename = "./UNIDATA/Blocks.txt";
	std::cout << "+Reading blocks from " << filename << "\n";

	file_iterator<> curr = get_file_iterator (filename);
	file_iterator<> last = curr.make_end();
	eat_comments (curr, last);

	parse_info <file_iterator<> > info;

	do
	{
		read_block b;

		// The line being decoded is formatted as:
		// Start Code..End Code; Block Name
		// e.g. 
		// 0000..007F; Basic Latin

		info = parse (curr, last,
			// start code
			hex_p [assign_a (b.first)] >> ".." >>
			// end code
			hex_p [assign_a (b.last)] >> ';' >>
			// block name
			lexeme_d [+(anychar_p - eol_p - '#')] [assign_a (b.name)] >>
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

		blocks.push_back (b);
	} while (curr != last);

	std::cout << "-Reading blocks from " << filename << "\n";
}

}}}	// namespaces
