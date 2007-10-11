// test_multiple_levels_different_logs.cpp

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
#include <boost/logging/level.hpp>
#include <boost/logging/process_msg/ostream_like.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>

using namespace boost::logging;

/* 
    Tests levels - having multiple levels, but all messages go to the same log

    By default we have these levels:

        info (smallest level),
        warning ,
        debug ,
        error ,
        fatal (highest level)

    Depending which level is enabled, some messages will reach the log: those messages having at least that level.
    For instance, if info level is enabled, all logged messages will reach the log.
    If warning level is enabled, all messages are logged, but the warnings.
    If debug level is enabled, messages that have levels debug, error, fatal will be logged.


    To see the difference between having multiple levels and logging to the same log destination, 
    or different log destinations, see test_multiple_levels_to_same_log.cpp
*/

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

typedef process_msg< gather::ostream_like::return_str<>, write_to_file> processor;

level::holder_no_ts level_holder;

typedef logger<processor, filter_level<level::holder_no_ts, level::debug> > debug_logger;
typedef logger<processor, filter_level<level::holder_no_ts, level::error> > error_logger;
typedef logger<processor, filter_level<level::holder_no_ts, level::info> > info_logger;

debug_logger g_log_dbg( init_both, "dbg.txt", &level_holder );
error_logger g_log_err( init_both, "err.txt", &level_holder );
info_logger g_log_app( init_both, "out.txt", &level_holder );


#define LAPP_ if ( !g_log_app) ; else g_log_app->read_msg().gather().out()
#define LERR_ if ( !g_log_err) ; else g_log_err->read_msg().gather().out()
#define LDBG_ if ( !g_log_dbg) ; else g_log_dbg->read_msg().gather().out()

void test_multiple_levels_different_logs() {
    // read from a file, dump it word by word
    std::ifstream in("readme.txt");
    std::string word;

    // at this time, print everything
    level_holder.set_enabled(level::info);

    // when reaching these many words, we toggle the log level (enable/disabling some of the log messages)
    int toggle_count = 10;

    int idx = 1;
    int idx_toggle_level = 0;
    for ( ; in >> word; ++idx) {
        LAPP_ << "info at : " << idx << " : reading word " << word;
        LDBG_ << "debug at: " << idx << ", reading " << word;
        LERR_ << "error at: " << idx << ", while reading " << word;
    
        if ( idx % toggle_count == 0) {
            // toggle the logs
            switch ( ++idx_toggle_level % 4) {
                case 0: level_holder.set_enabled(level::info); break;
                case 1: level_holder.set_enabled(level::debug); break;
                case 2: level_holder.set_enabled(level::error); break;
                case 3: level_holder.set_enabled(level::fatal); break;
            }
        }
    }
}

#ifdef SINGLE_TEST

int main() {
    test_multiple_levels_different_logs();
}

#endif

// End of file

