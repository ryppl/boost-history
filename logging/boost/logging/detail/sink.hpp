// sink.hpp

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

/*  FIXME this whole file will most likely dissapear

*/

#ifndef JT28092007_sink_HPP_DEFINED
#define JT28092007_sink_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {



    /*
        The sink

        Workflow - we have a sink object, which asks the logger if it's enabled     
        If the logger is enabled, it also returns the message processor.

        Thus, you can write code like this:
        if ( sink<some_type> s = find_logger(some_logger) )
            s.process_msg( ... );

        Note that this is useful only if you have a find_logger function which doesn't always return the same logger
        (thus, you need to cached the result).

        Otherwise, the logger already has helper funcs, which allow you to do this:

        if ( logger) logger.process_msg()....
    */
    template<class process_msg_type> struct sink {
        template<class logger> sink(const logger & f) {
            p = f.is_enabled() ? &f.process_msg() : 0;
        }
        operator bool() const           { return p != 0; }
        const process_msg_type & process_msg() { return *p; }
        /** 
            Syntactic sugar. Allow writing  
            if ( sink s = dbg_filter) s->some_func(x);
        */
        const process_msg_type* operator->() { return p; }
    protected:
        const process_msg_type * p;
    };

    /** 
        This negates the sink, allowing you to create LOG macros. That is, it allows for this construct:

        if ( negate_sink<some_type> s = find_logger(some_filter) )
            ;
        else
            s.process_msg() ...;

        @sa sink
    */
    template<class process_msg_type> struct negate_sink : sink<process_msg_type> {
        typedef sink<process_msg_type> base_type;
        template<class logger> negate_sink(const logger & f) : base_type(f) {}
        operator bool() const { return base_type::p == 0; }
    };

}}


#endif

