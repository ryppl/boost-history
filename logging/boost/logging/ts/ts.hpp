// defs.hpp

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

#ifndef JT_BOOST_LOG_TS_HPP
#define JT_BOOST_LOG_TS_HPP


#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#ifdef BOOST_HAS_THREADS
    
    #ifdef BOOST_LOG_USE_BOOST_THREADS
        #include <boost/logging/ts/ts_boost.hpp>
    #else
        #ifdef BOOST_WINDOWS
        #include <boost/logging/ts/ts_win32.hpp>
        #else
        #include <boost/logging/ts/ts_posix.hpp>
        #endif
    #endif

#else
    // no threads
    #include <boost/log/ts/ts_none.hpp>
#endif



namespace boost { namespace logging { namespace threading {

#ifdef BOOST_HAS_THREADS
    
    #ifdef BOOST_LOG_USE_BOOST_THREADS
        typedef mutex_boost mutex;
    #else
        #ifdef BOOST_WINDOWS
        typedef mutex_win32 mutex;
        #else
        typedef mutex_posix mutex;
        #endif
    #endif

#else
    // no threads
    typedef no_mutex mutex;
#endif


typedef mutex::scoped_lock scoped_lock;

}}}

#endif

