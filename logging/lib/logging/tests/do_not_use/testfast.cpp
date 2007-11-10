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



#define BOOST_LOG_COMPILE_FAST_OFF
#include <boost/logging/format_fwd.hpp>
#include <boost/logging/tags.hpp>

using namespace boost::logging;
typedef tag::holder< optimize::cache_string_one_str<>, tag::time, tag::file_line, tag::function> string;

// Step 1: Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( string )


// Step 3 : Specify your logging class(es)
typedef logger_format_write< > log_type;

#include <boost/logging/format.hpp>
#include <boost/logging/writer/ts_write.hpp>
#include <boost/logging/format/formatter/tags.hpp>


// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DECLARE_LOG(g_l, log_type) 
BOOST_DEFINE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DEFINE_LOG(g_l, log_type) 

#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l, g_log_filter->is_enabled() ) .set_tag(BOOST_LOG_TAG_FILELINE ).set_tag(BOOST_LOG_TAG_FUNCTION)



void your_scenario_example() {

    g_l->writer().add_formatter( formatter::idx() );
//    g_l->writer().add_formatter( formatter::tag::file_line() );
    g_l->writer().add_formatter( formatter::tag::function() );
    g_l->writer().add_formatter( formatter::append_newline() );
    g_l->writer().add_destination( destination::file("out.txt") );
    g_l->writer().add_destination( destination::cout() );
    g_l->writer().add_destination( destination::dbg_window() );

    // Step 8: use it...
    int i = 1;
    L_ << "this is so cool " << i++;
    L_ << "this is so cool again " << i++;

}



#ifdef SINGLE_TEST

int main() {
    your_scenario_example();
}

#endif

// End of file

