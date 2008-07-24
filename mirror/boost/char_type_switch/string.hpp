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


#include <boost/char_type_switch/char.hpp>
//
// Needed for ::std::char_traits / ::std::basic_string
#include <string>

namespace boost {
namespace cts {

// define char traits
typedef ::std::char_traits<bchar> bchar_traits;
// define string class type
typedef ::std::basic_string<bchar> bstring;


} // namespace cts
} // namespace boost

#endif //include guard


