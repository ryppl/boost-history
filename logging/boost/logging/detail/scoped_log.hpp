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
#include <algorithm>

namespace boost { namespace logging {


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


namespace detail {
    template<class writer> struct scoped_writer {
        scoped_writer(const writer & w) : m_w( w ) {}
        template<class msg_type> void operator()(msg_type & msg) const {
            m_w.gather_msg(msg);
        }
    private:
        const writer &m_w;
    };
}

#define BOOST_SCOPED_LOG_CTX_IMPL(logger_macro, operator_, class_name) \
struct class_name { \
    typedef ::boost::logging::hold_string_type string_type; \
    class_name()  { } \
    ~class_name() { logger_macro operator_ BOOST_LOG_STR("  end of ") operator_ m_str ; } \
    void gather_msg(string_type & str) const { std::swap(m_str, str); logger_macro operator_ BOOST_LOG_STR("start of ") operator_ m_str ; } \
    mutable string_type m_str; \
} BOOST_LOG_CONCATENATE(log_, __LINE__); \
::boost::logging::logger< ::boost::logging::gather::ostream_like::return_str< ::boost::logging::hold_string_type > , ::boost::logging::detail::scoped_writer< class_name > > \
    BOOST_LOG_CONCATENATE(scoped_log_val, __LINE__) ( BOOST_LOG_CONCATENATE(log_, __LINE__) ); \
    BOOST_LOG_CONCATENATE(scoped_log_val, __LINE__) .read_msg().gather().out()



// note: to use BOOST_SCOPED_LOG_CTX, you need to #include <boost/logging/gather/ostream_like.hpp>
//       This is included by default, in #include <boost/logging/format_fwd.hpp>
#define BOOST_SCOPED_LOG_CTX(logger) BOOST_SCOPED_LOG_CTX_IMPL(logger, << , BOOST_LOG_CONCATENATE(boost_scoped_log,__LINE__) )


}}

#endif

