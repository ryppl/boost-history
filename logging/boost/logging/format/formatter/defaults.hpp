// formatter_defaults.hpp

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


#ifndef JT28092007_formatter_defaults_HPP_DEFINED
#define JT28092007_formatter_defaults_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/manipulator.hpp>
#include <boost/logging/format/formatter/time.hpp>
#include <stdio.h>
#include <time.h>

namespace boost { namespace logging { namespace formatter {


/** 
@brief prefixes each message with an index. 

Example:
@code
L_ << "my message";
L_ << "my 2nd message";
@endcode

This will output something similar to:

@code
[1] my message
[2] my 2nd message
@endcode


@param convert [optional] In case there needs to be a conversion between std::(w)string and the string that holds your logged message. See convert_format.
For instance, you might use @ref boost::logging::optimize::cache_string_one_str "a cached_string class" (see @ref boost::logging::optimize "optimize namespace").
*/
template<class convert = do_convert_format::prepend> struct idx : formatter::non_const_context<int> {
    idx() : non_const_context_base((int)0) {}
    template<class msg_type> void operator()(msg_type & str) const {
        std::basic_ostringstream<char_type> idx;
        idx << _T("[") << ++context() << _T("] ");

        convert::write( idx.str(), str );
    }
};


/** 
@brief Appends an enter

@param convert [optional] In case there needs to be a conversion between std::(w)string and the string that holds your logged message. See convert_format.
For instance, you might use @ref boost::logging::optimize::cache_string_one_str "a cached_string class" (see @ref boost::logging::optimize "optimize namespace").
*/
template<class convert = do_convert_format::prepend> struct append_enter {
    template<class msg_type> void operator()(msg_type & str) const {
        convert::write( _T("\n"), str );
    }
};


/** 
@brief Appends an enter, if not already there

@param convert [optional] In case there needs to be a conversion between std::(w)string and the string that holds your logged message. See convert_format.
For instance, you might use @ref boost::logging::optimize::cache_string_one_str "a cached_string class" (see @ref boost::logging::optimize "optimize namespace").
*/
template<class convert = do_convert_format::append> struct append_enter_if_needed {
    template<class msg_type> void operator()(msg_type & str) const {
        bool is_needed = true;
        if ( !str.empty())
            if ( str[ str.size() - 1] == '\n')
                is_needed = false;

        if ( is_needed)
            convert::write( _T("\n"), str );
    }
};


}}}

#endif

