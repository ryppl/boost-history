/**
@example use_tss_ostringstream.cpp

@copydoc use_tss_ostringstream

@page use_tss_ostringstream use_tss_ostringstream.cpp Example


This usage:
- You have one logger
- You have one filter, always turned on
- You want to format the message before it's written 
- The logger has several log destinations
    - The output goes to console and debug output window
    - Formatting - prefix each message by its index, and append newline

Optimizations:
- use tss_ostringstream (each thread has its own ostringstream copy, to make writing faster: 
  when logging of a message, we won't need to create the ostringstream first ; it's created only once per thread )
- use a cache string (from optimize namespace), in order to make formatting the message faster


In this example, all output will be written to the console and debug window.
It will be:

@code
[1] this is so cool 1
[2] this is so cool again 2
[3] this is too cool 3
@endcode

*/



#include <boost/logging/format_fwd.hpp>

// Step 1: Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> )

BOOST_LOG_GATHER_CLASS( use_ostringstream< tss_ostringstream<> > )


#include <boost/logging/format.hpp>
#include <boost/logging/writer/ts_write.hpp>

using namespace boost::logging;

// Step 3 : Specify your logging class(es)
typedef logger_format_write< > log_type;


// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DECLARE_LOG(g_l, log_type) 

// Step 5: define the macros through which you'll log
#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l, g_log_filter->is_enabled() ) 

// Step 6: Define the filters and loggers you'll use (usually in a source file)
BOOST_DEFINE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DEFINE_LOG(g_l, log_type)


void use_tss_ostringstream_example() {
    // Step 7: add formatters and destinations
    //         That is, how the message is to be formatted and where should it be written to

    g_l->writer().add_formatter( formatter::idx(), "[%] "  );
    g_l->writer().add_formatter( formatter::append_newline_if_needed() );
    g_l->writer().add_destination( destination::cout() );
    g_l->writer().add_destination( destination::dbg_window() );
    g_l->turn_cache_off();

    // Step 8: use it...
    int i = 1;
    L_ << "this is so cool " << i++;
    L_ << "this is so cool again " << i++;
    L_ << "this is so too cool " << i++;

    // Step 9 : Enjoy!
}




int main() {
    use_tss_ostringstream_example();
}


// End of file

