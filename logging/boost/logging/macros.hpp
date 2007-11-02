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

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <boost/logging/detail/log_keeper.hpp>

namespace boost { namespace logging {

/** 
    @page macros Macros - how, what for?

    The need for macros - creating the object before main.

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

    @section macros_gathering

    FIXME

    Macros 
    - BOOST_LOG_COMPILE_FAST_ON
    - BOOST_LOG_COMPILE_FAST_OFF
    - BOOST_LOG_COMPILE_FAST

BOOST_LOG_TSS_USE_INTERNAL
BOOST_LOG_TSS_USE_BOOST
BOOST_LOG_TSS_USE_CUSTOM
BOOST_LOG_NO_TSS

*/

#ifdef BOOST_LOG_COMPILE_FAST_ON
#define BOOST_LOG_COMPILE_FAST
#elif defined(BOOST_LOG_COMPILE_FAST_OFF)
#undef BOOST_LOG_COMPILE_FAST
#else
// by default, turned on
#define BOOST_LOG_COMPILE_FAST
#endif




#if !defined(BOOST_LOG_TSS_USE_INTERNAL) && !defined(BOOST_LOG_TSS_USE_BOOST) && !defined(BOOST_LOG_TSS_USE_CUSTOM) && !defined(BOOST_LOG_NO_TSS)
// use has not specified what TSS strategy to use
#define BOOST_LOG_TSS_USE_INTERNAL

#endif




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Defining filter Macros 

#ifdef BOOST_LOG_COMPILE_FAST
// ****** Fast compile ******

#define BOOST_DECLARE_LOG(name,type) \
    type& name ## _boost_log_impl_(); \
    ::boost::logging::detail::fast_compile_with_default_gather<>::log_type & name ## _boost_log_impl_light_(); \
    extern boost::logging::detail::log_keeper<type, name ## _boost_log_impl_, ::boost::logging::detail::fast_compile_with_default_gather<>::log_type, name ## _boost_log_impl_light_ > name; 

#define BOOST_DEFINE_LOG(name,type)  type& name ## _boost_log_impl_() \
    { static type i; return i; } \
    ::boost::logging::detail::fast_compile_with_default_gather<>::log_type & name ## _boost_log_impl_light_()  \
    { typedef ::boost::logging::detail::fast_compile_with_default_gather<>::gather_msg gather_msg; \
    typedef type::write_type write_msg; \
    static ::boost::logging::implement_default_logger< gather_msg, write_msg* > p( &(name ## _boost_log_impl_().writer()) ); \
    return p; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_, ::boost::logging::detail::fast_compile_with_default_gather<>::log_type, name ## _boost_log_impl_light_ > name; 

#define BOOST_DEFINE_LOG_WITH_ARGS(name,type, args)  type& name ## _boost_log_impl_() \
    { static type i ( args ); return i; } \
    ::boost::logging::detail::fast_compile_with_default_gather<>::log_type & name ## _boost_log_impl_light_()  \
    { typedef ::boost::logging::detail::fast_compile_with_default_gather<>::gather_msg gather_msg; \
    typedef type::write_type write_msg; \
    static ::boost::logging::implement_default_logger< gather_msg, write_msg* > p( &(name ## _boost_log_impl_().writer()) ); \
    return p; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_, ::boost::logging::detail::fast_compile_with_default_gather<>::log_type, name ## _boost_log_impl_light_ > name; 



#else
// don't compile fast
#define BOOST_DECLARE_LOG(name,type) type& name ## _boost_log_impl_(); extern boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 
#define BOOST_DEFINE_LOG(name,type)  type& name ## _boost_log_impl_() \
    { static type i; return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 

#define BOOST_DEFINE_LOG_WITH_ARGS(name,type, args)  type& name ## _boost_log_impl_() \
    { static type i ( args); return i; } \
    namespace { boost::logging::detail::fake_using_log ensure_log_is_created_before_main ## name ( name ## _boost_log_impl_() ); } \
    boost::logging::detail::log_keeper<type, name ## _boost_log_impl_ > name; 


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


#define BOOST_LOG_USE_LOG(l, do_func, is_log_enabled) if ( !(is_log_enabled) ) ; else l .base()-> do_func

#define BOOST_LOG_USE_LOG_IF_LEVEL(l, holder, the_level) BOOST_LOG_USE_LOG(l, read_msg().gather().out(), holder->is_enabled(::boost::logging::level:: the_level) )

#define BOOST_LOG_USE_LOG_IF_FILTER(l, the_filter) BOOST_LOG_USE_LOG(l, read_msg().gather().out(), the_filter)

#define BOOST_LOG_USE_SIMPLE_LOG_IF_FILTER(l, is_log_enabled) if ( !(is_log_enabled) ) ; else l .base() ->read_msg().gather().out 





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Format and Destination Macros

/** @section BOOST_LOG_FORMAT_MSG BOOST_LOG_FORMAT_MSG

@note
    When using BOOST_LOG_FORMAT_MSG or BOOST_LOG_DESTINATION_MSG, you must not be within any namespace scope.

    This is because when using this macro, as @c msg_class, you can specify any of your class, or
    something residing in @c boost::logging namespace.
*/
#define BOOST_LOG_FORMAT_MSG(msg_class) \
    namespace boost { namespace logging { namespace formatter { \
    template<> struct msg_type<override> { typedef msg_class & type; }; \
    }}}

/**

@note
    When using BOOST_LOG_FORMAT_MSG or BOOST_LOG_DESTINATION_MSG, you must not be within any namespace scope.

    This is because when using this macro, as @c msg_class, you can specify any of your class, or
    something residing in @c boost::logging namespace.
*/
#define BOOST_LOG_DESTINATION_MSG(msg_class) \
    namespace boost { namespace logging { namespace destination { \
    template<> struct msg_type<override> { typedef const msg_class & type; }; \
    }}}



}}

#endif

