// my_app_log.cpp
#include "my_app_log.h"
#include <boost/logging/format.hpp>
#include <boost/logging/format/formatter/tags.hpp>

// uncomment if you're using Named Formatters and Destinations
// #include <boost/logging/format/named_write.hpp>

using namespace boost::logging;

BOOST_DEFINE_LOG_FILTER(g_log_filter, finder::filter ) 
BOOST_DEFINE_LOG(g_l, finder::logger) 


void init_logs() {
    // Add formatters and destinations
    // That is, how the message is to be formatted...
    g_l()->writer().add_formatter( formatter::tag::thread_id() );
    g_l()->writer().add_formatter( formatter::tag::time("$hh:$mm.$ss ") );
    g_l()->writer().add_formatter( formatter::idx() );
    g_l()->writer().add_formatter( formatter::append_newline() );

    //        ... and where should it be written to
    g_l()->writer().add_destination( destination::cout() );
    g_l()->writer().add_destination( destination::dbg_window() );
    g_l()->writer().add_destination( destination::file("out.txt") );
    g_l()->mark_as_initialized();
}
