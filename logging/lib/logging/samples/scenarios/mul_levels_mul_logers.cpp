// test_mul_levels_mul_logers.hpp
//
// A test of the Logging library with multiple levels and multiple logging classes (each writing to multiple destinations).

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
    @example mul_levels_mul_logers.cpp

    @copydoc mul_levels_mul_logers 

    @page mul_levels_mul_logers mul_levels_mul_logers.cpp Example


    This usage:
    - You have multiple levels (in this example: debug < info < error)
    - You want to format the message before it's written 
      (in this example: prefix it by index, by time, and append an enter to it)
    - You have several loggers
    - Each logger has several log destinations

    Optimizations:
    - use a cache string (from optimize namespace), in order to make formatting the message faster

    Logs:
    - Error messages go into err.txt file
      - formatting - prefix each message by time, index, and append enter
    - Info output goes to console, and a file called out.txt
      - formatting - prefix each message by "[app]", time, and append enter
    - Debug messages go to the debug output window, and a file called out.txt
      - formatting - prefix each message by "[dbg]", time, and append enter


    Here's how the output will look like:

    The output window:
    @code
    07:52.30 [dbg] this is so cool 1
    07:52.30 [dbg] this is so cool again 2
    @endcode


    The console:
    @code
    07:52.30 [app] hello, world
    07:52.30 [app] good to be back ;) 4
    @endcode


    The out.txt file:
    @code
    07:52.30 [dbg] this is so cool 1
    07:52.30 [dbg] this is so cool again 2
    07:52.30 [app] hello, world
    07:52.30 [app] good to be back ;) 4
    @endcode


    The err.txt file
    @code
    07:52.30 [1] first error 3
    07:52.30 [2] second error 5
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

// Step 3 : If you use levels, Set up a log level holder
level::holder g_log_level; // holds the application log level

// Step 4: declare which loggers you'll use
BOOST_DECLARE_LOG(g_log_err, log_type) // normally these go into a header file ;)
BOOST_DECLARE_LOG(g_log_app, log_type)
BOOST_DECLARE_LOG(g_log_dbg, log_type)

// Step 5: define which loggers you'll use
BOOST_DEFINE_LOG(g_log_err, log_type)
BOOST_DEFINE_LOG(g_log_app, log_type)
BOOST_DEFINE_LOG(g_log_dbg, log_type)

// Step 6: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_dbg, g_log_level, debug ) << "[dbg] "
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_err, g_log_level, error )
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_app, g_log_level, info ) << "[app] "

void test_mul_levels_one_logger() {
    // Step 7: add formatters and destinations
    //         That is, how the message is to be formatted and where should it be written to

    // Err log
    g_log_err->writer().add_formatter( formatter::idx() );
    g_log_err->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_err->writer().add_formatter( formatter::append_enter() );
    g_log_err->writer().add_destination( destination::file("err.txt") );

    destination::file out("out.txt");
    // App log
    g_log_app->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_app->writer().add_formatter( formatter::append_enter() );
    g_log_app->writer().add_destination( out );
    g_log_app->writer().add_destination( destination::cout() );

    // Inf log
    g_log_dbg->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_dbg->writer().add_formatter( formatter::append_enter() );
    g_log_dbg->writer().add_destination( out );
    g_log_dbg->writer().add_destination( destination::dbg_window() );

    // Step 8: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++;
    LDBG_ << "this is so cool again " << i++;
    LERR_ << "first error " << i++;

    std::string hello = "hello", world = "world";
    LAPP_ << hello << ", " << world;

    g_log_level.set_enabled(level::error);
    LDBG_ << "this will not be written anywhere";
    LAPP_ << "this won't be written anywhere either";

    g_log_level.set_enabled(level::info);
    LAPP_ << "good to be back ;) " << i++;
    LERR_ << "second error " << i++;

    // Step 9 : Enjoy!
}



#ifdef SINGLE_TEST

int main() {
    test_mul_levels_one_logger();
}

#endif

// End of file

