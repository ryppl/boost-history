

#include <boost/logging/format/named_write.hpp>
typedef boost::logging::named_logger<>::type logger_type;

#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_dbg(), g_log_level(), debug ) << "[dbg] "

BOOST_DEFINE_LOG_FILTER(g_log_level, boost::logging::level::holder ) 
BOOST_DEFINE_LOG(g_log_dbg, logger_type)

using namespace boost::logging;

void mul_levels_mul_logers_example() {
    // reuse the same destination for 2 logs
    destination::file out("out.txt");
    g_log_dbg()->writer().replace_destination("file", out);
    // formatting (first param) and destinations (second param)
    g_log_dbg()->writer().write("%time%($hh:$mm.$ss) |\n", "file cout debug");
    g_log_dbg()->mark_as_initialized();


    // Step 8: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++;

    g_log_level()->set_enabled(level::error);
    LDBG_ << "this will not be written anywhere";

    g_log_level()->set_enabled(level::info);
}




int main() {
    mul_levels_mul_logers_example();
}


// End of file

