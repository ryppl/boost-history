// macros.hpp

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


#ifndef JT28092007_macros_HPP_DEFINED
#define JT28092007_macros_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {

/** 
    @page macros Macros - how, what for?

    When dealing with logs, you will most likely want to use macros: simply to write less.
    To be efficient, you usually want to write to a log only if it's enabled.

    Either you always write: <tt> if ( g_log) g_log .... </tt>, or, you create macros:
    
    @code
    #define L_ if ( g_log) g_log .... 
    @endcode

    FIXME to be continued :) explain about the fact that you can create your own macros, depending on what *you* want

    Explain why the if ; else strategy: so that if withing if (x) LOG_ ... ; else blabla - still ok
        #define L_ if ( g_single_log) ; else g_single_log->read_msg().gather().msg()

*/


}}

#endif

