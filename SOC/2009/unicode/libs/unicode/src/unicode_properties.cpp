// Read Unicode character properties from the database.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/unicode/ucd/properties_types.hpp>
#include <boost/unicode/ucd/detail/unichar_data.hpp>

using namespace boost::unicode::ucd;
using namespace boost;

#define element_count(tbl) (int)(sizeof(tbl)/ sizeof(tbl[0]))

static const char* g_category[] =
{
    "unknown",
	"letter_uppercase",
	"letter_lowercase",
	"letter_titlecase",
	"letter_modifier",
	"letter_other",
	"mark_nonspacing",
	"mark_spacing_combining",
	"mark_enclosing",
	"number_decimal_digit",
	"number_letter",
	"number_other",
	"punctuation_connector",
	"punctuation_dash",
	"punctuation_open",
	"punctuation_close",
	"punctuation_initial_quote",
	"punctuation_final_quote",
	"punctuation_other",
	"symbol_math",
	"symbol_currency",
	"symbol_modifier",
	"symbol_other",
	"separator_space",
	"separator_line",
	"separator_paragraph",
	"other_control",
	"other_format",
	"other_surrogate",
	"other_private_use",
	"other_not_assigned"
};

BOOST_STATIC_ASSERT(element_count(g_category) == category::_count); // check g_category matched category

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::category::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::category::_count);
	return g_category[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_join_type[] =
{
	"none",		
	"right",
	"left",
	"dual",
	"causing",
	"transparent"
};

BOOST_STATIC_ASSERT(element_count(g_join_type) == join_type::_count); // check g_join_type matched join_type

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::join_type::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::join_type::_count);
	return g_join_type[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_bidi_class[] =
{
	"unknown",
	"strong_left_to_right",
	"strong_left_to_right_embedding",
	"strong_left_to_right_override",
	"strong_right_to_left",
	"strong_right_to_left_arabic",
	"strong_right_to_left_embedding",
	"strong_right_to_left_override",
	"weak_pop_direction_format",
	"weak_european_digits",
	"weak_european_number_separator",
	"weak_european_number_terminator",
	"weak_arabic_number",
	"weak_common_number_separator",
	"weak_non_spacing_mark",
	"weak_boundary_neutral",
	"neutral_paragraph_separator",
	"neutral_segment_separator",
	"neutral_whitespace",
	"neutral_other",
};

BOOST_STATIC_ASSERT(element_count(g_bidi_class) == bidi_class::_count); // check g_bidi_class matched bidi_class

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::bidi_class::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::bidi_class::_count);
	return g_bidi_class[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_break_class[] =
{
    "unknown",
	"mandatory",
	"carriage_return",
	"line_feed",
	"combining_mark",
	"next_line",
	"surrogate",
	"word_joiner",
	"zero_width_space",
	"non_breaking",
	"contingent_break_opportunity",
	"space",
	"break_opportunity_before_after",
	"break_opportunity_after",
	"break_opportunity_before",
	"hyphen",
	"closing_punctuation",
	"exclamation_interrogation",
	"inseparable",
	"non_starter",
	"opening_punctuation",
	"ambiguous_quotation",
	"infix_separator",
	"numeric",
	"postfix_numeric",
	"prefix_numeric",
	"symbol_allowing_break",
	"ambiguous",
	"ordinary_alpabetic_or_symbol_character",
	"hangul_lv_syllable",
	"hangul_lvt_syllable",
	"ideograph",
	"hangul_l_jamo",
	"hangul_v_jamo",
	"hangul_t_jamo",
	"complex_context"
};

BOOST_STATIC_ASSERT(element_count(g_break_class) == break_class::_count); // check g_break_class matched break_class

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::break_class::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::break_class::_count);
	return g_break_class[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_break_action[] =
{
	"direct",
	"indirect",
	"combiningIndirect",
	"combiningProhibited",
	"prohibited",
	"always"
};

BOOST_STATIC_ASSERT(element_count(g_break_action) == break_action::_count); // check g_break_action matched break_action

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::break_action::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::break_action::_count);
	return g_break_action[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_decomposition[] =
{
    "none",
	"font",
	"no_break",
	"initial",
	"medial",
	"final",
	"isolated",
	"circle",
	"super",
	"sub",
	"vertical",
	"wide",
	"narrow",
	"small",
	"square",
	"fraction",
	"compat",
	"canonical"
};

BOOST_STATIC_ASSERT(element_count(g_decomposition) == decomposition_type::_count); // check g_decomposition matched decomposition

const char* boost::unicode::ucd::as_string(
	boost::unicode::ucd::decomposition_type::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::decomposition_type::_count);
	return g_decomposition[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_grapheme_cluster_break[] =
{
    "any",
	"cr",
	"lf",
	"control",
	"l",
	"v",
	"t",
	"lv",
	"lvt",
	"extend",
    "prepend",
    "spacing_mark"
};

BOOST_STATIC_ASSERT(element_count(g_grapheme_cluster_break) == grapheme_cluster_break::_count); // check g_grapheme_cluster_break matched grapheme_cluster_break

const char* boost::unicode::ucd::as_string(
	boost::unicode::ucd::grapheme_cluster_break::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::grapheme_cluster_break::_count);
	return g_grapheme_cluster_break[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_word_break[] =
{
    "any",
	"format",
	"katakana",
	"aletter",
	"midletter",
	"midnum",
	"numeric",
	"extendnumlet",
    "cr",
    "lf",
    "newline",
    "extend",
    "midnumlet",
};

BOOST_STATIC_ASSERT(element_count(g_word_break) == word_break::_count); // check g_word_break matched word_break

const char* boost::unicode::ucd::as_string(
	boost::unicode::ucd::word_break::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::word_break::_count);
	return g_word_break[type];
}

// ----------------------------------------------------------------------------------------

static const char* g_sentence_break[] =
{
    "any",
	"sep",
	"format",
	"sp",
	"lower",
	"upper",
	"oletter",
	"numeric",
	"aterm",
	"sterm",
	"close",
    "cr",
    "lf",
    "extend",
    "scontinue"
};

BOOST_STATIC_ASSERT(element_count(g_sentence_break) == sentence_break::_count); // check g_sentence_break matched sentence_break

const char* boost::unicode::ucd::as_string(
	boost::unicode::ucd::sentence_break::type type)
{
	BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::sentence_break::_count);
	return g_sentence_break[type];
}

static const char* g_sort_type[] =
{
	"default_",
	"is_index",
	"zero_data1_data2_cp",
	"data1_0x0020_data2_cp",
};

BOOST_STATIC_ASSERT(element_count(g_sort_type) == boost::unicode::ucd::sort_type::_count); // check g_sort_type matched sort_type

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::sort_type::type type)
{
    BOOST_ASSERT(type >= 0 && type < boost::unicode::ucd::sort_type::_count);
	return g_sort_type[type];
}
