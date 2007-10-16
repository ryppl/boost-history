// destination_file.hpp

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


#ifndef JT28092007_destination_file_HPP_DEFINED
#define JT28092007_destination_file_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/manipulator.hpp>
#include <boost/logging/format/destination/convert_destination.hpp>
#include <fstream>

namespace boost { namespace logging { namespace destination {

/** 
    @brief settings for when constructing a file class
*/
struct file_settings {
    file_settings() : m_flush_each_time(true), m_initial_overwrite(false), m_do_append(true) {}

    file_settings & flush_each_time() { m_flush_each_time = true; return *this; }
    file_settings & initial_overwrite() { m_initial_overwrite = true; return *this; }
    file_settings & no_append() { m_do_append = false; return *this; }

    file_settings & no_flush_each_time() { m_flush_each_time = false; return *this; }
    file_settings & no_initial_overwrite() { m_initial_overwrite = false; return *this; }
    file_settings & append() { m_do_append = true; return *this; }

    file_settings & extra_open_flags(std::ios_base::open_mode extra_flags) { m_extra_flags = extra_flags; return *this; }

    /// if true (default), flushes after each write
    bool m_flush_each_time;
    // if true it initially overwrites the file; default = false
    bool m_initial_overwrite;
    // if true (default), opens the file for appending
    bool m_do_append;

    /// just in case you have some extra flags to pass, when opening the file
    std::ios_base::open_mode m_extra_flags;
};

namespace detail {
    std::ios_base::open_mode open_flags(file_settings fs) {
        std::ios_base::open_mode flags = std::ios_base::out | fs.m_extra_flags ;
        if ( m_do_append)
            flags |= std::ios_base::app;
        if ( !m_initial_overwrite)
            flags |= std::ios_base::in;

        return flags;
    }

    struct file_info {
        file_info(const char_type * name, file_settings settings) : out(name, open_flags(settings) ), settings(settings) {}
        std::basic_ofstream<char_type> out;
        file_settings settings;
    }
}

/** 
    @brief Writes the string to a file
*/
template<class convert_dest = do_convert_destination > struct file : non_const_context<detail::file_info> {
    /** 
        @brief constructs the file destination

        @param file_name name of the file
        @param set [optional] file settings - see file_settings class
    */
    file(const std::string & file_name, file_settings set = file_settings() ) : non_const_context_base(file_name) {}
    template<class msg_type> void operator()(const msg_type & msg) const {
        convert_dest::write(msg, non_const_context_base::context() );
    }
};






}}}

#endif

