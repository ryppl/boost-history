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

const int PROGRESS_INDICATION_EVERY  = 200;

tuple<char32, character_properties> read_one_character
	(file_iterator<> & curr, const file_iterator<> & last, const char* filename)
{
	parse_info <file_iterator<> > info;

	char32 character;
	character_properties prop;

	// set optionals to reasonable values
	prop.combining = 0;
	prop.uppercase = 0;
	prop.lowercase = 0;
	prop.titlecase = 0;

	info = parse (curr, last,
		// char32
		hex_p [assign_a (character)]
		>> ';' >>
		// name (don't include blanks that come after the name)
		// I'm not sure the valid characters are specified anywhere.
		// These were collected by testing on UnicodeData 4.1.0.
		lexeme_d [*((alnum_p | ' ' | '<' | '>' | '-' | ',')
			- (*blank_p >> ';'))]
			[assign_a (prop.name)]
		>> ';' >>
		// General category
		(str_p ("Lu")
			[assign_a (prop.general_category,
				category::letter_uppercase)] |
		str_p ("Ll")
			[assign_a (prop.general_category,
				category::letter_lowercase)] |
		str_p ("Lt")
			[assign_a (prop.general_category,
				category::letter_titlecase)] |
		str_p ("Lm")
			[assign_a (prop.general_category,
				category::letter_modifier)] |
		str_p ("Lo")
			[assign_a (prop.general_category,
				category::letter_other)] |
		str_p ("Mn")
			[assign_a (prop.general_category,
				category::mark_nonspacing)] |
		str_p ("Mc")
			[assign_a (prop.general_category,
				category::mark_spacing_combining)] |
		str_p ("Me")
			[assign_a (prop.general_category,
				category::mark_enclosing)] |
		str_p ("Nd")
			[assign_a (prop.general_category,
				category::number_decimal_digit)] |
		str_p ("Nl")
			[assign_a (prop.general_category,
				category::number_letter)] |
		str_p ("No")
			[assign_a (prop.general_category,
				category::number_other)] |
		str_p ("Pc")
			[assign_a (prop.general_category,
				category::punctuation_connector)] |
		str_p ("Pd")
			[assign_a (prop.general_category,
				category::punctuation_dash)] |
		str_p ("Ps")
			[assign_a (prop.general_category,
				category::punctuation_open)] |
		str_p ("Pe")
			[assign_a (prop.general_category,
				category::punctuation_close)] |
		str_p ("Pi")
			[assign_a (prop.general_category,
				category::punctuation_initial_quote)] |
		str_p ("Pf")
			[assign_a (prop.general_category,
				category::punctuation_final_quote)] |
		str_p ("Po")
			[assign_a (prop.general_category,
				category::punctuation_other)] |
		str_p ("Sm")
			[assign_a (prop.general_category,
				category::symbol_math)] |
		str_p ("Sc")
			[assign_a (prop.general_category,
				category::symbol_currency)] |
		str_p ("Sk")
			[assign_a (prop.general_category,
				category::symbol_modifier)] |
		str_p ("So")
			[assign_a (prop.general_category,
				category::symbol_other)] |
		str_p ("Zs")
			[assign_a (prop.general_category,
				category::separator_space)] |
		str_p ("Zl")
			[assign_a (prop.general_category,
				category::separator_line)] |
		str_p ("Zp")
			[assign_a (prop.general_category,
				category::separator_paragraph)] |
		str_p ("Cc")
			[assign_a (prop.general_category,
				category::other_control)] |
		str_p ("Cf")
			[assign_a (prop.general_category,
				category::other_format)] |
		str_p ("Cs")
			[assign_a (prop.general_category,
				category::other_surrogate)] |
		str_p ("Co")
			[assign_a (prop.general_category,
				category::other_private_use)]
		// No characters in the file have this property, according to
		// UCD.html
/*				str_p ("Cn")
			[assign_a (prop.general_category,
				category::other_not_assigned)]*/
		) >> ';' >>

		// Combining class
		!int_p [assign_a (prop.combining)]
		>> ';' >>

		// Bidi
		((str_p ("L")
			[assign_a (prop.bidi,
				bidi_class::strong_left_to_right)] >> ';') |
		(str_p ("LRE")
			[assign_a (prop.bidi,
				bidi_class::strong_left_to_right_embedding)] >> ';') |
		(str_p ("LRO")
			[assign_a (prop.bidi,
				bidi_class::strong_left_to_right_override)] >> ';') |
		(str_p ("R")
			[assign_a (prop.bidi,
				bidi_class::strong_right_to_left)] >> ';') |
		(str_p ("AL")
			[assign_a (prop.bidi,
				bidi_class::strong_right_to_left_arabic)] >> ';') |
		(str_p ("RLE")
			[assign_a (prop.bidi,
				bidi_class::strong_right_to_left_embedding)] >> ';') |
		(str_p ("RLO")
			[assign_a (prop.bidi,
				bidi_class::strong_right_to_left_override)] >> ';') |

		(str_p ("PDF")
			[assign_a (prop.bidi,
				bidi_class::weak_pop_direction_format)] >> ';') |
		(str_p ("EN")
			[assign_a (prop.bidi,
				bidi_class::weak_european_digits)] >> ';') |
		(str_p ("ES")
			[assign_a (prop.bidi,
				bidi_class::weak_european_number_separator)] >> ';') |
		(str_p ("ET")
			[assign_a (prop.bidi,
				bidi_class::weak_european_number_terminator)] >> ';') |
		(str_p ("AN")
			[assign_a (prop.bidi,
				bidi_class::weak_arabic_number)] >> ';') |
		(str_p ("CS")
			[assign_a (prop.bidi,
				bidi_class::weak_common_number_separator)] >> ';') |
		(str_p ("NSM")
			[assign_a (prop.bidi,
				bidi_class::weak_non_spacing_mark)] >> ';') |
		(str_p ("BN")
			[assign_a (prop.bidi,
				bidi_class::weak_boundary_neutral)] >> ';') |

		(str_p ("B")
			[assign_a (prop.bidi,
				bidi_class::neutral_paragraph_separator)] >> ';') |
		(str_p ("S")
			[assign_a (prop.bidi,
				bidi_class::neutral_segment_separator)] >> ';') |
		(str_p ("WS")
			[assign_a (prop.bidi,
				bidi_class::neutral_whitespace)] >> ';') |
		(str_p ("ON")
			[assign_a (prop.bidi,
				bidi_class::neutral_other)] >> ';')
		) >>

		// Decomposition type.
		// If none is indicated, it's a canonical decomposition_type.
		(((str_p ("<font>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::font)] |
		str_p ("<noBreak>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::no_break)] |
		str_p ("<initial>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::initial)] |
		str_p ("<medial>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::medial)] |
		str_p ("<final>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::final)] |
		str_p ("<isolated>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::isolated)] |
		str_p ("<circle>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::circle)] |
		str_p ("<super>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::super)] |
		str_p ("<sub>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::sub)] |
		str_p ("<vertical>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::vertical)] |
		str_p ("<wide>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::wide)] |
		str_p ("<narrow>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::narrow)] |
		str_p ("<small>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::small)] |
		str_p ("<square>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::square)] |
		str_p ("<fraction>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::fraction)] |
		str_p ("<compat>")
			[assign_a (prop.decomposition_kind,
				decomposition_type::compat)] |
		eps_p [assign_a (prop.decomposition_kind,
			decomposition_type::canonical)]
		) >>
		// composition
		+(hex_p [push_back_a (prop.decomposition)])
		) |
		// or no composition at all
		eps_p [assign_a (prop.decomposition_kind, decomposition_type::none)]
		) >> ';' >>

		// numeric type is skipped
		*(anychar_p - ';') >> ';' >>
		*(anychar_p - ';') >> ';' >>
		*(anychar_p - ';') >> ';' >>

		// Bidi_Mirrored is skipped
		*(anychar_p - ';') >> ';' >>

		// Unicode_1_Name is skipped
		*(anychar_p - ';') >> ';' >>

		// ISO_Comment is skipped
		*(anychar_p - ';') >> ';' >>

		// simple uppercase mapping
		! hex_p [assign_a (prop.uppercase)] >> ';' >>

		// simple lowercase mapping
		! hex_p [assign_a (prop.lowercase)] >> ';' >>

		// simple titlecase mapping
		! hex_p [assign_a (prop.titlecase)] >>

		// Skip any remaining entries, i.e., parse future versions
		// of the table as well.
		//*(anychar_p - ('#' | eol_p))
        *(anychar_p - eol_p)
		, blank_p);

	if (!info.hit)
	{
		throw syntax_error (filename, curr, last);
	}
	if (prop.combining < 0 || prop.combining > 255)
	{
		std::stringstream ss;
		ss << "Combining level out of range for char 0x" << 
									std::hex << character << " while processing " << filename;
		throw std::runtime_error (ss.str());
	}

	curr = info.stop;

	return make_tuple(character, prop);
}

void read_character_properties_unicodedata
	(std::map <char32, character_properties> & props, const char* filename, bool required)
{
	std::cout << "+Reading unicodedata from " << filename << "\n";

	file_iterator<> curr, last;
	/*
	Read the data from UnicodeData.txt.
	This is the bulk of the data: it contains all characters and their basic
	properties.
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
			std::cout << "-Reading unicodedata from optional file - not present\n";
			return;
		}
	}
	last = curr.make_end();

	eat_comments (curr, last);

	int progress = PROGRESS_INDICATION_EVERY;
	std::cout << " ";

	do
	{
		char32 character;
		character_properties prop;
		tie(character, prop) = read_one_character (curr, last, filename);
		eat_comments (curr, last);

		if (progress-- <= 0)
		{
			progress = PROGRESS_INDICATION_EVERY;
			std::cout << '.' << std::flush;
		}

		// A range in UnicodeData.txt is signified by one character named
		// <..., First> and one <..., Last>
		if (prop.name.find (", First") != std::string::npos)
		{
			if (!required)
			{
				// oops - user data trying to overwrite unicode data
				std::stringstream ss;
				ss << "Ranges are not supported in user data while processing " << filename;
				throw std::runtime_error (ss.str());
			}

			char32 character2;
			character_properties prop2;
			tie(character2, prop2) = read_one_character (curr, last, filename);
			eat_comments (curr, last);

			prop.name.erase (prop.name.find (", First"), 7);
			prop2.name.erase (prop2.name.find (", Last"), 6);
			if (prop.name != prop2.name)
			{
				std::stringstream ss;
				ss << "Names for curr of range and last of range do not match: "
					<< prop.name << " vs. " << prop2.name << " while processing " << filename;
				throw std::runtime_error (ss.str());
			}
			if (character >= character2)
			{
				std::stringstream ss;
				ss << "Range has a negative length " << character
					<< " >= " << character2 << " while processing " << filename;
				throw std::runtime_error (ss.str());
			}
			
			prop.written_by_block = true;
			do
			{
				if (props.find(character) != props.end())
				{
						std::stringstream ss;
					ss << "Duplicate char32(s) in UnicodeData.txt. "
						"The curr duplicate char32 is " << std::hex <<
						character << " while processing " << filename;
					throw std::runtime_error (ss.str());
				}

				props[character] = prop;
				++ character;
				// warning: we exit the loop one character early as the last character is written below
			} while (character < character2);
		}
		else if (!required)
		{
			std::map <char32, character_properties>::const_iterator preExisting = props.find(character);
			if (preExisting != props.end())
			{
				// already exists
				if (!(*preExisting).second.written_by_block)
				{
					// oops - user data trying to overwrite unicode data
					std::stringstream ss;
					ss << "User data trying to overwrite unicode data. "
						"The curr duplicate char32 is " << std::hex <<
						character << " while processing " << filename;
					throw std::runtime_error (ss.str());
				}
				else
				{
					// update it
					props[character] = prop;
					continue;
				}
			}
		}

		if (props.find(character) != props.end())
		{
				std::stringstream ss;
			ss << "Duplicate char32(s) in UnicodeData.txt. "
				"The curr duplicate char32 is " << std::hex <<
				character << " while processing " << filename;
			throw std::runtime_error (ss.str());
		}

		props[character] = prop;
	} while (curr != last);
	std::cout << "\n";

	std::cout << "-Reading unicodedata from " << filename << "\n";
}

}}}	// namespaces

