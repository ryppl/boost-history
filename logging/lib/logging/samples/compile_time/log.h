#ifndef LOG_H_header
#define LOG_H_header

#include <boost/logging/format_fwd.hpp>
#include <boost/logging/tags.hpp>

// Step 1: Optimize : use tags (on top of a cache string, to make formatting the message faster)
namespace bl = boost::logging;
typedef bl::tag::holder< bl::optimize::cache_string_one_str<>, bl::tag::file_line, bl::tag::thread_id, bl::tag::time> log_string;
BOOST_LOG_FORMAT_MSG( log_string )

#ifndef BOOST_LOG_COMPILE_FAST
#include <boost/logging/format.hpp>
#endif

// Step 3 : Specify your logging class(es)
using namespace boost::logging::scenario::usage;
typedef use<
        filter_::change::single_thread, 
        filter_::level::use_levels, 
        logger_::change::single_thread, 
        logger_::favor::correctness> finder;

// Step 4: declare which filters and loggers you'll use
BOOST_DECLARE_LOG_FILTER(g_l_filter, finder::filter ) 
BOOST_DECLARE_LOG(g_l, finder::logger) 

// Step 5: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), debug ) .set_tag( BOOST_LOG_TAG_FILELINE)
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), error ) .set_tag( BOOST_LOG_TAG_FILELINE)
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), info ) .set_tag( BOOST_LOG_TAG_FILELINE)

void init_logs();

#endif
