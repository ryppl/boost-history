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
//
// c string functions
#include <cstring>

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

// define macro expanding into a compile time const lenght
// of the given string literal
#define BOOST_STR_LIT_LENGTH(STR) ((sizeof(STR)/sizeof(char))-1)


/** Wrappers of cstring functions 
 */

// disable the deprecated function warning on msvc
// this warning is issued when not using the "safe"
// versions of string functions like strcpy_s (vs. strcpy)
#pragma warning(push)
#pragma warning(disable : 4996)


/** string compare
 */

inline size_t bstrlen(const bchar* str)
{
#ifdef BOOST_USE_WIDE_CHARS
	return ::std::wcslen(str);
#else
	return ::std::strlen(str);
#endif
}


	
inline int bstrcmp(const bchar* a, const bchar* b)
{
#ifdef BOOST_USE_WIDE_CHARS
	return ::std::wcscmp(a, b);
#else
	return ::std::strcmp(a, b);
#endif
}

/** string copy
 */
inline bchar* bstrcpy(bchar* dst, const bchar* src)
{
#ifdef BOOST_USE_WIDE_CHARS
	return ::std::wcscpy(dst, src);
#else
	return ::std::strcpy(dst, src);
#endif
}

// enable the deprecated function warnings on msvc
#pragma warning(pop)



} // namespace boost

#endif //include guard


