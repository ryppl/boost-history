// Template.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#ifndef JT28092007_TEMPLATE_HPP_DEFINED
#define JT28092007_TEMPLATE_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging { namespace destination {

template<class t> struct into {};

/** 
@brief Allows writing messages to destinations

It has 2 function overloads:
- write(message, output) - writes the given message, to the given output
- do_convert(message, into<other_type>() );

FIXME
*/
namespace convert {
    template<class obj, class char_traits, class char_type> void write(const obj & m, std::basic_iostream<char_type, char_traits> & out) {
        out << m;
    }

    template<class char_traits, class char_type> void write(const char_type* m, std::basic_iostream<char_type, char_traits> & out) {
        out << m;
    }

    inline const char_type * do_convert(const char_type * c, const into<const char_type*> &) { return c; }
    inline const char_type * do_convert(const std::basic_string<char_type> & s, const into<const char_type* > &) { return s.c_str(); }
}

struct do_convert_destination {
    template<class msg, class dest> void write(const msg & m, dest & d) {
        convert::write(msg, dest);
    }

    template<class msg, class dest> dest do_convert(const msg & m, const into<dest> &) {
        convert::do_convert(msg, into<dest>() );
    }

};

}}}

#endif

