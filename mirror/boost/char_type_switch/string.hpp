/**
 * \file boost/char_type_switch/string.hpp
 * Narrow/Wide character type switching for string.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_STRING
#define BOOST_CHAR_TYPE_SWITCH_STRING


#include <boost/char_type_switch/choice.hpp>
#include <boost/config.hpp>
// Needed for ::std::string / ::std::wstring
#include <string>
//
// c string functions
#include <cstring>

namespace boost {
namespace cts {


#ifdef BOOST_CTS_USE_WIDE_CHARS

#include <boost/preprocessor/wstringize.hpp>

	// If wide characters were picked
	//
	// define character type
	typedef wchar_t bchar;
	// define string class type
	typedef ::std::wstring bstring;
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
	// define string class type
	typedef ::std::string bstring;
	// define macro for string literal type selection
#	define BOOST_CTS_LIT(STR) STR
	// stringization
#	define BOOST_CTS_STRINGIZE(TO_TEXT) BOOST_PP_STRINGIZE(TO_TEXT)
#endif // NOT BOOST_CTS_USE_WIDE_CHARS

// define macro expanding into a compile time const length
// of the given string literal
#define BOOST_CTS_LIT_LENGTH(STR) ((sizeof(STR)/sizeof(::boost::bchar))-1)


/** Wrappers of cstring functions 
 */

// disable the deprecated function warning on msvc
// this warning is issued when not using the "safe"
// versions of string functions like strcpy_s (vs. strcpy)
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4996)
#endif


/** string compare
 */

inline size_t bstrlen(const bchar* str)
{
#ifdef BOOST_CTS_USE_WIDE_CHARS
	return ::std::wcslen(str);
#else
	return ::std::strlen(str);
#endif
}


	
inline int bstrcmp(const bchar* a, const bchar* b)
{
#ifdef BOOST_CTS_USE_WIDE_CHARS
	return ::std::wcscmp(a, b);
#else
	return ::std::strcmp(a, b);
#endif
}

/** string copy
 */
inline bchar* bstrcpy(bchar* dst, const bchar* src)
{
#ifdef BOOST_CTS_USE_WIDE_CHARS
	return ::std::wcscpy(dst, src);
#else
	return ::std::strcpy(dst, src);
#endif
}

/** string copy
 */
inline bchar* bstrncpy(bchar* dst, const bchar* src, size_t count)
{
#ifdef BOOST_CTS_USE_WIDE_CHARS
	return ::std::wcsncpy(dst, src, count);
#else
	return ::std::strncpy(dst, src, count);
#endif
}

// enable the deprecated function warnings on msvc
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

} // namespace cts
} // namespace boost

#endif //include guard


