// Define Unicode character properties.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNICODE_CHARACTER_PROPERTIES_HPP_INCLUDED
#define BOOST_UNICODE_CHARACTER_PROPERTIES_HPP_INCLUDED

namespace boost
{
namespace unicode
{

	namespace ucd
	{
		/*
		Unicode character types
		*/
	
		struct category
		{
			enum type					// maximum 32 values (5 bits)
			{
				letter_uppercase = 0,
				letter_lowercase,
				letter_titlecase,
				letter_modifier,
				letter_other,
				mark_nonspacing,
				mark_spacing_combining,
				mark_enclosing,
				number_decimal_digit,
				number_letter,
				number_other,
				punctuation_connector,
				punctuation_dash,
				punctuation_open,
				punctuation_close,
				punctuation_initial_quote,
				punctuation_final_quote,
				punctuation_other,
				symbol_math,
				symbol_currency,
				symbol_modifier,
				symbol_other,
				separator_space,
				separator_line,
				separator_paragraph,
				other_control,
				other_format,
				other_surrogate,
				other_private_use,
				// Todo: collapse these values into one?
				// It seems too subtle a difference.
				other_not_assigned,
				unknown,				// default value for unknown characters

				_count
			};
		};
		const char* as_string(category::type);

		struct join_type
		{
			enum type					// maximum 8 values (3 bits)
			{
				none = 0,				// default value for unknown characters
				right,
				left,
				dual,
				causing,
				transparent,
				_count
			};
		};
		const char* as_string(join_type::type);

		struct bidi_class
		{
			enum type					// maximum 32 values (5 bits)
			{
				// default value for unknown characters
				strong_left_to_right = 1,			
				strong_left_to_right_embedding,
				strong_left_to_right_override,
	
				strong_right_to_left,			// = 4
				strong_right_to_left_arabic,
				strong_right_to_left_embedding,
				strong_right_to_left_override,
	
				weak_pop_direction_format,		// = 8
				weak_european_digits,
				weak_european_number_separator,
				weak_european_number_terminator,
				weak_arabic_number,
				weak_common_number_separator,
				weak_non_spacing_mark,
				weak_boundary_neutral,
	
				neutral_paragraph_separator,	// = 16
				neutral_segment_separator,
				neutral_whitespace,
				neutral_other,

				_count
			};
		};
		const char* as_string(bidi_class::type);

		struct break_class
		{
			enum type					// maximum 64 values (6 bits)
			{
				mandatory = 0,
				carriage_return,
				line_feed,
				combining_mark,
				next_line,
				surrogate,
				word_joiner,
				zero_width_space,
				non_breaking,
				contingent_break_opportunity,
				space,
				break_opportunity_before_after,
				break_opportunity_after,
				break_opportunity_before,
				hyphen,
				closing_punctuation,
				exclamation_interrogation,
				inseparable,
				non_starter,
				opening_punctuation,
				ambiguous_quotation,
				infix_separator,
				numeric,
				postfix_numeric,
				prefix_numeric,
				symbol_allowing_break,
				ambiguous,
				ordinary_alpabetic_or_symbol_character,
				hangul_lv_syllable,
				hangul_lvt_syllable,
				ideograph,
				hangul_l_jamo,
				hangul_v_jamo,
				hangul_t_jamo,
				complex_context,
				unknown,				// default value for unknown characters

				_count
			};
		};
		const char* as_string(break_class::type);
		 
		struct break_action					// maximum 8 values (3 bits)
		{
			enum type
			{
				direct = 0,
				indirect = 1,				// default value for unknown characters
				combining_indirect = 2,
				combining_prohibited = 3,
				prohibited = 4,		
				always = 5,

				_count
			};
		};
		const char* as_string(break_action::type);

		struct decomposition_type
		{
			enum type					// maximum 32 values (5 bits)
			{
				font = 0,
				no_break,
				initial,
				medial,
				final,
				isolated,
				circle,
				super,
				sub,
				vertical,
				wide,
				narrow,
				small,
				square,
				fraction,
				compat,
				canonical,
				none,					// default value for unknown characters

				_count
			};
		};
		const char* as_string(decomposition_type::type);

        struct grapheme_cluster_break
        {
            enum type			        // maximum 16 values (4 bits)
            {
                cr = 0,                 // value fixed
                lf = 1,                 // value fixed
                control = 2,            // value fixed
                l = 3,                  // value fixed
                v = 4,                  // value fixed
                t = 5,                  // value fixed
                lv = 6,                 // value fixed
                lvt = 7,                // value fixed
                extend = 8,             // value fixed
                // default value for unknown characters 
                any = 9,              // value fixed
   
				_count
            };
        };
		const char* as_string(grapheme_cluster_break::type);

        struct word_break
        {
            enum type			        // maximum 8 values (3 bits)
            {
                format = 0,             
                katakana,
                aletter,
                midletter,
                midnum,
                numeric,
                extendnumlet,
                any,                    // default value for unknown characters
   
				_count
            };
        };
		const char* as_string(word_break::type);

        struct sentence_break
        {
            enum type			        // maximum 16 values (4 bits)
            {
                sep = 0,             
                format,
                sp,
                lower,
                upper,
                oletter,
                numeric,
                aterm,
                sterm,
                close,
                any,                    // default value for unknown characters
   
				_count
            };
        };
		const char* as_string(sentence_break::type);

	}	// namespace ucd

}   // namespace unicode
}	// namespace boost

#endif	// BOOST_UNICODE_CHARACTER_PROPERTIES_HPP_INCLUDED
