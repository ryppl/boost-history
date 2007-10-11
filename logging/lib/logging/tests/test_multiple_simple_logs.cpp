// test_multiple_simple_logs.cpp

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
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>

using namespace boost::logging;

struct write_to_file {
    typedef boost::shared_ptr<std::ofstream> ptr;

    write_to_file(const std::string & filename) : m_out(new std::ofstream(filename.c_str())) {}
    void operator()(const std::string & msg) const {
        (*m_out) << msg << std::endl ;
    }

    // Always, when you need to hold some context in your writer object, you need to hold
    // it as a shared pointer - this is because multiple copies of the same object might exist at the same time
    mutable ptr m_out;
};

struct write_to_cout {
    void operator()(const std::string & msg) const {
        std::cout << msg << std::endl ;
    }
};

struct write_to_dbg {
    void operator()(const std::string & msg) const {
#ifdef BOOST_WINDOWS
        ::OutputDebugStringA( (msg + "\n").c_str() );
#endif
    }
};

typedef process_msg< gather::ostream_like::return_str<>, write_to_cout> processor_cout;
typedef process_msg< gather::ostream_like::return_str<>, write_to_file> processor_file;
typedef process_msg< gather::ostream_like::return_str<>, write_to_dbg> processor_dbg;

logger<processor_cout, filter::no_ts> g_log_app;
logger<processor_file, filter::no_ts> g_log_err("err.txt");
logger<processor_dbg,  filter::no_ts> g_log_dbg;

#define LAPP_ if ( !g_log_app) ; else g_log_app->read_msg().gather().out()
#define LERR_ if ( !g_log_err) ; else g_log_err->read_msg().gather().out()
#define LDBG_ if ( !g_log_dbg) ; else g_log_dbg->read_msg().gather().out()


void test_multiple_simple_logs() {
    // read from a file, dump it word by word
    std::ifstream in("readme.txt");
    std::string word;

    g_log_err.set_enabled(false);

    // when reaching these many words, we toggle the logs (enable/disable them)
    int toggle_count = 20;

    int idx = 1;
    for ( ; in >> word; ++idx) {
        LAPP_ << idx << " : reading word " << word;
        LERR_ << "error at " << idx << ", while reading " << word;
        LDBG_ << "debug info: " << idx << ", reading " << word;


        if ( idx % toggle_count == 0) {
            // toggle the logs
            g_log_app.set_enabled( !g_log_app.is_enabled() ); 
            g_log_err.set_enabled( !g_log_err.is_enabled() ); 
            g_log_dbg.set_enabled( !g_log_dbg.is_enabled() ); 
        }
    }
}

#ifdef SINGLE_TEST

int main() {
    test_multiple_simple_logs();
}

#endif

// End of file

