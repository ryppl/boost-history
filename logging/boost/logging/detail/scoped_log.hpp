// scoped_log.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#ifndef JT28092007_scoped_log_HPP_DEFINED
#define JT28092007_scoped_log_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {

#define BOOST_LOG_CONCATENATE2(a,b) a ## b

#define BOOST_LOG_CONCATENATE(a,b) BOOST_LOG_CONCATENATE2(a,b)

#ifndef BOOST_LOG_USE_WCHAR_T

#define BOOST_SCOPED_LOG_WITH_CLASS_NAME(logger, msg, class_name) \
struct class_name { \
    class_name()  { logger ( "start of " msg ) ;} \
    ~class_name() { logger ( "  end of " msg ) ; } \
} BOOST_LOG_CONCATENATE(log_, __LINE__);

#define BOOST_SCOPED_LOG(logger, msg) BOOST_SCOPED_LOG_WITH_CLASS_NAME(logger, msg, BOOST_LOG_CONCATENATE(boost_scoped_log,__LINE__) )

#else
// unicode
#define BOOST_SCOPED_LOG_WITH_CLASS_NAME(logger, msg, class_name) \
struct class_name { \
    class_name()  { logger ( L"start of " msg ) ;} \
    ~class_name() { logger ( L"  end of " msg ) ; } \
} BOOST_LOG_CONCATENATE(log_, __LINE__);

#define BOOST_SCOPED_LOG(logger, msg) BOOST_SCOPED_LOG_WITH_CLASS_NAME(logger, msg, BOOST_LOG_CONCATENATE(boost_scoped_log,__LINE__) )

#endif





#define BOOST_SCOPED_LOG_CTX(logger)
struct class_name { \
    class_name()  { logger ( "start of " msg ) ;} \
    ~class_name() { logger ( "  end of " msg ) ; } \
    boost::logging::logger<default_, 
} BOOST_LOG_CONCATENATE(log_, __LINE__);





}}

#endif

