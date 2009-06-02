// Read Unicode character properties from the database.
// Contains helper routines.
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

#include "read_character_properties_files.hpp"

using namespace boost::spirit::classic;
using namespace boost::unicode;
using namespace boost::unicode::ucd;
using namespace boost::lambda;

using boost::char32;

namespace boost { namespace unicode { namespace ucd {

std::string file_read_error::make_message (const std::string & filename)
{
	std::stringstream ss;
	ss << "Could not read from file " << filename;
	return ss.str();
}

std::string syntax_error::make_message (
	const char* filename,
	const boost::spirit::classic::file_iterator<> & first,
	const boost::spirit::classic::file_iterator<> & last)
{
	std::stringstream ss;
	ss << "Syntax error in file \"";
	ss << filename;
	ss << "\" at:\n\"";
	// take a maximum of 150 characters from the file where the error was found
	ss << std::string (first, (last - first < 150) ? last : first + 150);
	ss << "\"\n";
	return ss.str();
}

file_iterator<> get_file_iterator (const char * name)
{
	file_iterator<> first (name);
	if (!first)
	{
		throw file_read_error (name);
	}
	return first;
}

void eat_comments (file_iterator<> & current,
				   const file_iterator<> & last)
{
	parse_info <file_iterator<> > info;
	info = parse (current, last,
		*((*blank_p >> eol_p) | comment_p ('#') | '#')
		);
	current = info.stop;
}

}}}	// namespaces
