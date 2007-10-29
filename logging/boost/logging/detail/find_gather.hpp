// find_gather.hpp

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


#ifndef JT28092007_find_gather_HPP_DEFINED
#define JT28092007_find_gather_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // specialize logger for format_write class
    //

    namespace gather { namespace ostream_like {
        template<class> struct return_str ;
        template<class, class> struct return_cache_str ;
    }}

    namespace optimize {
        template<class> struct cache_string_one_str ;
        template<class, class> struct cache_string_several_str ;
    }

    namespace detail {
        template<class param> struct find_gather {};
        template<> struct find_gather< std::basic_string<char_type> > { typedef gather::ostream_like::return_str< std::basic_ostringstream<char_type> > type ; };

        template< class string_type> 
        struct find_gather< boost::logging::optimize::cache_string_one_str<string_type> > { 
            typedef gather::ostream_like::return_cache_str< boost::logging::optimize::cache_string_one_str<string_type>, std::basic_ostringstream<char_type> > type;
        };

        template< class string_type> 
        struct find_gather< boost::logging::optimize::cache_string_several_str<string_type,void*> > { 
            typedef gather::ostream_like::return_cache_str< boost::logging::optimize::cache_string_several_str<string_type,void*>, std::basic_ostringstream<char_type> > type;
        };

    }

}}

#endif

