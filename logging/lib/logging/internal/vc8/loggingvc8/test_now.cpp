
#define BOOST_LOG_COMPILE_FAST_OFF
#include <boost/logging/format_fwd.hpp>
#include <boost/logging/tags.hpp>

// Step 1: Optimize : use a cache string, to make formatting the message faster
namespace bl = boost::logging;
typedef bl::tag::holder< bl::optimize::cache_string_one_str<>, bl::tag::file_line, bl::tag::thread_id, bl::tag::time> log_string; //(1a)
//typedef bl::tag::holder< bl::hold_string_type, bl::tag::file_line, bl::tag::thread_id, bl::tag::time> log_string; //(1b)
BOOST_LOG_FORMAT_MSG( log_string )

#include <boost/logging/format.hpp>
#include <boost/logging/writer/ts_write.hpp>
#include <boost/logging/format/formatter/tags.hpp>

#include <boost/logging/detail/scoped_log.hpp>


using namespace boost::logging;

// Step 3 : Specify your logging class(es)
using namespace boost::logging::scenario::usage;
typedef use<default_,filter_::level::use_levels> finder;

// Step 4: declare which filters and loggers you'll use (usually in a header file)
BOOST_DECLARE_LOG_FILTER(g_log_filter, finder::filter )
BOOST_DECLARE_LOG(g_l, finder::logger)

// Step 5: define the macros through which you'll log
//#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l, g_log_filter->is_enabled() ) .set_tag( BOOST_LOG_TAG_FILELINE)
#define LDBG BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_filter, debug ) \
  .set_tag( BOOST_LOG_TAG_FILELINE) << "[dbg] "
#define LINF BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_filter, info ) \
  .set_tag( BOOST_LOG_TAG_FILELINE) << "[inf] "
#define LWRN BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_filter, warning ) \
  .set_tag( BOOST_LOG_TAG_FILELINE) << "[wrn] "
#define LERR BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_filter, error ) \
  .set_tag( BOOST_LOG_TAG_FILELINE) << "[err] "
#define LFAT BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_log_filter, fatal ) \
  .set_tag( BOOST_LOG_TAG_FILELINE) << "[fat] "

void init_logs();




// Step 6: Define the filters and loggers you'll use (usually in a source file)
BOOST_DEFINE_LOG_FILTER(g_log_filter, finder::filter )
BOOST_DEFINE_LOG(g_l, finder::logger)

void init_logs() {
  using namespace boost::logging;
  // Add formatters and destinations
  // That is, how the message is to be formatted...
  g_l->writer().add_formatter( formatter::tag::time("$hh:$mm.$ss ") );
//  g_l->writer().add_formatter( formatter::tag::file_line() );
  g_l->writer().add_formatter( formatter::idx() );
//  g_l->writer().add_formatter( formatter::append_newline_if_needed() ); //(2a)
  g_l->writer().add_formatter( formatter::append_newline() ); //(2b)

  //        ... and where should it be written to
  g_l->writer().add_destination( destination::cout() );
  //g_l->writer().add_destination( destination::dbg_window() );
/*  g_l->writer().add_destination
    ( destination::rolling_file("atne_logging",
                                destination::rolling_file_settings().max_size_bytes(1024 * 10) )
      );
 */ g_l->writer().add_destination( destination::file("out.txt") );
} 



void test(int a, const char * str) {
//    BOOST_SCOPED_LOG(LDBG, "testing inout" << a << str );
    std::string s;
    BOOST_SCOPED_LOG(LDBG << , "testing inout" );
//    BOOST_SCOPED_LOG(LDBG << , "testing inout2" );
    int i = 1;
//    LDBG << "this is so cool " << i++ << "\n";
}


void test2(int a, const char * str) {
    BOOST_SCOPED_LOG_CTX(LDBG) << "test21-" << a << ", str=" << str;
    BOOST_SCOPED_LOG_CTX(LDBG) << "test22-" << a << ", str=" << str;
}

void your_scenario_example() {

    init_logs();

    test(5, "cucu");
    test2(5, "cucu2");
    // Step 8: use it...
    int i = 1;
    LDBG << "the end" << i++ << "\n";
}




int main() {
    your_scenario_example();
}


// End of file

