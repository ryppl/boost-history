// use_format_write.hpp

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


#ifndef JT28092007_use_format_write_HPP_DEFINED
#define JT28092007_use_format_write_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/format/optimize.hpp>
#include <boost/logging/process_msg/ostream_like.hpp>
#include <boost/logging/detail/manipulator.hpp>
#include <boost/logging/detail/find_gather.hpp>

namespace boost { namespace logging {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // specialize logger for format_write class
    //

    namespace detail {

        template<class string, class formatter_base, class destination_base> struct find_format_write_params {
            typedef typename boost::logging::format_and_write::simple<string> apply_format_and_write ;
            typedef typename msg_route::simple<formatter_base, destination_base> router_type;
        };

        template<class string_type, class formatter_base, class destination_base> 
            struct find_format_write_params< typename boost::logging::optimize::cache_string_several_str<string_type>, formatter_base, destination_base>
        {
            typedef typename boost::logging::optimize::cache_string_several_str<string_type> cache_string;
            typedef typename boost::logging::format_and_write::use_cache<formatter_base, destination_base, cache_string> apply_format_and_write ;
            typedef typename msg_route::with_route<formatter_base, destination_base> router_type;
        };
    }

/** 
@brief Makes it easier to use a logger with format_write class

You just define your <tt>logger<...> </tt> class like this:

@code
typedef logger< use_format_write<format_base,destination_base> logger_type;
@endcode

instead of

@code
typedef logger< process_msg< 
        gather::ostream_like::return_str<>, 
        writer::format_write<formatter_base,destination_base> > > logger_type;
@endcode

FIXME need to have more template params

@param format_base_type your formatter base class
@param destination_base your destination base class
@param thread_safety
@param gather
*/
template<
            class format_base_type , 
            class destination_base_type ,
            class thread_safety ,
            class gather 
    >
struct use_format_write {

    typedef typename use_default<format_base_type, boost::logging::formatter::base<> > ::type format_base;
    typedef typename use_default<destination_base_type, boost::logging::destination::base<> > ::type destination_base;

    typedef typename format_base::raw_param format_param;
    typedef typename detail::find_gather<format_param>::type gather_type;

    typedef typename detail::find_format_write_params<format_param, format_base, destination_base>::apply_format_and_write apply_format_and_write;
    typedef typename detail::find_format_write_params<format_param, format_base, destination_base>::router_type router_type;
};


template<class format_base, class destination_base, class thread_safety, class gather> 
struct logger_format_write
    : logger< 
            typename use_format_write<format_base, destination_base, thread_safety, gather>::gather_type,
            writer::format_write<
                typename use_format_write<format_base, destination_base, thread_safety, gather>::format_base,
                typename use_format_write<format_base, destination_base, thread_safety, gather>::destination_base,
                typename use_format_write<format_base, destination_base, thread_safety, gather>::apply_format_and_write,
                typename use_format_write<format_base, destination_base, thread_safety, gather>::router_type
            > 
    >
{
    typedef logger< 
            typename use_format_write<format_base, destination_base, thread_safety, gather>::gather_type,
            writer::format_write<
                typename use_format_write<format_base, destination_base, thread_safety, gather>::format_base,
                typename use_format_write<format_base, destination_base, thread_safety, gather>::destination_base,
                typename use_format_write<format_base, destination_base, thread_safety, gather>::apply_format_and_write,
                typename use_format_write<format_base, destination_base, thread_safety, gather>::router_type
            > 
    >
     logger_base_type;

    logger_format_write() {}
    BOOST_LOGGING_FORWARD_CONSTRUCTOR(logger_format_write, logger_base_type)
};

}}

#endif

