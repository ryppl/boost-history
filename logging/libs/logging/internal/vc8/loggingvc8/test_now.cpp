#ifndef my_app_LOG_H_header
#define my_app_LOG_H_header

#include "boost/logging/logging.hpp"
#include "boost/logging/format_fwd.hpp"

namespace bl = boost::logging;

typedef bl::tag::holder< bl::optimize::cache_string_one_str<>
                        , bl::tag::time
                        , bl::tag::thread_id
                        , bl::tag::level
                       //, bl::tag::file_line
                       //, bl::tag::function
                       > logstring;
BOOST_LOG_FORMAT_MSG( logstring )

typedef bl::logger_format_write< > my_logger_type;
typedef bl::level::holder my_filter_type;

BOOST_DECLARE_LOG_FILTER(g_l_level, my_filter_type)
BOOST_DECLARE_LOG(g_l, my_logger_type)

#define L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_level(), lvl ).set_tag( BOOST_LOG_TAG_LEVEL(lvl) )
// for "context information" which cannot be calc'd automatically (such as time), set_tag must be called
#define LDBG_ L_ (debug)
#define LAPP_ L_ (info )
#define LERR_ L_ (error)

void init_logs();

#endif
//#include "my_app_log.h"

#include <boost/logging/format.hpp>
#include <boost/logging/format/formatter/tags.hpp>
#include <boost/logging/format/formatter/named_spacer.hpp>

using namespace boost::logging;

BOOST_DEFINE_LOG_FILTER(g_l_level, my_filter_type ) 
BOOST_DEFINE_LOG(g_l, my_logger_type) 

//void init_logs() {
// // works for Named Formatters and Destinations
// // see <boost/logging/format/named_write.hpp>
//    // formatting    : time [idx] message \n
//    // destinations  : console, file "out.txt" and debug window
//    g_l()->writer().write("%time%($hh:$mm.$ss.$mili) [%idx%] |\n", "cout file(out.txt) debug");
//    g_l()->mark_as_initialized();
//    g_l()->turn_cache_off();
//}

void init_logs() {
    // Add formatters and destinations
    // That is, how the message is to be formatted...
   g_l()->writer().add_formatter( formatter::named_spacer( "%time% [T%thread_id%] %level% :" )
        // (the list of formatters must correspond to the list of tags in the logstring (bl::tag::holder)
      .add("time", formatter::tag::time("$hh:$mm.$ss.$mili ") )
      .add("thread_id", formatter::tag::thread_id() )
      .add("level", formatter::tag::level() )
//      .add("file_line", formatter::tag::file_line() )
//      .add("function", formatter::tag::function() )
      );
    g_l()->writer().add_formatter( formatter::append_newline() );

    //        ... and where should it be written to
    g_l()->writer().add_destination( destination::cout() );
    g_l()->writer().add_destination( destination::dbg_window() );
    g_l()->writer().add_destination( destination::file("out.txt") );
    g_l()->mark_as_initialized();

    BOOST_SCOPED_LOG_CTX(LDBG_) << "a";
}
