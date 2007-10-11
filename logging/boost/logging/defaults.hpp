// defaults.hpp

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

#ifndef JT28092007_defaults_HPP_DEFINED
#define JT28092007_defaults_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/ts/ts.hpp>
#include <boost/logging/ts/ts_resource.hpp>

#include <string>


namespace boost { namespace logging {

/** 
@page override_defaults Defaults, and overriding them.

The logging lib has a few default types, used throughout the lib. They are:
- @c char_type - the char type used throught the lib; by default, it's @c char
- @c hold_string_type - the type used to hold a string; by default, it's @c std::string
- @c filter_type - the default filter; by default, it's filter::no_ts
- @c lock_resource - used to lock resources for access. See locker namespace.

They are all present in @c default_types structure.

If you want to override any of the above, you should do the following:
- before including anything from Boost Logging Library, <tt>\#include <boost/logging/defaults.hpp> </tt>
- override the types
- do <tt>\#include <boost/logging/logging.hpp> </tt>
      
Example:

@code
    #include <boost/logging/defaults.hpp>

    namespace boost { namespace logging {
        struct types<override> {
            // define your types
            typedef wchar_t char_type;
            // etc.
        };
    }}

    #include <boost/logging/logging.hpp>
@endcode


*/



    struct override {};

    // forward defines

    namespace filter {
        struct no_ts;
    };

    struct default_types {
        typedef char char_type;
        // this is the type we use to hold a string, internally
        typedef std::string hold_string_type;

        // default filter type
        typedef filter::no_ts filter_type;

        struct lock_resource {
            template<class lock_type> struct finder {
                typedef typename locker::ts_resource<lock_type> type;
            };
        };


    };

    // FIXME we need defaults for different scenarios!
    template<class T> struct types : default_types {
    };
}}

#endif

