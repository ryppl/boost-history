/**
 * \file boost/char_type_switch/iostream.hpp
 * Narrow/Wide character type switching for iostreams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_IOSTREAM
#define BOOST_CHAR_TYPE_SWITCH_IOSTREAM

#include <boost/char_type_switch/string.hpp>
// Needed for ::std cout, cin, cerr, wcin, wcout, wcerr
#include <iostream>

namespace boost {
namespace cts {

typedef ::std::basic_istream<bchar, bchar_traits > bistream;
typedef ::std::basic_ostream<bchar, bchar_traits> bostream;
typedef ::std::basic_iostream<bchar, bchar_traits> biostream;


#ifdef BOOST_CTS_USE_WIDE_CHARS
	//
	// define input stream
	inline ::std::wistream& bcin(void){return ::std::wcin;}
	// define output stream
	inline ::std::wostream& bcout(void){return ::std::wcout;}
	// define error stream
	inline ::std::wostream& bcerr(void){return ::std::wcerr;}
#else // NOT BOOST_CTS_USE_WIDE_CHARS
	// if narrow characters were picked
//
	// define input stream
	inline ::std::istream& bcin(void){return ::std::cin;}
	// define output stream
	inline ::std::ostream& bcout(void){return ::std::cout;}
	// define error stream
	inline ::std::ostream& bcerr(void){return ::std::cerr;}
#endif // NOT BOOST_CTS_USE_WIDE_CHARS

} // namespace cts
} // namespace boost

#endif //include guard
