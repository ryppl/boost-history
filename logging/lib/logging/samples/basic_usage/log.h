#ifndef LOG_H_header
#define LOG_H_header

#include <boost/logging/format_fwd.hpp>

// Step 1: Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> ) 

#ifndef BOOST_LOG_COMPILE_FAST
#include <boost/logging/format.hpp>
#endif

// Step 3 : Specify your logging class(es)
typedef boost::logging::logger_format_write< > log_type;

// Step 4: declare which filters and loggers you'll use
BOOST_DECLARE_LOG_FILTER(g_l_filter, level::holder)
BOOST_DECLARE_LOG(g_l, log_type)

// Step 5: define the macros through which you'll log
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_l_filter, debug ) << "[dbg] "
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_l_filter, error ) << "[ERR] "
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l, g_l_filter, info )

void init_logs();

#endif
