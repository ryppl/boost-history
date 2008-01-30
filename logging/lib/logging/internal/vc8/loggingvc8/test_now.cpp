#define BOOST_LOG_BEFORE_INIT_USE_CACHE_FILTER

// uncomment this, and all messages inside singleton's constructor will be logged!
//#define BOOST_LOG_BEFORE_INIT_LOG_ALL

// uncomment this, and NO messages inside singleton's constructor will be logged
//#define BOOST_LOG_BEFORE_INIT_IGNORE_BEFORE_INIT

#include <boost/logging/format_fwd.hpp>

BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> )

#include <boost/logging/format.hpp>

typedef boost::logging::logger_format_write< > logger_type;


BOOST_DECLARE_LOG_FILTER(g_log_level, boost::logging::level::holder ) // holds the application log level
BOOST_DECLARE_LOG(g_log_err, logger_type) 
BOOST_DECLARE_LOG(g_log_app, logger_type)
BOOST_DECLARE_LOG(g_log_dbg, logger_type)

#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_dbg(), g_log_level(), debug ) << "[dbg] "
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_err(), g_log_level(), error )
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_log_app(), g_log_level(), info ) << "[app] "

BOOST_DEFINE_LOG_FILTER(g_log_level, boost::logging::level::holder ) 
BOOST_DEFINE_LOG(g_log_err, logger_type)
BOOST_DEFINE_LOG(g_log_app, logger_type)
BOOST_DEFINE_LOG(g_log_dbg, logger_type)

using namespace boost::logging;

struct singleton {
    singleton() {
        // note: these messages are written before logs are initialized
        int i = 1;
        LDBG_ << "this is so cool " << i++;
        LDBG_ << "this is so cool again " << i++;
        LERR_ << "first error " << i++;

        std::string hello = "hello", world = "world";
        LAPP_ << hello << ", " << world;

        LAPP_ << "coolio " << i++;
        LERR_ << "second error " << i++;
        LDBG_ << "some debug message" << i++;
    }
} s_;

void init_logs() {
    // Err log
    g_log_err()->writer().add_formatter( formatter::idx(), "[%] "  );
    g_log_err()->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_err()->writer().add_formatter( formatter::append_newline() );
    g_log_err()->writer().add_destination( destination::file("err.txt") );

    destination::file out("out.txt");
    // App log
    g_log_app()->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_app()->writer().add_formatter( formatter::append_newline() );
    g_log_app()->writer().add_destination( out );
    g_log_app()->writer().add_destination( destination::cout() );

    // Debug log
    g_log_dbg()->writer().add_formatter( formatter::time("$hh:$mm.$ss ") );
    g_log_dbg()->writer().add_formatter( formatter::append_newline() );
    g_log_dbg()->writer().add_destination( out );
    g_log_dbg()->writer().add_destination( destination::dbg_window() );

    // if you change this, you'll get a different output (more or less verbose)
    g_log_level()->set_enabled(level::info);

    g_log_err()->mark_as_initialized();
    g_log_app()->mark_as_initialized();
    g_log_dbg()->mark_as_initialized();
}

void cache_before_init_example() {
    init_logs();
    int i = 10;
    LAPP_ << "after logs have been initialized " << i++;
    g_log_level()->set_enabled(level::debug);
    LDBG_ << "some debug message after logs have been initialized " << i++;
}




int main() {
    cache_before_init_example();
}


// End of file

