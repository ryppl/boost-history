
#include "log.h"
#include <boost/logging/format.hpp>

// Step 6: Define the filters and loggers you'll use
BOOST_DEFINE_LOG(g_l, log_type)
BOOST_DEFINE_LOG_FILTER(g_l_filter, level::holder)

using namespace boost::logging;

void init_logs() {
    // Add formatters and destinations
    // That is, how the message is to be formatted...
    g_l->writer().add_formatter( formatter::idx() );
    g_l->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_l->writer().add_formatter( formatter::append_enter() );

    //        ... and where should it be written to
    g_l->writer().add_destination( destination::cout() );
    g_l->writer().add_destination( destination::dbg_window() );
    g_l->writer().add_destination( destination::file("out.txt") );
}
