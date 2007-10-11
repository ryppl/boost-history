// test_simple_format_and_write_simple_formatter.cpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
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
    Using simple formatters/destinations.
    Also, using the simple router: which just calls all formatters, and then all destinations.

    Always, in your applications, all formatters need to have a common base class. Same for destinations.

    Other than that, you can choose your "string" type, and then implement operator().

    Note that here, for formatters, I'm using cache_string - which optimizes formatting the strings.

    Also, I'm adding formatters/destinations, and at a later time, I'm removing some - thus, when logging,
    we won't call the deleted formatters/destinations.
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

struct write_to_file : destination_base, destination::non_const_context<std::ofstream> {
    write_to_file(const char* filename) : non_const_context_base(filename) {}
    void operator()(param msg) const {
        context() << msg ;
    }
};

typedef process_msg< 
    gather::ostream_like::return_cache_str<> , 
    writer::format_write< 
        format_base, 
        destination_base, format_and_write::simple<cache_string> > > process;
logger<process, filter::no_ts> g_l;

#define L_ if ( !g_l) ; else g_l->read_msg().gather().out()


void test_simple_format_and_write_simple_formatter() {
    // add formatters : [idx] [time] message [enter]
    g_l->writer().add_formatter( write_idx() );
    g_l->writer().add_formatter( write_time() );
    g_l->writer().add_formatter( append_enter() );

    // write to cout and file
    g_l->writer().add_destination( write_to_cout() );
    g_l->writer().add_destination( write_to_file("out.txt") );

    int i = 1;
    L_ << "must be prefixed by index and time , enter is appended as well " << i++;
    L_ << "must be prefixed by index and time , enter is appended as well " << i++;
    L_ << "must be prefixed by index and time , enter is appended as well " << i++;

    g_l->writer().del_formatter( write_idx() );
    g_l->writer().del_formatter( append_enter() );
    g_l->writer().del_destination( write_to_cout() );

    // will not be written to cout
    L_ << "must be prefixed by time , no enter " << i++;
    L_ << "must be prefixed by time , no enter " << i++;
}

#ifdef SINGLE_TEST

int main() {
    test_simple_format_and_write_simple_formatter();
}

#endif

// End of file

