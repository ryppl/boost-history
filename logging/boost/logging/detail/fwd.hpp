// fwd.hpp

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


#ifndef JT28092007_fwd_HPP_DEFINED
#define JT28092007_fwd_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/ts/ts.hpp>
#include <boost/logging/ts/ts_resource.hpp>
#include <boost/logging/detail/util.hpp>

#include <boost/logging/defaults.hpp>

// minimize inclusion of STL headers in our headers!!!
#include <string>




/* 
    Important: we define here only the things that are needed by ALL OF THE LIBRARY.
    So be very careful when modifying this file - we don't want any circular dependencies!

    If unsure where to place something, place it logging.hpp!
*/

namespace boost { namespace logging {
    // see our types
    typedef types<override>::char_type char_type;
    typedef types<override>::hold_string_type hold_string_type;
    typedef types<override>::filter_type filter_type;
    typedef types<override>::mutex mutex;

    namespace writer {}



    /** 
        just in case you're doing a typo - "write" instead of "writer"
    */
    namespace write = writer;
}}


#endif

