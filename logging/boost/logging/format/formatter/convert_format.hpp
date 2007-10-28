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
namespace convert {
    typedef boost::logging::char_type char_type;
    typedef std::basic_string<char_type> string_type;

    /**
    Example : write_time
    */
    namespace prepend {
        inline void write(const char_type * src, string_type & dest ) {
            const char_type * end = src;
            for ( ; *end; ++end);
            dest.insert( dest.begin(), src, end);
        }
        inline void write(const string_type & src, string_type & dest) {
            dest.insert( dest.begin(), src.begin(), src.end() );
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.prepend_string(src);
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_several_str<string> & dest) {
            dest.prepend_string(src);
        }
    }

    /** 
    */
    namespace append {
        inline void write(const char_type * src, string_type & dest ) {
            dest += src;
        }
        inline void write(const string_type & src, string_type & dest) {
            dest += src;
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.append_string(src);
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_several_str<string> & dest) {
            dest.append_string(src);
        }
    }

    /** 
    */
    namespace modify {
        inline void write(const char_type * src, string_type & dest ) {
            dest = src;
        }
        inline void write(const string_type & src, string_type & dest) {
            dest = src;
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.set_string(src);
        }
        template<class string> void write(const string_type & src, boost::logging::optimize::cache_string_several_str<string> & dest) {
            dest.set_string(src);
        }
        template<class string> void write(string_type & src, boost::logging::optimize::cache_string_one_str<string> & dest) {
            dest.set_string_swap(src);
        }
    }
}



struct do_convert_format {
    typedef std::basic_string<char_type> string_type;

    struct prepend {
        template<class src_type, class string> static void write(const src_type & src, string & dest) {
            convert::prepend::write(src, dest);
        }
        template<class src_type, class string> static void write(src_type & src, string & dest) {
            convert::prepend::write(src, dest);
        }
    };

    struct append {
        template<class src_type, class string> static void write(const src_type & src, string & dest) {
            convert::append::write(src, dest);
        }
        template<class src_type, class string> static void write(src_type & src, string & dest) {
            convert::append::write(src, dest);
        }
    };

    struct modify {
        template<class src_type, class string> static void write(const src_type & src, string & dest) {
            convert::modify::write(src, dest);
        }
        template<class src_type, class string> static void write(src_type & src, string & dest) {
            convert::modify::write(src, dest);
        }
    };
};



}}}

#endif

