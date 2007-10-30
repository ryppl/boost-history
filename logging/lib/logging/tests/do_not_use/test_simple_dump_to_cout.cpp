// simple_dump_to_cout.cpp

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
#include <iostream>
#include <fstream>

/* 
    As simple as it gets: Have a simple non-thread safe logger (filter::no_ts),
    and if the log is enabled, dump to cout.

    We log full messages, no using any "<<" when doing logging. That is,
    we do this:
    L_("my message")

    not:
    L_ << "my message comes from user " << user << ", with greetings"
*/

struct write_to_cout {
    void operator()(const std::string & msg) const {
        std::cout << msg << std::endl ;
    }
};

using namespace boost::logging;
logger<write_to_cout, filter::no_ts> g_single_log;

// FIXME i need to create a macro for "if ( sink<write_to_cout> s = g_single_log) s.process_msg()"
#define L_(x) if ( g_single_log) g_single_log.process_msg()(x)


void test_simple_dump_to_cout() {
    // read from a file, dump it word by word
    std::ifstream in("readme.txt");
    std::string word;

    // we allow only this many words
    int allow_word_count = 100;

    int idx = 0;
    for ( ; in >> word; ++idx)
        if ( idx < allow_word_count )
            L_("reading word " + word);
        else if ( idx == allow_word_count )
            // disable the log
            g_single_log.set_enabled(false);
}


#ifdef SINGLE_TEST

int main() {
    test_simple_dump_to_cout();
}

#endif

// End of file

