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

#if defined(_MSC_VER) 
#pragma warning ( disable : 4355)
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
    typedef detail::flag<file_settings> flag;

    file_settings() 
        : flush_each_time(this, true)
        , initial_overwrite(this, false)
        , do_append(this, true)
        , extra_flags(this, 0) {}


    /// if true (default), flushes after each write
    flag::t<bool> flush_each_time;
    // if true it initially overwrites the file; default = false
    flag::t<bool> initial_overwrite;
    // if true (default), opens the file for appending
    flag::t<bool> do_append;

    /// just in case you have some extra flags to pass, when opening the file
    flag::t<std::ios_base::open_mode> extra_flags;
};

namespace detail {
    std::ios_base::open_mode open_flags(file_settings fs) {
        std::ios_base::open_mode flags = std::ios_base::out | fs.extra_flags() ;
        if ( fs.do_append() )
            flags |= std::ios_base::app;
        if ( !fs.initial_overwrite() )
            flags |= std::ios_base::in;

        return flags;
    }

    struct file_info {
        file_info(const std::string& name, file_settings settings) : name(name), out(name.c_str(), open_flags(settings) ), settings(settings) {}

        std::string name;
        std::basic_ofstream<char_type> out;
        file_settings settings;
    };
}

/** 
    @brief Writes the string to a file
*/
template<class convert_dest = do_convert_destination > struct file_t : is_generic, non_const_context<detail::file_info> {
    /** 
        @brief constructs the file destination

        @param file_name name of the file
        @param set [optional] file settings - see file_settings class
    */
    file_t(const std::string & file_name, file_settings set = file_settings() ) : non_const_context_base(file_name,set) {}
    template<class msg_type> void operator()(const msg_type & msg) const {
        convert_dest::write(msg, non_const_context_base::context().out );
        if ( non_const_context_base::context().settings.flush_each_time() )
            non_const_context_base::context().out.flush();
    }

    bool operator==(const file_t & other) const {
        return non_const_context_base::context().name == other.context().name;
    }
};

/** 
@brief file_t with default values. See file_t
*/
typedef file_t<> file;




}}}

#endif

