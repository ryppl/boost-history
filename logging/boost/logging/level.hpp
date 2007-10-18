// level.hpp

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


#ifndef JT28092007_level_HPP_DEFINED
#define JT28092007_level_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {

/**
    @brief Handling levels - classes that can hold and/or deal with levels - filters and level holders

    By default we have these levels:

        - debug (smallest level),
        - info,
        - warning ,
        - error ,
        - fatal (highest level)

    Depending on which level is enabled for your application, some messages will reach the log: those 
    messages having at least that level. For instance, if info level is enabled, all 
    logged messages will reach the log.
    If warning level is enabled, all messages are logged, but the warnings. 
    If debug level is enabled, messages that have levels debug, error, fatal will be logged.

    @sa filter_level
*/
namespace level {
    /** the higher the level , the more critical the error */
    typedef unsigned int type;

    enum {
        disable_all = (type)-1,
        enable_all = 0,
        info = 1000,
        warning = 2000,
        debug = 3000,
        error = 4000,
        fatal = 5000
    };

    /**
        @brief Filter - holds the level, in a non-thread-safe way.

        Holds the level, and tells you if a specific level is enabled.
        It does this in a non-thread-safe way.

        If you change set_enabled() while program is running, it can take a bit to propagate
        between threads. Most of the time, this should be acceptable.
    */
    struct holder_no_ts {
        holder_no_ts(type default_level = enable_all) : m_level(default_level) {}
        bool is_enabled(type level) const { return level >= m_level; }
        void set_enabled(type level) {
            m_level = level;
        }
    private:
        type m_level;
    };

    
    /** 
        @brief Filter - holds the level, in a thread-safe way.

        Holds the level, and tells you if a specific level is enabled.
        It does this in a thread-safe way.

        However, it manages it rather ineffiently - always locking before asking.
    */
    struct holder_ts {
        typedef boost::logging::threading::scoped_lock scoped_lock;
        typedef boost::logging::threading::mutex mutex;

        holder_ts(type default_level = enable_all) : m_level(default_level) {}
        bool is_enabled(type level) const { 
            scoped_lock lk(m_cs);
            return level >= m_level; 
        }
        void set_enabled(type level) {
            scoped_lock lk(m_cs);
            m_level = level;
        }
    private:
        type m_level;
        mutable mutex m_cs;
    };

    /** 
        @brief Filter - holds the level, in a thread-safe way, using TLS.

        Uses TLS (Thread Local Storage) to find out if a level is enabled or not. It caches the current "is_enabled" on each thread.
        Then, at a given period, it retrieves the real "level".
    */
    struct holder_tls_with_cache {
        typedef locker::tls_resource_with_cache<type> data;

        holder_tls_with_cache(int cache_millis, type default_level = enable_all) : m_level(default_level, cache_millis) {}
        bool is_enabled(type test_level) const { 
            data::read cur_level(m_level);
            return test_level >= cur_level.use(); 
        }
        void set_enabled(type level) {
            data::write cur_level(m_level);
            cur_level.use() = level;
        }
    private:
        data m_level;
    };

    /** 
        @brief Filter - holds the level - and tells you at compile time if a filter is enabled or not.

        Fix (compile time) holder
    */
    template<int fix_level = debug> struct holder_compile_time {
        static bool is_enabled(type level) { 
            return fix_level >= level;
        }
    };

} // namespace level

/** 
    @brief It's a filter that enables holding a level

    Allows managing whether a level is enabled or not (so that a logggers that wants to use levels,
    can determine if it's enabled or not)

    Example:

@code
typedef process_msg< gather::ostream_like::return_str<>, write_to_file> processor;
level::holder_no_ts level_holder;

typedef logger<processor, filter_level<level::holder_no_ts, level::debug> > debug_logger;
typedef logger<processor, filter_level<level::holder_no_ts, level::error> > error_logger;
typedef logger<processor, filter_level<level::holder_no_ts, level::info> > info_logger;

debug_logger g_log_dbg( init_both, "dbg.txt", &level_holder );
error_logger g_log_err( init_both, "err.txt", &level_holder );
info_logger g_log_app( init_both, "out.txt", &level_holder );
#define LAPP_ if ( !g_log_app) ; else g_log_app->read_msg().gather().out()
#define LERR_ if ( !g_log_err) ; else g_log_err->read_msg().gather().out()
#define LDBG_ if ( !g_log_dbg) ; else g_log_dbg->read_msg().gather().out()


// usage
LAPP_ << "info at : " << idx << " : reading word " << word;
LDBG_ << "debug at: " << idx << ", reading " << word;
LERR_ << "error at: " << idx << ", while reading " << word;

@endcode

    @sa level::holder_no_ts, level::holder_ts, level::holder_tls_with_cache
*/
template<class holder_type, int level> struct filter_level {
    filter_level(holder_type * level_holder) : m_level_holder(*level_holder) {}
    bool is_enabled() const {
        return m_level_holder.is_enabled(level);
    }
private:
    holder_type & m_level_holder;
};

}}




#endif

