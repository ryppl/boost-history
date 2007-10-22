// mul_loggers_one_filter.cpp
//
// A test of the Logging library with multiple levels, multiple logging classes (each writing to multiple destinations) and one filter.

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


/**
@example mul_loggers_one_filter.cpp

@copydoc mul_loggers_one_filter 

@page mul_loggers_one_filter mul_loggers_one_filter.cpp Example


This usage:
- You have several loggers
- You have one filter, which can be turned on or off
- You want to format the message before it's written 
- Each logger has several log destinations

Optimizations:
- use a cache string (from optimize namespace), in order to make formatting the message faster

Logs:
- Error messages go into err.txt file
  - formatting - prefix each message by time, index, and append enter
- Info output goes to console, and a file called out.txt
  - formatting - prefix each message by time, "[app]", and append enter
- Debug messages go to the debug output window, and the console
  - formatting - prefix each message by "[dbg]", time, and append enter


Here's how the output will look like:

The debug output window:
@code
18:59.24 [dbg] this is so cool 1
18:59.24 [dbg] this is so cool again 2
@endcode


The console:
@code
18:59.24 [dbg] this is so cool 1
18:59.24 [dbg] this is so cool again 2
18:59.24 [app] hello, world
18:59.24 [app] good to be back ;) 4
@endcode


The out.txt file:
@code
18:59.24 [app] hello, world
18:59.24 [app] good to be back ;) 4
@endcode


The err.txt file
@code
18:59.24 [1] first error 3
18:59.24 [2] second error 5
@endcode
*/



#define BOOST_LOGGING_COMPILE_FAST_OFF
#include <boost/logging/logging.hpp>
#include <boost/logging/format.hpp>

using namespace boost::logging;

// Optimize : use a cache string, to make formatting the message faster
typedef optimize::cache_string_one_str<> cache_string;

// Step 1: specify your formatter & destination base classes 
typedef formatter::base< cache_string& > formatter_base;
typedef destination::base< const std::string & > destination_base;

// Step 2 : Define your logging class(es)
typedef logger< use_format_write<formatter_base,destination_base> > log_type;

// Step 3 : Set up a filter
filter::no_ts g_log_filter ; 

// Step 4: declare which loggers you'll use
BOOST_DECLARE_LOG(g_log_err, log_type) // normally these go into a header file ;)
BOOST_DECLARE_LOG(g_log_app, log_type)
BOOST_DECLARE_LOG(g_log_dbg, log_type)

// Step 5: define which loggers you'll use
BOOST_DEFINE_LOG(g_log_err, log_type)
BOOST_DEFINE_LOG(g_log_app, log_type)
BOOST_DEFINE_LOG(g_log_dbg, log_type)

// Step 6: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_FILTER(g_log_dbg, g_log_filter.is_enabled() ) << "[dbg] "
#define LERR_ BOOST_LOG_USE_LOG_IF_FILTER(g_log_err, g_log_filter.is_enabled() )
#define LAPP_ BOOST_LOG_USE_LOG_IF_FILTER(g_log_app, g_log_filter.is_enabled() ) << "[app] "

void mul_logger_one_filter_example() {
    // Step 7: add formatters and destinations
    //         That is, how the message is to be formatted and where should it be written to

    // Err log
    g_log_err->writer().add_formatter( formatter::idx() );
    g_log_err->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_err->writer().add_formatter( formatter::append_enter() );
    g_log_err->writer().add_destination( destination::file("err.txt") );

    // App log
    g_log_app->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_app->writer().add_formatter( formatter::append_enter() );
    g_log_app->writer().add_destination( destination::file("out.txt") );
    g_log_app->writer().add_destination( destination::cout() );

    // Debug log
    g_log_dbg->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_dbg->writer().add_formatter( formatter::append_enter() );
    g_log_dbg->writer().add_destination( destination::dbg_window() );
    g_log_dbg->writer().add_destination( destination::cout() );

    // Step 8: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++;
    LDBG_ << "this is so cool again " << i++;
    LERR_ << "first error " << i++;

    std::string hello = "hello", world = "world";
    LAPP_ << hello << ", " << world;

    g_log_filter.set_enabled(false);
    LDBG_ << "this will not be written anywhere";
    LAPP_ << "this won't be written anywhere either";
    LERR_ << "this error is not logged " << i++;

    g_log_filter.set_enabled(true);
    LAPP_ << "good to be back ;) " << i++;
    LERR_ << "second error " << i++;

    // Step 9 : Enjoy!
}



#ifdef SINGLE_TEST

int main() {
    mul_logger_one_filter_example();
}

#endif

// End of file

