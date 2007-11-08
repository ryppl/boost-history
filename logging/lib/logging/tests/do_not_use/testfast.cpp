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
using namespace boost::logging;

// Step 3 : Specify your logging class(es)
//  - the filter is always accurate (but slow)
//  - the filter does not use levels
//  - the logger favors speed (on a dedicated thread)
//  - the logger is initialized once, when only one thread is running
using namespace boost::logging::scenario::usage;
typedef use<filter_::change::always_accurate, filter_::level::no_levels, logger_::change::set_once_when_one_thread, logger_::favor::speed> finder;


void your_scenario_example() {
    finder ::filter f;
    finder:: logger l;
    int i = 0;
}



#ifdef SINGLE_TEST

int main() {
    your_scenario_example();
}

#endif

// End of file

