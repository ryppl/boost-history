/**
 * \file boost/char_type_switch/choice.hpp
 * Definition of pp symbols for choosing of the used character type
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_CHOICE
#define BOOST_CHAR_TYPE_SWITCH_CHOICE

// this is technically not necessary, but .. 
namespace boost {
namespace cts {

#ifndef BOOST_CTS_USE_WIDE_CHARS

#    ifdef UNICODE
#    define BOOST_CTS_USE_WIDE_CHARS
#    endif

#endif

} // namespace cts
} // namespace boost

#endif //include guard
