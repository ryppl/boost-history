// test_dump_to_file_and_cout_ostream_like.cpp

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
#include <boost/logging/process_msg/ostream_like.hpp>
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace boost::logging;

/* 
    Dump to both cout and file, using the cool "<<" operator.

    Note that writing to the log is totally decoupled from the logging syntax , 
    while being efficient (if the log is disabled, we don't compute anything related to the message).
*/

struct write_to_cout_and_file {
    typedef boost::shared_ptr<std::ofstream> ptr;

    write_to_cout_and_file(const std::string & filename) : m_out(new std::ofstream(filename.c_str())) {}
    void operator()(const std::string & msg) const {
        std::cout << msg << std::endl ;
        (*m_out) << msg << std::endl ;
    }

    // Always, when you need to hold some context in your writer object, you need to hold
    // it as a shared pointer - this is because multiple copies of the same object might exist at the same time
    mutable ptr m_out;
};

typedef process_msg< gather::ostream_like::return_str<>, write_to_cout_and_file> processor;
logger<processor, filter::no_ts> g_single_log("out.txt");

#define L_ if ( !g_single_log) ; else g_single_log->read_msg().gather().out()


void test_dump_to_file_and_cout_ostream_like() {
    // read from a file, dump it word by word
    std::ifstream in("readme.txt");
    std::string word;

    // when reaching these many words, we toggle the log (enable/disable it)
    int toggle_count = 20;

    int idx = 1;
    for ( ; in >> word; ++idx) {
        L_ << idx << " : reading word " << word;

        if ( idx % toggle_count == 0)
            g_single_log.set_enabled( !g_single_log.is_enabled() ); // toggle the log
    }
}

#ifdef SINGLE_TEST

int main() {
    test_dump_to_file_and_cout_ostream_like();
}

#endif

// End of file

