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

// IMPORTANT : the JT28092007_macros_HPP_DEFINED needs to remain constant - don't change the macro name!
#ifndef JT28092007_macros_HPP_DEFINED
#define JT28092007_macros_HPP_DEFINED

/* 
    VERY IMPORTANT: 
    Not using #pragma once
    We might need to re-include this file, when defining the logs
*/

#include <boost/logging/detail/fwd.hpp>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <boost/logging/detail/log_keeper.hpp>

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

    don't want compile fast? then log.h will look easier; but - are you sure you don't want to turn compile fast off?

    Macros 
    - BOOST_LOG_COMPILE_FAST_ON
    - BOOST_LOG_COMPILE_FAST_OFF
    - BOOST_LOG_COMPILE_FAST

*/

#ifdef BOOST_LOG_COMPILE_FAST_ON
#define BOOST_LOG_COMPILE_FAST
#elif defined(BOOST_LOG_COMPILE_FAST_OFF)
#undef BOOST_LOG_COMPILE_FAST
#else
// by default, turned on
#define BOOST_LOG_COMPILE_FAST
#endif










//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Defining filter Macros 

#ifdef BOOST_LOG_COMPILE_FAST
// ****** Fast compile ******

#define BOOST_DECLARE_LOG(name,type) \
    type& name ## _boost_log_impl_(); \
    ::boost::logging::detail::fast_compile_with_default_gather<>::log_type & name ## _boost_log_impl_light_(); \
    extern boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 

#define BOOST_DEFINE_LOG(name,type)  type& name ## _boost_log_impl_() \
    { static type i; return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; \
    ::boost::logging::detail::fast_compile_with_default_gather<>::log_type & name ## _boost_log_impl_light_()  \
    { typedef ::boost::logging::detail::fast_compile_with_default_gather<>::gather_msg gather_msg; \
    typedef type::process_msg_type process_msg_type; \
    typedef process_msg_type::write_type write_msg; \
    static ::boost::logging::detail::process_msg_with_ptr< gather_msg, write_msg > p( name ## _boost_log_impl_() ); \
    static ::boost::logging::detail::fast_compile_with_default_gather<>::log_type i ( &p ); \
    return i; }


#else
// don't compile fast

#define BOOST_DECLARE_LOG(name,type) type& name ## _boost_log_impl_(); extern boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 
#define BOOST_DEFINE_LOG(name,type)  type& name ## _boost_log_impl_() \
    { static type i; return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 

/** 
    Advanced
*/
#define BOOST_DECLARE_LOG_WITH_GATHER(name,type,gather_type) BOOST_DECLARE_LOG(name,type)

#endif





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filter Macros 

#define BOOST_DECLARE_LOG_FILTER_NO_NAMESPACE_PREFIX(name,type) type& name ## _boost_log_filter_impl_(); extern boost::logging::detail::log_filter_keeper<type, name ## _boost_log_filter_impl_ > name; 
#define BOOST_DEFINE_LOG_FILTER_NO_NAMESPACE_PREFIX(name,type)  type& name ## _boost_log_filter_impl_() \
    { static type i; return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_filter_impl_() ); } \
    boost::logging::detail::log_filter_keeper<type, name ## _boost_log_filter_impl_ > name; 


/** 
@note
    It is assumed that @c type is a filter class from the @c boost::logging namespace.
    In case you're creating your own filter class, make sure to have it in the boost::logging namespace. \n
    Or, you can use the BOOST_DECLARE_LOG_FILTER_NO_NAMESPACE_PREFIX macro instead
*/
#define BOOST_DECLARE_LOG_FILTER(name,type) BOOST_DECLARE_LOG_FILTER_NO_NAMESPACE_PREFIX(name, ::boost::logging:: type)

/** 
@note
    It is assumed that @c type is a filter class from the @c boost::logging namespace.
    In case you're creating your own filter class, make sure to have it in the boost::logging namespace. \n
    Or, you can use the BOOST_DEFINE_LOG_FILTER_NO_NAMESPACE_PREFIX macro instead.
*/
#define BOOST_DEFINE_LOG_FILTER(name,type) BOOST_DEFINE_LOG_FILTER_NO_NAMESPACE_PREFIX(name, ::boost::logging:: type)








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log Macros

// FIXME always need to specify the function to call
// when compiling fast, I'll use l ## _boost_log_impl() !!!

#define BOOST_LOG_USE_LOG(l, do_func, is_log_enabled) if ( !(is_log_enabled) ) ; else l -> do_func

#define BOOST_LOG_USE_LOG_IF_LEVEL(l, holder, the_level) BOOST_LOG_USE_LOG(l, read_msg().gather().out(), holder->is_enabled(::boost::logging::level:: the_level) )

#define BOOST_LOG_USE_LOG_IF_FILTER(l, the_filter) BOOST_LOG_USE_LOG(l, read_msg().gather().out(), the_filter)

#define BOOST_LOG_USE_SIMPLE_LOG_IF_FILTER(l, is_log_enabled) if ( !(is_log_enabled) ) ; else l ->operator() 





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Format and Destination Macros

/**

@note
    When using BOOST_LOG_FORMAT_MSG or BOOST_LOG_DESTINATION_MSG, you must not be within any namespace scope.

    This is because when using this macro, as @c msg_class, you can specify any of your class, or
    something residing in @c boost::logging namespace.
*/
#define BOOST_LOG_FORMAT_MSG(msg_class) \
    namespace boost { namespace logging { \
    template<> struct formatter::msg_type<override> { typedef msg_class & type; }; \
    }}

/**

@note
    When using BOOST_LOG_FORMAT_MSG or BOOST_LOG_DESTINATION_MSG, you must not be within any namespace scope.

    This is because when using this macro, as @c msg_class, you can specify any of your class, or
    something residing in @c boost::logging namespace.
*/
#define BOOST_LOG_DESTINATION_MSG(msg_class) \
    namespace boost { namespace logging { \
    template<> struct destination::msg_type<override> { typedef const msg_class & type; }; \
    }}



}}

#endif

