// Template.hpp

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


#ifndef JT28092007_TEMPLATE_HPP_DEFINED
#define JT28092007_TEMPLATE_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {


namespace filter {
    template<int> struct use_tss_with_cache ;
    struct no_ts ;
    struct ts ;
    struct use_tss_once_init ;
};

namespace level {
    template<int> struct holder_tss_with_cache ;
    struct holder_tss_once_init ;
    struct holder_ts;
    struct holder_no_ts ;
}


/** 
    @brief Use it when you have a specific scenario, and want the best logger/filter classes for the scenario. Check out scenario::usage 
*/
namespace scenario {

/** 
    @brief Find out the right logger/filter, based on how your application will use the loggers and filters

    First, don't forget to \n <tt>use namespace boost::logging::scenario::usage;</tt>
*/
namespace usage {

    /** @brief Filter usage settings : filter_::change and filter_::level
    */
    namespace filter_ {
        /** @brief When does the filter change? */
        namespace change {
            /** @brief Optimize for %often %change. Does per-thread caching. At a given period, it re-synchronizes. 
                
                This is the default, for a multi-threaded application.

                @param cache_period_secs At what period should we re-syncronize
            */
            template<int cache_period_secs = 5> struct often {};

            /** @brief Set only once, when there's only one thread running - thus, you don't need to worry about thread-syncronizing */
            struct set_once_when_one_thread {};

            /** @brief Set only once, when there could be multiple thread running. 
            
            We automatically implement a strategy to check if the filter/logger has been initialized, and when it's done, we cache
            the result on every thread */
            struct set_once_when_multiple_threads {};

            /** @brief This is always accurate. However, it's the slowest too.

            In case of multiple threads, it always locks the logger/filter before accessing it. 

            Not recommended, you should usually go with another strategy (often, set_once_when_one_thread or set_once_when_multiple_threads)
            */
            struct always_accurate {};

            /** @brief Single threading. It doesn't matter when/how %often the filter/logger changes. 

                This is the default, for a single-threaded application.
            */
            struct single_thread {};

#ifdef BOOST_HAS_THREADS
            typedef often<> default_;
#else
            typedef single_thread default_;
#endif
        };

        /** @brief What's our "level" policy? */
        namespace level {
            /** @brief not using levels (default) */
            struct no_levels {};
            /** @brief using levels */
            struct use_levels {};

            typedef no_levels default_;
        };
    }

    /** @brief Logger %usage settings : logger_::change and logger_::favor
    */
    namespace logger_ {
        /** @brief When does the logger change, that is, how often do you manipulate it? 
        
        Note that using the log does not mean changing it. 
        Manipulation means invoking non-const functions on the logger, like 
        adding/removing formatters/destinations for instance.
        */
        namespace change {
            /** @brief Optimize for often change. Does per-thread caching. At a given period, it re-synchronizes. 
                
                This is the default, for a multi-threaded application.

                @param cache_period_secs At what period should we re-syncronize
            */
            template<int cache_period_secs = 5> struct often {};

            /** @brief Set only once, when there's only one thread running - thus, you don't need to worry about thread-syncronizing */
            struct set_once_when_one_thread {};

            /** @brief Set only once, when there could be multiple thread running. 
            
            We automatically implement a strategy to check if the filter/logger has been initialized, and when it's done, we cache
            the result on every thread */
            struct set_once_when_multiple_threads {};

            /** @brief This is always accurate. However, it's the slowest too.

            In case of multiple threads, it always locks the logger/filter before accessing it. 

            Not recommended, you should usually go with another strategy (often, set_once_when_one_thread or set_once_when_multiple_threads)
            */
            struct always_accurate {};

            /** @brief Single threading. It doesn't matter when/how often the filter/logger changes. 
            
                This is the default, for a single-threaded application.
            */
            struct single_thread {};

#ifdef BOOST_HAS_THREADS
            typedef often<> default_;
#else
            typedef single_thread default_;
#endif
        };

