// process_msg.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#ifndef JT28092007_process_msg_HPP_DEFINED
#define JT28092007_process_msg_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/forward_constructor.hpp>

namespace boost { namespace logging {


    
    template<class holder, class gather> struct gather_holder {
        typedef gather gather_type;
        gather_holder(const holder & p_this) : m_this(p_this) {}
        ~gather_holder() { 
            // FIXME handle exiting from exceptions!!!
            
            m_this.on_do_write(m_obj); 
        }
        gather_type & gather() { return m_obj; }
    private:
        const holder & m_this;
        mutable gather_type m_obj;
    };

    namespace detail {
        template<class type> type& as_non_const(const type & t) { return const_cast<type&>(t); }
    }

    /** 
    @brief Processes a message (see @ref workflow_processing "workflow")

    As described in @ref workflow_processing "workflow", processing the message is composed of 2 things:
    - @ref workflow_2a "Gathering the message" 
    - @ref workflow_2b "Processing the message"

    The process_msg class has 2 template parameters:


    @param gather_msg A new gather instance is created each time a message is written. 
    The @c gather_msg class needs to be default-constructible.
    The @c gather_msg must have a function called @c .msg() which contains all information about the written message.
    It will be passed to the write_msg class.
    You can implement your own @c gather_msg class, or use any from the gather namespace.


    @param write_msg This is the object that does the @ref workflow_2b "second step" - the writing of the message.
    It can be a simple functor.
    Or, it can be a more complex object that contains logic of how the message is to be further formatted,
    and written to multiple destinations. 
    You can implement your own @c write_msg class, or it can be any of the classes defined in writer namespace.
    Check out writer::format_write - which allows you to use
    several formatters to further format the message, and then write it to destinations.



    \n\n    
    The logger forwards all message processing to the @c %process_msg class. The @c %process_msg class forwards
    the gathering of the message to the @c gather_msg class. Once all message is gathered, it's passed on to the writer.
    This is usually done through a @ref macros "macro".

    @code
    logger< process_msg<...> ... > g_log;

    #define L_ if ( !g_log) ; else g_log->read_msg().gather().some_function_in_the_gather_class

    // usage
    L_ << "cool " << "message";

    @endcode



    \n\n        
    To understand more on the workflow that involves process_msg:
    - check out the gather namespace
    - check out the writer namespace
    
    */
    template<class gather_msg, class write_msg> struct process_msg {
        typedef process_msg<gather_msg, write_msg> self;

        // FIXME we should have a way to specify a template gather_msg - for example useful for cache_string

        process_msg() {}
        BOOST_LOGGING_FORWARD_CONSTRUCTOR(process_msg,m_writer)

        // FIXME watch for copy-construction!
        /** 
            reads all data about a log message (gathers all the data about it)
            FIXME
        */
        gather_holder<self, gather_msg> read_msg() const { return gather_holder<self, gather_msg>(*this) ; }

        write_msg & writer()                    { return m_writer; }
        const write_msg & writer() const        { return m_writer; }

        // called after all data has been gathered
        void on_do_write(gather_msg & gather) const {
            m_writer( detail::as_non_const(gather.msg()) );
        }

    private:
        write_msg m_writer;
    };

    template<class arg_type = const char*> struct write_call_func {
        typedef void (*func)(arg_type) ;
        write_call_func(func f) : m_f(f) {}
        void operator()(arg_type a) { f(a); }
    private:
        func m_f;
    };


}}

#endif

