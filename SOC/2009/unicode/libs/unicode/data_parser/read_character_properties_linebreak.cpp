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
		break_class::type break_type;
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
				break_class::mandatory)] |
			str_p("CR") [assign_a (break_type,
				break_class::carriage_return)] |
			str_p("LF") [assign_a (break_type,
				break_class::line_feed)] |
			str_p("CM") [assign_a (break_type,
				break_class::combining_mark)] |
			str_p("NL") [assign_a (break_type,
				break_class::next_line)] |
			str_p("SG") [assign_a (break_type,
				break_class::surrogate)] |
			str_p("WJ") [assign_a (break_type,
				break_class::word_joiner)] |
			str_p("ZW") [assign_a (break_type,
				break_class::zero_width_space)] |
			str_p("GL") [assign_a (break_type,
				break_class::non_breaking)] |
			str_p("CB") [assign_a (break_type,
				break_class::contingent_break_opportunity)] |
			str_p("SP") [assign_a (break_type,
				break_class::space)] |
			str_p("B2") [assign_a (break_type,
				break_class::break_opportunity_before_after)] |
			str_p("BA") [assign_a (break_type,
				break_class::break_opportunity_after)] |
			str_p("BB") [assign_a (break_type,
				break_class::break_opportunity_before)] |
			str_p("HY") [assign_a (break_type,
				break_class::hyphen)] |

			str_p("CL") [assign_a (break_type,
				break_class::closing_punctuation)] |
			str_p("EX") [assign_a (break_type,
				break_class::exclamation_interrogation)] |
			str_p("IN") [assign_a (break_type,
				break_class::inseparable)] |
			str_p("NS") [assign_a (break_type,
				break_class::non_starter)] |
			str_p("OP") [assign_a (break_type,
				break_class::opening_punctuation)] |
			str_p("QU") [assign_a (break_type,
				break_class::ambiguous_quotation)] |

			str_p("IS") [assign_a (break_type,
				break_class::infix_separator)] |
			str_p("NU") [assign_a (break_type,
				break_class::numeric)] |
			str_p("PO") [assign_a (break_type,
				break_class::postfix_numeric)] |
			str_p("PR") [assign_a (break_type,
				break_class::prefix_numeric)] |
			str_p("SY") [assign_a (break_type,
				break_class::symbol_allowing_break)] |

			str_p("AI") [assign_a (break_type,
				break_class::ambiguous)] |
			str_p("AL") [assign_a (break_type,
				break_class::ordinary_alpabetic_or_symbol_character)] |
			str_p("H2") [assign_a (break_type,
				break_class::hangul_lv_syllable)] |
			str_p("H3") [assign_a (break_type,
				break_class::hangul_lvt_syllable)] |
			str_p("ID") [assign_a (break_type,
				break_class::ideograph)] |
			str_p("JL") [assign_a (break_type,
				break_class::hangul_l_jamo)] |
			str_p("JV") [assign_a (break_type,
				break_class::hangul_v_jamo)] |
			str_p("JT") [assign_a (break_type,
				break_class::hangul_t_jamo)] |
			str_p("SA") [assign_a (break_type,
				break_class::complex_context)] |
			str_p("XX") [assign_a (break_type,
				break_class::unknown)] |
            (+alnum_p) [warning_assign_a (break_type,
				break_class::unknown)]
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

			if (prop->second.line_break != (break_class::type)-1)
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

