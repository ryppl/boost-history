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

#ifndef JT28092007_format_write_detail_HPP_DEFINED
#error do not include this directly. Include logging/format/format_write.hpp instead
#endif

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
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace logging {

namespace writer {
    template<class msg_type, class base_type> struct on_dedicated_thread ;
    template<class base_type> struct ts_write ;

    /** 
        @brief specify thread-safety of your logger_format_write class
    */
    namespace threading {
        /** @brief not thread-safe */
        struct no_ts {};
        /** @brief thread-safe write. All writes are protected by a lock */
        struct ts_write {};
        /** @brief thread-safe write on a dedicated thread. Very efficient. Formatting & writing to destinations happens on the dedicated thread */
        struct on_dedicated_thread {};
    }
}



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

        template<class thread_safety, class gather_type, class format_write> struct find_writer_with_thread_safety {
#ifdef BOOST_HAS_THREADS
            // use ts_write
            typedef writer::ts_write<format_write> type;
#else
            typedef format_write type;
#endif
        };

        template<class gather_type, class format_write> struct find_writer_with_thread_safety<boost::logging::writer::threading::no_ts,gather_type,format_write> {
            typedef format_write type;
        };

        template<class gather_type, class format_write> struct find_writer_with_thread_safety<boost::logging::writer::threading::ts_write,gather_type,format_write> {
            typedef writer::ts_write<format_write> type;
        };

        template<class gather_type, class format_write> struct find_writer_with_thread_safety<boost::logging::writer::threading::on_dedicated_thread,gather_type,format_write> {
            typedef typename gather_type::param param;
            typedef typename boost::remove_reference<param>::type param_no_ref;
            typedef typename boost::remove_const<param_no_ref>::type msg_type;
    
            typedef writer::on_dedicated_thread<msg_type, format_write> type;
        };
    }



template<
            class format_base_type , 
            class destination_base_type ,
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


/** 
@brief Makes it easier to use a logger with format_write class

You just define your <tt>logger<...> </tt> class like this:

@code
typedef logger_format_write<format_base,destination_base> logger_type;
@endcode

instead of

@code
typedef logger_format_write< 
        gather::ostream_like::return_str<>, 
        writer::format_write<formatter_base,destination_base> > > logger_type;
@endcode

FIXME need to have more template params

@param format_base_type your formatter base class
@param destination_base your destination base class
@param thread_safety
@param gather
*/
template<class format_base, class destination_base, class thread_safety, class gather> 
struct logger_format_write
    : logger< 
            typename use_format_write<format_base, destination_base, gather>::gather_type,
            typename detail::find_writer_with_thread_safety<
                thread_safety,
                typename use_format_write<format_base, destination_base, gather>::gather_type,
                writer::format_write<
                    typename use_format_write<format_base, destination_base, gather>::format_base,
                    typename use_format_write<format_base, destination_base, gather>::destination_base,
                    typename use_format_write<format_base, destination_base, gather>::apply_format_and_write,
                    typename use_format_write<format_base, destination_base, gather>::router_type
                > 
            >::type
    >
{
    typedef logger< 
            typename use_format_write<format_base, destination_base, gather>::gather_type,
            typename detail::find_writer_with_thread_safety<
                thread_safety,
                typename use_format_write<format_base, destination_base, gather>::gather_type,
                writer::format_write<
                    typename use_format_write<format_base, destination_base, gather>::format_base,
                    typename use_format_write<format_base, destination_base, gather>::destination_base,
                    typename use_format_write<format_base, destination_base, gather>::apply_format_and_write,
                    typename use_format_write<format_base, destination_base, gather>::router_type
                > 
            >::type
    >
     logger_base_type;

    logger_format_write() {}
    BOOST_LOGGING_FORWARD_CONSTRUCTOR(logger_format_write, logger_base_type)
};

}}

#endif

