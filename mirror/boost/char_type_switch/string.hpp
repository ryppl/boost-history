/**
 * \file boost/char_width_switch/string.hpp
 * Narrow/Wide character type switching for string.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_WIDTH_SWITCH_STRING
#define BOOST_CHAR_WIDTH_SWITCH_STRING

#include <boost/char_type_switch/choice.hpp>
// Needed for ::std::string / ::std::wstring
#include <string>

namespace boost {


#ifdef BOOST_USE_WIDE_CHARS
	// If wide characters were chosen
	//
	// define character type
	typedef wchar_t bchar;
	// define string class type
	typedef ::std::wstring bstring;
	// define macro for string literal type selection
#	define BOOST_STR_LIT(STR) L##STR
#else // NOT BOOST_USE_WIDE_CHARS
	// if narrow characters were chosen
	//
	// define character type
	typedef char bchar;
	// define string class type
	typedef ::std::string bstring;
	// define macro for string literal type selection
#	define BOOST_STR_LIT(STR) STR
#endif // NOT BOOST_USE_WIDE_CHARS

} // namespace boost

#endif //include guard
