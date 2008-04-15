/**
 * \file boost/char_width_switch/choice.hpp
 * Definition of pp symbols for choosing of the used character type
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_WIDTH_SWITCH_CHOICE
#define BOOST_CHAR_WIDTH_SWITCH_CHOICE

namespace boost {

// TODO: there is a lot of work to do here ;)
#ifndef BOOST_USE_WIDE_CHARS

#    ifdef UNICODE
#    define BOOST_USE_WIDE_CHARS
#    endif

#endif

} // namespace boost

#endif //include guard
