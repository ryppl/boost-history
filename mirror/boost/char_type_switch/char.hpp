/**
 * \file boost/char_type_switch/char.hpp
 * Narrow/Wide character type switching.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_CHAR_HPP
#define BOOST_CHAR_TYPE_SWITCH_CHAR_HPP


#include <boost/char_type_switch/choice.hpp>
#include <boost/config.hpp>

namespace boost {
namespace cts {


#ifdef BOOST_CTS_USE_WIDE_CHARS

#include <boost/preprocessor/wstringize.hpp>

	// If wide characters were picked
	//
	// define character type
	typedef wchar_t bchar;
	// define macro for string literal type selection
#	define BOOST_CTS_LIT(STR) L##STR
	// stringization
#	define BOOST_CTS_STRINGIZE(TO_TEXT) BOOST_PP_WSTRINGIZE(TO_TEXT)
#else // NOT BOOST_CTS_USE_WIDE_CHARS

#include <boost/preprocessor/stringize.hpp>

	// if narrow characters were chosen
	//
	// define character type
	typedef char bchar;
	// define macro for string literal type selection
#	define BOOST_CTS_LIT(STR) STR
	// stringization
#	define BOOST_CTS_STRINGIZE(TO_TEXT) BOOST_PP_STRINGIZE(TO_TEXT)
#endif // NOT BOOST_CTS_USE_WIDE_CHARS

// define macro expanding into a compile time const length
// of the given string literal
#define BOOST_CTS_LIT_LENGTH(STR) ((sizeof(BOOST_CTS_LIT(STR))/sizeof(::boost::cts::bchar))-1)


} // namespace cts
} // namespace boost

#endif //include guard


