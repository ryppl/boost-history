#include "log.h"

BOOST_DEFINE_LOG_FILTER(g_log_filter, boost::logging::filter::no_ts) 
BOOST_DEFINE_LOG(g_l, log_type )

using namespace boost::logging;

void init_logs() {
    // first, write to a clean file (that is, don't append to it)
    g_l()->writer().add_destination( destination::file("exe.txt", destination::file_settings().initial_overwrite(true) ));
    g_l()->writer().add_destination( destination::file("dllexe.txt", destination::file_settings().do_append(true) ));
    g_l()->writer().add_formatter( formatter::idx(), "[%] "  );
    g_l()->writer().add_formatter( formatter::append_newline() );
    g_l()->writer().add_destination( destination::cout() );
    g_l()->turn_cache_off();
}

