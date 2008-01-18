
#define BOOST_LOG_BEFORE_INIT_IGNORE_BEFORE_INIT
#include <boost/logging/format_fwd.hpp>

// Step 1: Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> )

#include <boost/logging/format.hpp>
#include <boost/logging/format/destination/rolling_file.hpp>

using namespace boost::logging;

// Step 3 : Specify your logging class(es)
typedef logger_format_write< > log_type;


// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DECLARE_LOG(g_l, log_type) 

// Step 5: define the macros through which you'll log
#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 

// Step 6: Define the filters and loggers you'll use (usually in a source file)
BOOST_DEFINE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DEFINE_LOG(g_l, log_type)

void f () {
//    BOOST_SCOPED_LOG_CTX(L_);// << "execution_pool::executor_defer_func(...)";
    L_ << "Executing component ";
}

void one_logger_one_filter_example() {
    forward_to_logger< default_, destination::cout > l;
    logger<default_> * ll = l.common_base();
    // Step 7: add formatters and destinations
    //         That is, how the message is to be formatted and where should it be written to

    g_l()->writer().add_formatter( formatter::idx(), "[%] "  );
    g_l()->writer().add_formatter( formatter::append_newline_if_needed() );
    g_l()->writer().add_destination( destination::file("out.txt") );
    g_l()->writer().add_destination( destination::cout() );
    g_l()->writer().add_destination( destination::dbg_window() );


    destination::rolling_file_settings settings;
    settings
        .file_count( 10 )
        .max_size_bytes( 5242880 )
        .flush_each_time(true); 
    destination::rolling_file loggerFile( "test.txt", settings );
    g_l()->writer().add_destination( loggerFile );

    g_l()->turn_cache_off();

    // Step 8: use it...
    int i = 1;
    L_ << "this is so cool " << i++;
    L_ << "this is so cool again " << i++;

    std::string hello = "hello", world = "world";
    L_ << hello << ", " << world;

    g_log_filter()->set_enabled(false);
    L_ << "this will not be written to the log";
    L_ << "this won't be written to the log";

    g_log_filter()->set_enabled(true);
    L_ << "good to be back ;) " << i++;

    // Step 9 : Enjoy!
}




int main() {
    one_logger_one_filter_example();
}


// End of file

