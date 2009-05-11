/**
 * \file boost/char_type_switch/sstream.hpp
 * Narrow/Wide character type switching for sstreams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_SSTREAM
#define BOOST_CHAR_TYPE_SWITCH_SSTREAM

#include <boost/char_type_switch/string.hpp>
// Needed for ::std cout, cin, cerr, wcin, wcout, wcerr
#include <sstream>

namespace boost {
namespace cts {

typedef ::std::basic_stringstream<bchar, bchar_traits> bstringstream;

} // namespace cts
} // namespace boost

#endif //include guard
