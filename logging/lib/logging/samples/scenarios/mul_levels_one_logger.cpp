/**
@example mul_levels_one_logger.cpp

@copydoc mul_levels_one_logger

@page mul_levels_one_logger mul_levels_one_logger.cpp Example

This usage:
- You have multiple levels (in this example: debug < info < error)
- You want to format the message before it's written 
  (in this example: prefix it by time, by index, and append newline to it)
- You have <b>one log</b>, which writes to several log destinations
  (in this example: the console, the debug output window, and a file)

In this example, all output will be written to the console, debug output window, and "out.txt" file.
It will look similar to this one:

@code
21:03.17.243 [1] this is so cool 1
21:03.17.243 [2] first error 2
21:03.17.243 [3] hello, world
21:03.17.243 [4] second error 3
21:03.17.243 [5] good to be back ;) 4
21:03.17.243 [6] third error 5
@endcode

*/



#include <boost/logging/format.hpp>
#include <boost/logging/writer/ts_write.hpp>
//#include <boost/logging/format/formatter/high_precision_time.hpp>
#include <boost/logging/format/destination/named.hpp>

using namespace boost::logging;
// Step 3 : Specify your logging class(es)
typedef logger_format_write< > log_type;

// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_level, level::holder ) 
BOOST_DECLARE_LOG(g_l, log_type) 

// Step 5: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, debug )
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, error )
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_level, info )

// Step 6: Define the filters and loggers you'll use (usually in a source file)
BOOST_DEFINE_LOG_FILTER(g_log_level, level::holder ) // holds the application log level
BOOST_DEFINE_LOG(g_l, log_type)


void test_mul_levels_one_logger() {
    // Step 7: add formatters and destinations
    //         That is, how the message is to be formatted...
    g_l->writer().add_formatter( formatter::idx(), "[%] "  );
    //g_l->writer().add_formatter( formatter::high_precision_time("$hh:$mm:$ss.$mili ") );
    g_l->writer().add_formatter( formatter::append_newline() );

    //        ... and where should it be written to
    g_l->writer().add_destination( 
        destination::named("cout debug out")
            .add( "cout", destination::cout())
            .add( "out", destination::file("out.txt"))
            .add( "debug", destination::dbg_window() )
         );

    g_l->turn_cache_off();

    // Step 8: use it...
    int i = 1;
    LDBG_ << "this is so cool " << i++;
    LERR_ << "first error " << i++;

    std::string hello = "hello", world = "world";
    LAPP_ << hello << ", " << world;

    g_log_level->set_enabled(level::error);
    LDBG_ << "this will not be written anywhere";
    LAPP_ << "this won't be written anywhere either";
    LERR_ << "second error " << i++;

    g_log_level->set_enabled(level::info);
    LAPP_ << "good to be back ;) " << i++;
    LERR_ << "third error " << i++;

    // Step 9 : Enjoy!
}



int main() {
    test_mul_levels_one_logger();
}


// End of file

