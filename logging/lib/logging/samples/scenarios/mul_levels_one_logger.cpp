// test_mul_levels_one_logger.hpp

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

#define BOOST_LOGGING_COMPILE_FAST_OFF
#include <boost/logging/logging.hpp>
#include <boost/logging/format.hpp>

using namespace boost::logging;

/*
    Common usage:
    - You have multiple levels (in this example: debug < info < error)
    - You want to format the message before it's written 
      (in this example: prefix it by index, by time, and append an enter to it)
    - You have several log destinations
      (in this example: the console, the output window, and a file

*/

// Step 1: define your formatter & destination base classes 
typedef formatter::base< std::string& > formatter_base;
typedef destination::base< const std::string & > destination_base;

// Step 2 : define your logging class(es)
typedef logger< use_format_write<formatter_base,destination_base> > log_type;

// Step 3 : if you use levels, have a log level holder
level::holder g_log_level; // holds the application log level

// Step 4A: declare which loggers you'll use
BOOST_DECLARE_LOG(g_l, log_type) // normally this goes into a header file ;)

// Step 4B: define which loggers you'll use
BOOST_DEFINE_LOG(g_l, log_type)

// Step 5: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, debug )
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, error )
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, info )

void test_mul_levels_one_logger() {
    // Step 6: add formatters and destinations
    //         That is, how the message is to be formatted...
    g_l->writer().add_formatter( formatter::idx() );
    g_l->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_l->writer().add_formatter( formatter::append_enter() );

    //        ... and where should it be written to
    g_l->writer().add_destination( destination::cout() );
    g_l->writer().add_destination( destination::dbg_window() );
    g_l->writer().add_destination( destination::file("out.txt") );

    // Step 7: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++;

    std::string hello = "hello", world = "world";
    LAPP_ << hello << ", " << world;

    g_log_level.set_enabled(level::error);
    LDBG_ << "this will not be written anywhere";
    LAPP_ << "this won't be written anywhere either";

    g_log_level.set_enabled(level::info);
    LAPP_ << "good to be back ;) " << i++;

    /*
    Step 8 : Enjoy!

    The output will be written to the console, debug window, and "out.txt" file.
    It will look similar to this one:

    12:59.27 [1] this is so cool 1
    12:59.27 [2] hello, world
    12:59.27 [3] good to be back ;) 2
    */
}



#ifdef SINGLE_TEST

int main() {
    test_mul_levels_one_logger();
}

#endif

// End of file

