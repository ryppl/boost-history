// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/unicode/ucd/detail/unichar_data.hpp>
#include "read_character_properties.hpp"
#include "read_character_properties_files.hpp"
#include "write_character_properties.hpp"

using namespace boost::unicode;
using namespace boost::unicode::ucd;

using boost::char32;


int main()
{
    try
    {
	    std::map <char32, character_properties> props;
	    read_character_properties (props);
	    std::vector <boost::unicode::ucd::read_block> blocks;
	    read_blocks (blocks);

	    write_data data;
	    prepare(props, blocks, data);
	    write(blocks, data, "../src/ucd/", "../../../boost/unicode/ucd/");

		return 0;
	}
	catch (std::exception & e)
	{
		std::cerr << "Error: " << e.what() << "." << std::endl;
		return -1;
	}
}
