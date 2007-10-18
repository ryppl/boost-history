// test_manipulator_with_msg_route.cpp

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


#include <boost/logging/logging.hpp>
#include <boost/logging/format/optimize.hpp>
#include <boost/logging/process_msg/ostream_like.hpp>
#include <boost/logging/format.hpp>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <time.h>
#include <iostream>
#include <fstream>


using namespace boost::logging;

/* 
    Using formatters/destinations.

    Always, in your applications, all formatters need to have a common base class. Same for destinations.

    This calls the formatters/destinations in a specific way - specified by a "route"
    (class boost::logging::msg_route::with_route)

    The simple router just calls all formatters, and then all destinations.
    However, with the with_route router, you can specify how they're called.

    In our example:

    to cout:        [idx] [time] message [enter]
    to dbg_window:  [time] message [enter]
    to file:        [idx] message [enter]

    So our route is:

    prepend [time]
    append [enter]
    to dbg window
    prepend [idx]
    to cout
    clear format
    prepend [idx]
    append [enter]
    to file
*/
typedef optimize::cache_string_one_str<> cache_string;


//////////////////////////////////////////////////////////////////////
// Formatters

typedef formatter::base<cache_string&, op_equal::same_type > format_base;

// formatter: prepend the message's index
struct write_idx : format_base, formatter::non_const_context<int> {
    write_idx() : non_const_context_base((int)0) {}
    void operator()(param str) const {
        std::stringstream idx;
        idx << "[" << ++context() << "] ";
        str.prepend_string( idx.str() );
    }
};

struct write_time : format_base {
    void operator()(cache_string & str) const {
        char t[10];
        time_t now = time(0);
        tm details = *localtime(&now);
        strftime(t, 10, "%M:%S ", &details);
        str.prepend_string(t);
    }
};

struct append_enter : format_base {
    void operator()(cache_string & str) const {
        str.append_string("\n");
    }
};

//////////////////////////////////////////////////////////////////////
// Destinations

typedef destination::base<const std::string &, op_equal::same_type> destination_base;

struct write_to_cout : destination_base {
    void operator()(param msg) const {
        std::cout << msg ;
    }
};

struct write_to_dbg : destination_base {
    void operator()(param msg) const {
#ifdef BOOST_WINDOWS
        ::OutputDebugStringA( msg.c_str() );
#else
        msg; // so that there's no warning
#endif
    }
};

struct write_to_file : destination_base, destination::non_const_context<std::ofstream> {
    write_to_file() : non_const_context_base("out.txt") {}
    void operator()(param msg) const {
        context() << msg ;
    }
};

typedef process_msg< 
    gather::ostream_like::return_cache_str<> , 
    writer::format_write< 
        format_base, 
        destination_base, 
        format_and_write::simple_care_for_clear_format<cache_string>,
        msg_route::with_route<format_base,destination_base> > > process;
logger<process, filter::no_ts> g_l;

#define L_ if ( !g_l) ; else g_l->read_msg().gather().out()



void test_manipulator_with_msg_route() {
    g_l->writer().add_formatter( write_time() );
    g_l->writer().add_formatter( write_idx() );
    g_l->writer().add_formatter( append_enter() );

    g_l->writer().add_destination( write_to_cout() );
    g_l->writer().add_destination( write_to_dbg() );
    g_l->writer().add_destination( write_to_file() );

    /*
    prepend [time]
    append [enter]
    to dbg window
    prepend [idx]
    to cout
    clear format
    prepend [idx]
    append [enter]    
    to file
    */
    g_l->writer().router().set_route()
        .fmt( write_time() ) 
        .fmt( append_enter() )
        .dest( write_to_dbg() )
        .fmt( write_idx() )
        .dest( write_to_cout() )
        .clear()
        .fmt( write_idx() )
        .fmt( append_enter() )
        .fmt( write_to_file())
        ;
//    DOES NOT work

    int i = 1;
    L_ << "must be prefixed by index and time , enter is appended as well " << i++;
}

#ifdef SINGLE_TEST

int main() {
    test_manipulator_with_msg_route();
    



}

#endif

// End of file

