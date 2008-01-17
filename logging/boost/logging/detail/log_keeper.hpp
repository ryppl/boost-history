// log_keeper.hpp

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


#ifndef JT28092007_log_keeper_HPP_DEFINED
#define JT28092007_log_keeper_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace logging {

namespace detail {

    template<class T> struct fast_compile_with_default_gather ;


} // namespace detail


/** 
    @brief Allows using a log without knowing its full type yet. Even if the log is not fully @b defined, you can still use it.

    This will allow you to log messages even if you don't know the full type of the log (which can aid compilation time).
*/
template<class type, class gather_msg = default_, class dummy = override> struct log_holder {
    typedef typename use_default<gather_msg, typename ::boost::logging::detail::fast_compile_with_default_gather<dummy>::gather_msg > ::type gather_type;
    typedef logger<gather_type> logger_base;

    BOOST_LOGGING_FORWARD_CONSTRUCTOR_WITH_NEW_AND_INIT(log_holder, m_log, type, init)
    ~log_holder() { 
        delete m_base;
        m_base = 0;
        delete m_log; 
        m_log = 0;
    }

    const type* operator->() const      { return m_log; }
    type* operator->()                  { return m_log; }

    /** 
        in case you want to get the real log object
    */
    const type* get() const             { return m_log; }
    type* get()                         { return m_log; }


    const logger_base * base() const    { return m_base; }
    logger_base * base()                { return m_base; }

private:
    void init() {
        m_base = 0;
        typedef typename type::write_type write_type;
        m_base = new forward_to_logger<gather_type, write_type>(m_log);
    }
private:
    type * m_log;
    logger_base * m_base;
};

/** 
    @brief Ensures the log is created before main(), even if not used before main

    We need this, so that we won't run into multi-threaded issues while the log is created
    (in other words, if the log is created before main(), we can safely assume there's only one thread running,
    thus no multi-threaded issues)
*/
struct ensure_early_log_creation {
    template<class type> ensure_early_log_creation ( type & log) {
#ifndef BOOST_NO_INT64_T
    typedef boost::int64_t long_type ;
#else
    typedef long long_type ;
#endif
        long_type ignore = reinterpret_cast<long_type>(&log);
        // we need to force the compiler to force creation of the log
        if ( time(0) < 0)
            if ( time(0) < (time_t)ignore) {
                printf("LOGGING LIB internal error - should NEVER happen. Please report this to the author of the lib");
                exit(0);
            }
    }
};


/** 
    @brief Ensures the filter is created before main(), even if not used before main

    We need this, so that we won't run into multi-threaded issues while the filter is created
    (in other words, if the filter is created before main(), we can safely assume there's only one thread running,
    thus no multi-threaded issues)
*/
typedef ensure_early_log_creation ensure_early_filter_creation;

/** 
    Useful for log_holder - to get the logger' base (so that we can use it even without knowing the full log's definition).

    If used on a logger, it just returns it .
*/
template<class logger> inline logger* get_logger_base(logger * l) { return l; }
template<class logger> inline const logger* get_logger_base(const logger * l) { return l; }

template<class type, class gather_msg> inline typename log_holder<type,gather_msg>::logger_base* get_logger_base(log_holder<type,gather_msg> & l) { return l.base(); }
template<class type, class gather_msg> inline const typename log_holder<type,gather_msg>::logger_base* get_logger_base(const log_holder<type,gather_msg> & l) { return l.base(); }



}}

#endif

