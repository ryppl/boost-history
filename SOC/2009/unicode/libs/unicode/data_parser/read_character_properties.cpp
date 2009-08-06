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

void add_unknown_character(std::map <char32, character_properties> & props)
{
	// finally add the dead character - used when a character does not exist in the unicode range
	std::map <char32, character_properties>::const_iterator iter_not_present = 
									props.find(character_properties::CHARACTER_DOES_NOT_EXIST);
	if (iter_not_present != props.end())
	{
		std::stringstream ss;
		ss << "codepoint " << std::hex << character_properties::CHARACTER_DOES_NOT_EXIST
			<< " appears already in files - this is an internal character used during processing "
			<< " only so please change its value";
		throw std::runtime_error (ss.str());
	}

	character_properties char_not_present;
	char_not_present.general_category = category::unknown;
	char_not_present.combining = 0;
	char_not_present.bidi = bidi_class::strong_left_to_right;
	char_not_present.decomposition_kind = decomposition_type::none;
	char_not_present.line_break = line_break::unknown;
	char_not_present.joining = join_type::none;
	char_not_present.unknown_char = true;
	props[character_properties::CHARACTER_DOES_NOT_EXIST] = char_not_present;
}

void read_character_properties (std::map <char32, character_properties> & props)
{
	read_character_properties_unicodedata (props, "./UNIDATA/UnicodeData.txt", true);
	read_character_properties_unicodedata (props, "./USERDATA/UnicodeData.txt", false);
	read_character_properties_specialcasing (props, "./UNIDATA/SpecialCasing.txt", true);
	read_character_properties_specialcasing (props, "./USERDATA/SpecialCasing.txt", false);
	read_character_properties_linebreak (props, "./UNIDATA/LineBreak.txt", true);
	read_character_properties_linebreak (props, "./USERDATA/LineBreak.txt", false);
	read_character_properties_arabicshaping (props, "./UNIDATA/ArabicShaping.txt", true);
	read_character_properties_arabicshaping (props, "./USERDATA/ArabicShaping.txt", false);
	read_character_properties_grapheme_break (props, "./UNIDATA/auxiliary/GraphemeBreakProperty.txt", true);
	read_character_properties_grapheme_break (props, "./USERDATA/auxiliary/GraphemeBreakProperty.txt", false);
	read_character_properties_word_break (props, "./UNIDATA/auxiliary/WordBreakProperty.txt", true);
	read_character_properties_word_break (props, "./USERDATA/auxiliary/WordBreakProperty.txt", false);
	read_character_properties_sentence (props, "./UNIDATA/auxiliary/SentenceBreakProperty.txt", true);
	read_character_properties_sentence (props, "./USERDATA/auxiliary/SentenceBreakProperty.txt", false);
    read_collation_data(props, "./UNIDATA/UCA/allkeys.txt", true);
    read_collation_data(props, "./USERDATA/UCA/allkeys.txt", false);
    
    read_character_properties_compex (props, "./UNIDATA/CompositionExclusions.txt", true);
    read_character_properties_compex (props, "./USERDATA/CompositionExclusions.txt", false);

	add_unknown_character(props);
}

}}}	// namespaces

