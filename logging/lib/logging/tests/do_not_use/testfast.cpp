// your_scenario.cpp
//
// A test of the Logging library with multiple levels, multiple logging classes (each writing to multiple destinations) and one filter.
// The logger and filter are specified using the boost::logging::scenario namespace

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
@example your_scenario.cpp

@copydoc your_scenario 

@page your_scenario your_scenario.cpp Example


This usage:
- You have several loggers
- You have one filter, which can be turned on or off
- You want to format the message before it's written 
- Each logger has several log destinations
- The logger and filter are specified using the boost::logging::scenario namespace
  - the filter is always accurate (but slow)
  - the filter does not use levels
  - the logger favors speed (on a dedicated thread)
  - the logger is initialized once, when only one thread is running

Optimizations:
- use a cache string (from optimize namespace), in order to make formatting the message faster

Logs:
- Error messages go into err.txt file
  - formatting - prefix each message by time, index, and append enter
- Info output goes to console, and a file called out.txt
  - formatting - prefix each message by time and append enter
- Debug messages go to the debug output window, and the console
  - formatting - prefix each message by time, and append enter


Here's how the output will look like:

The debug output window:
@code
18:59.24 this is so cool 1
18:59.24 this is so cool again 2
@endcode


The console:
@code
18:59.24 this is so cool 1
18:59.24 this is so cool again 2
18:59.24 hello, world
18:59.24 good to be back ;) 4
@endcode


The out.txt file:
@code
18:59.24 hello, world
18:59.24 good to be back ;) 4
@endcode


The err.txt file
@code
18:59.24 [1] first error 3
18:59.24 [2] second error 5
@endcode
*/



#define BOOST_LOG_COMPILE_FAST_OFF
#include <boost/logging/format_fwd.hpp>

// Step 1: Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> )

#include <boost/logging/format_ts.hpp>
#include <boost/thread/xtime.hpp>
using namespace boost::logging;

// Step 3 : Specify your logging class(es)
using namespace boost::logging::scenario::usage;
typedef use<
        //  the filter is always accurate (but slow)
        filter_::change::always_accurate, 
        //  filter does not use levels
        filter_::level::no_levels, 
        // the logger is initialized once, when only one thread is running
        logger_::change::single_thread 
        // the logger favors speed (on a dedicated thread)
        > finder;

// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_filter, finder::filter ) 
BOOST_DECLARE_LOG(g_log_err, finder::logger ) 
BOOST_DECLARE_LOG(g_log_app, finder::logger )
BOOST_DECLARE_LOG(g_log_dbg, finder::logger )

// Step 5: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_FILTER(g_log_dbg, g_log_filter->is_enabled() ) 
#define LERR_ BOOST_LOG_USE_LOG_IF_FILTER(g_log_err, g_log_filter->is_enabled() )
#define LAPP_ BOOST_LOG_USE_LOG_IF_FILTER(g_log_app, g_log_filter->is_enabled() ) 

// Step 6: Define the filters and loggers you'll use (usually in a source file)
BOOST_DEFINE_LOG_FILTER(g_log_filter, finder::filter ) 
BOOST_DEFINE_LOG(g_log_err, finder::logger )
BOOST_DEFINE_LOG(g_log_app, finder::logger )
BOOST_DEFINE_LOG(g_log_dbg, finder::logger )

void do_sleep(int ms) {
    using namespace boost;
    xtime next;
    xtime_get( &next, TIME_UTC);
    next.nsec += (ms % 1000) * 1000000;

    int nano_per_sec = 1000000000;
    next.sec += next.nsec / nano_per_sec;
    next.sec += ms / 1000;
    next.nsec %= nano_per_sec;
    thread::sleep( next);
}

void your_scenario_example() {

    // Step 7: add formatters and destinations
    //         That is, how the message is to be formatted and where should it be written to

    // Err log
    g_log_err->writer().add_formatter( formatter::idx() );
    g_log_err->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_err->writer().add_formatter( formatter::append_newline() );
    g_log_err->writer().add_destination( destination::file("err.txt") );

    // App log
    g_log_app->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_app->writer().add_formatter( formatter::append_newline_if_needed() );
//    g_log_app->writer().add_destination( destination::file("out.txt", destination::file_settings().initial_overwrite(true) ) );
    g_log_app->writer().add_destination( destination::file("out.txt") );
    g_log_app->writer().add_destination( destination::cout() );

    // Debug log
    g_log_dbg->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_dbg->writer().add_formatter( formatter::append_newline_if_needed() );
    g_log_dbg->writer().add_destination( destination::dbg_window() );
    g_log_dbg->writer().add_destination( destination::cout() );

    // Step 8: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++ << "\n";
    LDBG_ << "this is so cool again " << i++;
    LERR_ << "first error " << i++;

    std::string hello = "hello", world = "world";
    LAPP_ << hello << ", " << world;

    g_log_filter->set_enabled(false);
    LDBG_ << "this will not be written anywhere";
    LAPP_ << "this won't be written anywhere either";
    LERR_ << "this error is not logged " << i++;

    g_log_filter->set_enabled(true);
    LAPP_ << "good to be back ;) " << i++;
    LERR_ << "second error " << i++;

    // Step 9 : Enjoy!

    // just so that we can see the output to the console as well (the messages are written no a different thread)...
    do_sleep(1000);
}



#ifdef SINGLE_TEST

int main() {
    your_scenario_example();
}

#endif

// End of file

