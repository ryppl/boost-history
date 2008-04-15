/**
 * \file boost/char_width_switch/iostream.hpp
 * Narrow/Wide character type switching for iostreams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_WIDTH_SWITCH_IOSTREAM
#define BOOST_CHAR_WIDTH_SWITCH_IOSTREAM

#include <boost/char_type_switch/choice.hpp>
// Needed for ::std cout, cin, cerr, wcin, wcout, wcerr
#include <iostream>

namespace boost {

#ifdef BOOST_USE_WIDE_CHARS
	// If wide characters were chosen
	//
	// define input stream
	::std::wistream& bcin   = ::std::wcin;
	// define output stream
	::std::wostream& bcout  = ::std::wcout;
	// define error stream
	::std::wostream& bcerr  = ::std::wcerr;
#else // NOT BOOST_USE_WIDE_CHARS
	// if narrow characters were chosen
	//
	// define input stream
	::std::istream& bcin   = ::std::cin;
	// define output stream
	::std::ostream& bcout  = ::std::cout;
	// define error stream
	::std::ostream& bcerr  = ::std::cerr;
#endif // NOT BOOST_USE_WIDE_CHARS

} // namespace boost

#endif //include guard
