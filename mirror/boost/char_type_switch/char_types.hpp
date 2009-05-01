/**
 * \file boost/char_type_switch/char_types.hpp
 * Supported character types and char-type-prefixes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_CHAR_TYPES_HPP
#define BOOST_CHAR_TYPE_SWITCH_CHAR_TYPES_HPP


#include <boost/char_type_switch/choice.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/list/for_each_i.hpp>


// the prefixes for various char type literals
#define BOOST_CTS_PREFIX_char() BOOST_PP_EMPTY()
#define BOOST_CTS_PREFIX_wchar_t() L
#define BOOST_CTS_PREFIX_char16_t() u
#define BOOST_CTS_PREFIX_char32_t() U

#define BOOST_CTS_PREFIX_CHAR_T(CHAR_T) \
	BOOST_PP_CAT(BOOST_CTS_PREFIX_, CHAR_T)()

// here the switching is done
#if defined BOOST_CTS_USE_WIDE_CHARS
#define BOOST_CTS_BCHAR() wchar_t
#else
#define BOOST_CTS_BCHAR() char
#endif

// if we want to use just the bchars 
#ifdef BOOST_CTS_USE_BCHARS_ONLY
#define BOOST_CTS_CHAR_TYPE_SEQ() (BOOST_CTS_BCHAR())
#define BOOST_CTS_CHAR_TYPE_LIST() (BOOST_CTS_BCHAR(), BOOST_PP_NIL)
#else
// a PP sequence of supported character types
#define BOOST_CTS_CHAR_TYPE_SEQ() (char)(wchar_t)
#define BOOST_CTS_CHAR_TYPE_LIST() (char, (wchar_t, BOOST_PP_NIL))
#endif

// returns the prefix for bchar literals
#define BOOST_CTS_BCHAR_PREFIX() \
	BOOST_PP_CAT(BOOST_CTS_PREFIX_,BOOST_CTS_BCHAR())()


#define BOOST_CTS_FOR_EACH_CHAR_T(MACRO, DATA) \
	BOOST_PP_SEQ_FOR_EACH_I(MACRO, DATA, BOOST_CTS_CHAR_TYPE_SEQ())

#define BOOST_CTS_FOR_EACH_CHAR_T_2(MACRO, DATA) \
	BOOST_PP_LIST_FOR_EACH_I(MACRO, DATA, BOOST_CTS_CHAR_TYPE_LIST())


#endif //include guard


