// convert_format.hpp

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


#ifndef JT28092007_convert_format_HPP_DEFINED
#define JT28092007_convert_format_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/format/optimize.hpp>

namespace boost { namespace logging { namespace formatter {


/** 
    @brief Allows format convertions - In case you're using a formatter that does not match your string type

    In case you want to use a formatter developed by someone else (for instance, a formatter provided by this lib),
    perhaps you're using another type of string to hold the message - thus, you need to provide a conversion function

    Example:
    FIXME

    --> convert_format::prepend

    explain that you can extend the following - since they're namespaces!!!
    so that you can "inject" your own write function in the convert_format::prepend/orwhatever namespace, and
    then it'll be automatically used!
*/
namespace convert_format {
    typedef boost::logging::char_type char_type;
    typedef std::basic_string<char_type> string_type;

    /**
    Example : write_time
    */
    namespace prepend {
        void write(const string_type & src, string & dest) {
            dest.insert( dest.begin(), src.begin(), src.end() );
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.prepend_string(src);
        }
    }

    /** 
    */
    namespace append {
        void write(const string_type & src, string & dest) {
            dest += src;
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.append_string(src);
        }
    }

    /** 
    */
    namespace modify {
        void write(const string_type & src, string & dest) {
            dest = src;
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.set_string(src);
        }
    }
}



struct do_convert_format {
    struct prepend {
        template<class string> void write(const string_type & src, string & dest) {
            convert_format::prepend::write(src, dest);
        }
    };

    struct append {
        template<class string> void write(const string_type & src, string & dest) {
            convert_format::append::write(src, dest);
        }
    };

    struct modify {
        template<class string> void write(const string_type & src, string & dest) {
            convert_format::modify::write(src, dest);
        }
    };
};



}}}

#endif

