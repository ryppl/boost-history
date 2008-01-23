


#include <boost/logging/format_fwd.hpp>
#include <boost/logging/format/named_writer.hpp>

// Step 3 : Specify your logging class(es)
typedef boost::logging::named_logger<>::type logger_type;


// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_level, boost::logging::level::holder ) // holds the application log level
BOOST_DECLARE_LOG(g_log_err, logger_type) 
BOOST_DECLARE_LOG(g_log_app, logger_type)
BOOST_DECLARE_LOG(g_log_dbg, logger_type)

// Step 5: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_dbg(), g_log_level(), debug ) << "[dbg] "
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_err(), g_log_level(), error )
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_app(), g_log_level(), info ) << "[app] "


// Step 6: Define the filters and loggers you'll use (usually in a source file)
BOOST_DEFINE_LOG_FILTER(g_log_level, boost::logging::level::holder ) 
BOOST_DEFINE_LOG(g_log_err, logger_type)
BOOST_DEFINE_LOG(g_log_app, logger_type)
BOOST_DEFINE_LOG(g_log_dbg, logger_type)

using namespace boost::logging;

void mul_levels_mul_logers_example() {
    // reuse the same destination for 2 logs
    destination::file out("out.txt");
    g_log_app()->writer().replace_destination("file", out);
    g_log_dbg()->writer().replace_destination("file", out);
    // formatting (first param) and destinations (second param)
    g_log_err()->writer().write("[%idx%] %time%($hh:$mm.$ss) |\n", "file(err.txt)");
    g_log_app()->writer().write("[%idx%] %time%($hh:$mm.$ss) |\n", "file cout");
    g_log_dbg()->writer().write("[%idx%] %time%($hh:$mm.$ss) |\n", "file cout debug");

    g_log_app()->mark_as_initialized();
    g_log_err()->mark_as_initialized();
    g_log_dbg()->mark_as_initialized();









    g_log_err()->writer().write("%idx%/ [%time%($mm.$ss.$mili)] |\n", "file(err.txt)");
    g_log_app()->writer().write("%idx%/ [%time%($mm.$ss.$mili)] |\n", "file cout");
    g_log_dbg()->writer().write("%idx%/ [%time%($mm.$ss.$mili)] |\n", "file cout debug");





    // Step 8: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++;
    LDBG_ << "this is so cool again " << i++;
    LERR_ << "first error " << i++;

    std::string hello = "hello", world = "world";
    LAPP_ << hello << ", " << world;

    g_log_level()->set_enabled(level::error);
    LDBG_ << "this will not be written anywhere";
    LAPP_ << "this won't be written anywhere either";

    g_log_level()->set_enabled(level::info);
    LAPP_ << "good to be back ;) " << i++;
    LERR_ << "second error " << i++;

    // Step 9 : Enjoy!
}




int main() {
    mul_levels_mul_logers_example();
}


// End of file

