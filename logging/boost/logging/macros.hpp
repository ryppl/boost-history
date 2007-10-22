// macros.hpp

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


#ifndef JT28092007_macros_HPP_DEFINED
#define JT28092007_macros_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <time.h>
#include <stdlib.h>

namespace boost { namespace logging {

/** 
    @page macros Macros - how, what for?

    When dealing with logs, you will most likely want to use macros: simply to write less.
    To be efficient, you usually want to write to a log only if it's enabled.

    Either you always write: <tt> if ( g_log) g_log .... </tt>, or, you create macros:
    
    @code
    #define L_ if ( g_log) g_log .... 
    @endcode

    FIXME to be continued :) explain about the fact that you can create your own macros, depending on what *you* want

    Explain why the if ; else strategy: so that if withing if (x) LOG_ ... ; else blabla - still ok
        #define L_ if ( g_single_log) ; else g_single_log->read_msg().gather().msg()



    Macros 
    - BOOST_LOGGING_COMPILE_FAST_ON
    - BOOST_LOGGING_COMPILE_FAST_OFF
    - BOOST_LOGGING_COMPILE_FAST
*/

#ifdef BOOST_LOGGING_COMPILE_FAST_ON
#define BOOST_LOGGING_COMPILE_FAST
#elif defined(BOOST_LOGGING_COMPILE_FAST_OFF)
#undef BOOST_LOGGING_COMPILE_FAST
#else
// by default, turned on
#define BOOST_LOGGING_COMPILE_FAST
#endif

namespace detail {
    /* 
        Note that BOOST_DECLARE_LOG & BOOST_DEFINE_LOG define a function,
        so that we don't run into the problem of using an object before it's initialized.

        However, client code doesn't need to be aware of that.
        So, for instance, clients will say:

        typedef logger<...> app_log;
        BOOST_DEFINE_LOG(g_l,app_log);

        g_l->writer().add_formatter( formatter::idx() );
    */
    template<class type, type& (*func)() > struct log_keeper {
        typedef typename type::process_msg_type process_msg_type ;

        const process_msg_type* operator->() const  { return func().operator->(); }
        process_msg_type* operator->()              { return func().operator->(); }
    };

    struct fake_using_log {
        template<class type> fake_using_log( type & log) {
            long long ignore = reinterpret_cast<long long>(&log);
            // we need to force the compiler to force creation of the log
            if ( time(0) < 0)
                if ( time(0) < ignore)
                    exit(0);
        }
    };
}

#ifdef BOOST_LOGGING_COMPILE_FAST

// FIXME need to reimplement them when using compile_fast
#define BOOST_DECLARE_LOG(name,type) type& name ## _boost_log_impl_(); extern boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 
#define BOOST_DEFINE_LOG(name,type)  type& name ## _boost_log_impl_() \
    { static type i; return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 

#else

// FIXME need to reimplement them when using compile_fast
#define BOOST_DECLARE_LOG(name,type) type& name ## _boost_log_impl_(); extern boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 
#define BOOST_DEFINE_LOG(name,type)  type& name ## _boost_log_impl_() \
    { static type i; return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 

#endif





#define BOOST_LOG_USE_LOG(l, do_func, is_log_enabled) if ( !(is_log_enabled) ) ; else l -> do_func

#define BOOST_LOG_USE_LOG_IF_LEVEL(l, holder, the_level) BOOST_LOG_USE_LOG(l, read_msg().gather().out(), holder.is_enabled(::boost::logging::level:: the_level) )

#define BOOST_LOG_USE_LOG_IF_FILTER(l, the_filter) BOOST_LOG_USE_LOG(l, read_msg().gather().out(), the_filter)

#define BOOST_LOG_USE_SIMPLE_LOG_IF_FILTER(l, is_log_enabled) if ( !(is_log_enabled) ) ; else l ->operator() 












}}

#endif

