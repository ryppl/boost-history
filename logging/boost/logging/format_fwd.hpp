// format_fwd.hpp

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



#ifndef JT28092007_format_fwd_HPP_DEFINED
#define JT28092007_format_fwd_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/logging.hpp>
#include <boost/logging/format/optimize.hpp>

namespace boost { namespace logging {

/** 
@file boost/logging/format_fwd.hpp

Include this file when you're using @ref manipulator "formatters and destinations",
and you want to declare the logger classes, in a header file
(using BOOST_DECLARE_LOG)

Example:

@code
#ifndef LOG_H_header
#define LOG_H_header

#include <boost/logging/logging.hpp>
#include <boost/logging/format/optimize.hpp>

BOOST_LOG_FORMAT_MSG( boost::logging::optimize::cache_string_one_str<> ) 

#if defined(BOOST_LOG_DEFINE_LOGS)
#include <boost/logging/format.hpp>

typedef logger< use_format_write< > > log_type;
#endif

BOOST_DECLARE_LOG(g_l, log_type)
BOOST_DECLARE_LOG_FILTER(g_l_filter, level::holder)

#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, debug ) << "[dbg] "
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, error ) << "[ERR] "
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, info ) << "[app] "

void init_logs();

#endif
@endcode
*/

}}

#endif

