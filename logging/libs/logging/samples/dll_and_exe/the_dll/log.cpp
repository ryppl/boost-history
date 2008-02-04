#include "dll_log.h"

// note : we export this filter & logger
THE_DLL_API BOOST_DEFINE_LOG_FILTER(g_dll_log_filter, finder::filter ) 
THE_DLL_API BOOST_DEFINE_LOG(g_dll_l, finder::logger )

using namespace boost::logging;

void init_logs() {
    // first, write to a clean file (that is, don't append to it)
    g_dll_l()->writer().add_destination( destination::file("dll.txt", destination::file_settings().initial_overwrite(true) ));
    g_dll_l()->writer().add_destination( destination::file("dllexe.txt", destination::file_settings().do_append(true) ));
    g_dll_l()->writer().add_formatter( formatter::append_newline_if_needed() );
    g_dll_l()->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_dll_l()->writer().add_destination( destination::cout() );
    g_dll_l()->turn_cache_off();
}
