#ifndef LOG_H
#define LOG_H
#pragma once

#include <boost/logging/format.hpp>

typedef boost::logging::logger_format_write< > log_type;

BOOST_DECLARE_LOG_FILTER(g_log_filter, boost::logging::filter::no_ts) 
BOOST_DECLARE_LOG(g_l, log_type )

#define L_EXE_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 

void init_logs() ;

#endif