        /** @brief When logging, what should we %favor? */
        namespace favor {
            /** @brief This will favor speed (logging will happen on a dedicated thread). The only problem you could have is if the application crashes.

                In this case, on Windows, the rest of the application will continue, and any non-flushed log message will be flushed.

                On POSIX, this may not be the case.
            */
            struct speed {};

            /** @brief all messages will be logged 
            
            This is the default
            */
            struct correctness {};

            typedef correctness default_;
        };
    }



    namespace detail_find_filter {
        namespace level = ::boost::logging::scenario::usage::filter_::level;
        namespace change = ::boost::logging::scenario::usage::filter_::change;

        //////// use levels

        template<class change_> struct find_filter_use_levels {};
        
        template<int period_ms> struct find_filter_use_levels< change::often<period_ms> > {
            typedef ::boost::logging::level::holder_tss_with_cache<period_ms> type;
        };

        template<> struct find_filter_use_levels< change::set_once_when_one_thread > {
            typedef ::boost::logging::level::holder_no_ts type;
        };

        template<> struct find_filter_use_levels< change::set_once_when_multiple_threads > {
            typedef ::boost::logging::level::holder_tss_once_init type;
        };

        template<> struct find_filter_use_levels< change::always_accurate > {
            typedef ::boost::logging::level::holder_ts type;
        };

        template<> struct find_filter_use_levels< change::single_thread > {
            typedef ::boost::logging::level::holder_no_ts type;
        };



        //////// no levels

        template<class change_> struct find_filter_no_levels {};
        
        template<int period_ms> struct find_filter_no_levels< change::often<period_ms> > {
            typedef ::boost::logging::filter::use_tss_with_cache<period_ms> type;
        };

        template<> struct find_filter_no_levels< change::set_once_when_one_thread > {
            typedef ::boost::logging::filter::no_ts type;
        };

        template<> struct find_filter_no_levels< change::set_once_when_multiple_threads > {
            typedef ::boost::logging::filter::use_tss_once_init type;
        };

        template<> struct find_filter_no_levels< change::always_accurate > {
            typedef ::boost::logging::filter::ts type;
        };

        template<> struct find_filter_no_levels< change::single_thread > {
            typedef ::boost::logging::filter::no_ts type;
        };



        template<class change_, class level_> struct find_filter {
            // no levels
            typedef typename find_filter_no_levels<change_>::type type;
        };

        template<class change_> struct find_filter<change_, level::use_levels> {
            typedef typename find_filter_use_levels<change_>::type type;
        };

    }


    namespace detail_find_logger {
        namespace favor = ::boost::logging::scenario::usage::logger_::favor;
        namespace change = ::boost::logging::scenario::usage::logger_::change;
    }

    /** 
        @brief Finds a filter class and a logger class, based on your specific scenario

        @param filter_change (optional) How does the %filter change? Any of the classes in the filter_::change namespace
        @param filter_level_ (optional) Does our %filter %use levels? Any of the classes in the filter_::level namespace
        @param logger_change (optional) How does our %logger change? Any of the classes in the logger_::change namespace
        @param logger_favor (optional) What does the %logger favor? Any of the classes in the logger_::favor namespace
    */
    template<
        class filter_change = default_,
        class filter_level = default_, 
        class logger_change = default_,
        class logger_favor = default_>
    struct use {

        typedef typename use_default<filter_change, filter_::change::default_ >::type filter_change_type;
        typedef typename use_default<filter_level, filter_::level::default_ >::type filter_level_type;

        typedef typename detail_find_filter::find_filter<filter_change_type, filter_level_type>::type filter;


//        typedef ... logger;
  //      typedef ... filter;
    };
}

/** 
    @brief Find out the right logger/filter, based on thread-safety of logger(s)/filter(s)

    First, don't forget to <tt>use namespace boost::logging::scenario::ts;</tt>
*/
namespace ts {
}

}

}}

#endif

