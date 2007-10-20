// filter.hpp

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


#ifndef JT28092007_filter_HPP_DEFINED
#define JT28092007_filter_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/forward_constructor.hpp>

namespace boost { 

/** 
    @brief Root namespace. All the logging lib is contained in this namespace, or sub-namespaces of this one.
*/
namespace logging {


/**
    @brief Contains filter implementations. A filter tells the logger if it's enabled or not.


    The %filter namespace contains a few implementations of %filter classes.

    @c Filter is just a concept. You decide what a @c filter is.

    The minimalistic filter knows only if <i>it's enabled</i>

    Filter interface:
    @code
    struct some_filter class {
        // is filter enabled
        bool is_enabled() ;

        // ... whatever else you might want
    };
    @endcode

    In your logger, you can use any filter class that's already here, or implement your own. Implementing a filter is usually as easy as it gets:

    @code
    struct filter_no_ts {
        filter_no_ts() : m_enabled(true) {}

        bool is_enabled() const { return m_enabled; }
        void set_enabled(bool enabled) { m_enabled = enabled; }
    private:
        bool m_enabled;
    };
    @endcode

    The filters defined by the library are:
    - filter::no_ts
    - filter::ts
    - filter::use_tls_with_cache
    - filter::always_enabled
    - filter::always_disabled
    - filter::debug_enabled
    - filter::release_enabled
    - filter_level (when you use levels)

*/
namespace filter {


/** 
    Manages is_enabled/set_enabled in a non-thread-safe way (usually, this is the default filter - unless you override it). 

    If you change set_enabled() while program is running, it can take a bit to propagate
    between threads. Most of the time, this should be acceptable.
*/
struct no_ts {
    no_ts() : m_enabled(true) {}
    bool is_enabled() const { return m_enabled; }
    void set_enabled(bool enabled) { m_enabled = enabled; }
private:
    bool m_enabled;
};


/** 
    Filter that is always enabled
*/
struct always_enabled {
    static bool is_enabled() { return true; }
};


/** 
    Filter that is always disabled
*/
struct always_disabled {
    static bool is_enabled() { return false; }
};


/** 
    Filter that is enabled in debug mode
*/
struct debug_enabled {
#ifndef NDEBUG
    static bool is_enabled() { return true; }
#else
    static bool is_enabled() { return false; }
#endif
};


/** 
    Filter that is enabled in release mode
*/
struct release_enabled {
#ifdef NDEBUG
    static bool is_enabled() { return true; }
#else
    static bool is_enabled() { return false; }
#endif
};


/** 
    Thread-safe filter. Manages is_enabled/set_enabled in a thread-safe way. 
    However, it manages it rather ineffiently - always locking before asking.
*/
struct ts {
    ts() : m_enabled(true) {}
    bool is_enabled() const { 
        threading::scoped_lock lk(m_cs);
        return m_enabled; 
    }
    void set_enabled(bool enabled) { 
        threading::scoped_lock lk(m_cs);
        m_enabled = enabled; 
    }
private:
    bool m_enabled;
    mutable threading::mutex m_cs;
};

/** 
    Uses TLS (Thread Local Storage) to find out if a filter is enabled or not. It caches the current "is_enabled" on each thread.
    Then, at a given period, it retrieves the real "is_enabled".

    @remarks

    Another implementation can be done, which could be faster - where you retrieve the "is_enabled" each X calls on a given thread
    (like, every 20 calls on a given thread)
*/
struct use_tls_with_cache {
    typedef locker::tls_resource_with_cache<bool> data;

    use_tls_with_cache(int cache_millis) : m_enabled(true, cache_millis) {}
    bool is_enabled() const { 
        data::read enabled(m_enabled);
        return enabled.use(); 
    }
    void set_enabled(bool enabled) { 
        data::write cur(m_enabled);
        cur.use() = enabled; 
    }
private:
    data m_enabled;
};

} // namespace filter


    /** 
        @brief Base class for all loggers
    */
    template<class process_msg_type_ > struct logger_base {
        typedef process_msg_type_ process_msg_type;

        logger_base() {}

        BOOST_LOGGING_FORWARD_CONSTRUCTOR(logger_base, m_processor)

        /** 
            The only way to get to the message processor. Allow writing  <tt>logger->some_method(x);</tt>
        */
        const process_msg_type* operator->() const  { return &m_processor; }
        process_msg_type* operator->()              { return &m_processor; }

        // in the future, this little class counts, since it'll expose a small virtual interface
        // to allow BOOST_DECLARE_LOG/BOOST_DEFINE_LOG
        //
        // it also counts since it's the same interface as log_keeper
    private:
        process_msg_type m_processor;
    };

    /** 
    @brief The logger class. Every log from your application is an instance of this.

    The logger has a message processor, which will process the message if <i>you decide</i> the log is enabled
    (@ref workflow "see workflow")

    The logger derives from the filter, so you can manipulate it as a %filter (enable/disable it, if the %filter allows it)
    In addition to that, it implements the bool() and ! (not) operator, which just return whether the log is enabled or not:
    
    @code
    logger<write_to_cout> g_log;

    // usage
    if ( g_log) do_something(g_log);
    if ( !g_log) ; else do_something(g_log);
    @endcode

    The logger owns an instance of @c process_msg_type, and allows access to it through @c process_msg function, or through @c operator->

    @code
    logger<write_to_cout> g_log;
    g_log.process_msg().some_method(...);
    g_log->some_method(...)
    @endcode

    By default, when constructed, the logger forwards all arguments to the process_msg instance. However, you can initialize both
    the filter and the process_msg at the same time, like this:

    @code
    logger<...> l(init_both, param_to_initialize_process, param_to_initialize_filter);
    @endcode

    The logger always forwards all message processing to the @c process_msg_type class (see process_msg). Basically, usage through @ref macros "macros" is:

    @code
    logger<...> g_log;

    #define L_ if ( !g_log) ; else g_log.process_msg().do_processing...;
    // OR
    #define L_ if ( !g_log) ; else g_log->do_processing...;

    // usage
    L_ << "cool " << "message";
    @endcode





    @note
    You might think that the logical place for the filter would be in the logger (that is, the logger should own the filter). Well, this is how
    the interface was at the beginning. However, in practice, it often happens that the same filter be shared be different loggers.
    So it's best to just have the filter and logger concepts completely separated.

    FIXME explain how filters work!


    \n\n

    @param process_msg_type The message processor. It can be any class that will process the message.
                            How the class does the actual processing is up to you - also, based on that you'll @ref macros "define your macro(s)". 
                            For non-trivial cases, it should be the process_msg class.    

    */
    template<class process_msg_type_ > struct logger : logger_base<process_msg_type_> {
        typedef logger_base<process_msg_type_> logger_base_type;
        logger() {}
        BOOST_LOGGING_FORWARD_CONSTRUCTOR(logger, logger_base_type)
    };



}}


#endif

