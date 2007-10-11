// fwd.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
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

    namespace writer {};



    /** 
        just in case you're doing a typo - "write" instead of "writer"
    */
    namespace write = writer;
}}


#endif

