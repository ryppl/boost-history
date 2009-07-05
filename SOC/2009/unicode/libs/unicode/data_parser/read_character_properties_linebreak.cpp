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

void read_character_properties_linebreak(
	                std::map <char32, character_properties> & props, 
                                    const char* filename, bool required)
{
	std::cout << "+Reading line break data from " << filename << "\n";

	parse_info <file_iterator<> > info;
	file_iterator<> curr, last;

	/*
	Read file containing information about the line breaking properties of characters.
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
			std::cout << "-Reading line break data from optional file - not present\n";
			return;
		}
	}
	last = curr.make_end();

	eat_comments (curr, last);

	int progress = PROGRESS_INDICATION_EVERY;
	std::cout << " ";

	do
	{
		// The format is two fields separated by a semicolon.
		// Field 0: Unicode value
		// Field 1: LineBreak property, consisting of a fixed set of two letter values
		// e.g. 
		// 0000;CM # <control>
		// or
		// F0000..FFFFD;XX # <Plane 15 Private Use, First>..<Plane 15 Private Use, Last>

		char32 first_code, last_code;
		line_break::type break_type;
		info = parse (curr, last,
			// start code
			((hex_p [assign_a (first_code)] >> ".." >>
			// end code
			hex_p [assign_a (last_code)]) |
			// one code
			hex_p [assign_a (first_code)][assign_a (last_code)])
			>> ';' >>
			// block name
			(
			str_p("BK") [assign_a (break_type,
				line_break::mandatory)] |
			str_p("CR") [assign_a (break_type,
				line_break::carriage_return)] |
			str_p("LF") [assign_a (break_type,
				line_break::line_feed)] |
			str_p("CM") [assign_a (break_type,
				line_break::combining_mark)] |
			str_p("NL") [assign_a (break_type,
				line_break::next_line)] |
			str_p("SG") [assign_a (break_type,
				line_break::surrogate)] |
			str_p("WJ") [assign_a (break_type,
				line_break::word_joiner)] |
			str_p("ZW") [assign_a (break_type,
				line_break::zero_width_space)] |
			str_p("GL") [assign_a (break_type,
				line_break::non_breaking)] |
			str_p("CB") [assign_a (break_type,
				line_break::contingent_break_opportunity)] |
			str_p("SP") [assign_a (break_type,
				line_break::space)] |
			str_p("B2") [assign_a (break_type,
				line_break::break_opportunity_before_after)] |
			str_p("BA") [assign_a (break_type,
				line_break::break_opportunity_after)] |
			str_p("BB") [assign_a (break_type,
				line_break::break_opportunity_before)] |
			str_p("HY") [assign_a (break_type,
				line_break::hyphen)] |

			str_p("CL") [assign_a (break_type,
				line_break::closing_punctuation)] |
			str_p("EX") [assign_a (break_type,
				line_break::exclamation_interrogation)] |
			str_p("IN") [assign_a (break_type,
				line_break::inseparable)] |
			str_p("NS") [assign_a (break_type,
				line_break::non_starter)] |
			str_p("OP") [assign_a (break_type,
				line_break::opening_punctuation)] |
			str_p("QU") [assign_a (break_type,
				line_break::ambiguous_quotation)] |

			str_p("IS") [assign_a (break_type,
				line_break::infix_separator)] |
			str_p("NU") [assign_a (break_type,
				line_break::numeric)] |
			str_p("PO") [assign_a (break_type,
				line_break::postfix_numeric)] |
			str_p("PR") [assign_a (break_type,
				line_break::prefix_numeric)] |
			str_p("SY") [assign_a (break_type,
				line_break::symbol_allowing_break)] |

			str_p("AI") [assign_a (break_type,
				line_break::ambiguous)] |
			str_p("AL") [assign_a (break_type,
				line_break::ordinary_alpabetic_or_symbol_character)] |
			str_p("H2") [assign_a (break_type,
				line_break::hangul_lv_syllable)] |
			str_p("H3") [assign_a (break_type,
				line_break::hangul_lvt_syllable)] |
			str_p("ID") [assign_a (break_type,
				line_break::ideograph)] |
			str_p("JL") [assign_a (break_type,
				line_break::hangul_l_jamo)] |
			str_p("JV") [assign_a (break_type,
				line_break::hangul_v_jamo)] |
			str_p("JT") [assign_a (break_type,
				line_break::hangul_t_jamo)] |
			str_p("SA") [assign_a (break_type,
				line_break::complex_context)] |
			str_p("XX") [assign_a (break_type,
				line_break::unknown)] |
            (+alnum_p) [warning_assign_a (break_type,
				line_break::unknown)]
			) >>
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
			std::cout << ".";
		}

		char32 curr_code = first_code;

		do
		{
			std::map <char32, character_properties>::iterator prop = props.find (curr_code);
			if (prop == props.end())
			{
				std::stringstream ss;
				ss << "codepoint 0x" << std::hex << curr_code
					<< " is in LineBreak, but not in UnicodeData.txt while processing "
					<< filename;
				throw std::runtime_error (ss.str());
			} 

			if (prop->second.line_break != (line_break::type)-1)
			{
				std::stringstream ss;
				ss << "codepoint " << std::hex << curr_code
					<< " appears twice in LineBreak source files while processing "
					<< filename;
				throw std::runtime_error (ss.str());
			}

			prop->second.line_break = break_type;

            ++ curr_code;
		}
		while (curr_code <= last_code);

	} while (curr != last);

	std::cout << "\n";

	std::cout << "-Reading line break data from " << filename << "\n";
}

}}}	// namespaces

