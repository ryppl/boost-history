// formatter_thread_id.hpp

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


#ifndef JT28092007_formatter_thread_id_HPP_DEFINED
#define JT28092007_formatter_thread_id_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging { namespace formatter {

/** 
@brief Writes the thread_id to the log

@param convert [optional] In case there needs to be a conversion between std::(w)string and the string that holds your logged message. See convert_format.
For instance, you might use @ref boost::logging::optimize::cache_string_one_str "a cached_string class" (see @ref boost::logging::optimize "optimize namespace").
*/
template<class convert = do_convert_format::prepend> struct thread_id {
    template<class msg_type> void operator()(msg_type & msg) {
        std::basic_ostringstream<char_type> out;
        out << _T("[T")
    #if defined (BOOST_HAS_WINTHREADS)
            << ::GetCurrentThreadId()
    #elif defined (BOOST_HAS_PTHREADS)
            << pthread_self ()
    #elif defined (BOOST_HAS_MPTASKS)
            << MPCurrentTaskID()
    #endif
            << _T("] ");

        convert::write( out.str(), msg );
    }

}}}

#endif

