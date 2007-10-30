// on_dedicated_thread.hpp

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


#ifndef JT28092007_on_dedicated_thread_HPP_DEFINED
#define JT28092007_on_dedicated_thread_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/forward_constructor.hpp>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/bind.hpp>
#include <boost/logging/detail/manipulator.hpp> // boost::logging::manipulator

namespace boost { namespace logging { namespace writer {

namespace detail {
    template<class msg_type> struct dedicated_context {
        dedicated_context() : is_working(true), write_period_ms(100) {}

        bool is_working;
        int write_period_ms;

        boost::logging::threading::mutex cs;

        // the thread doing the write
        boost::shared_ptr<boost::thread> writer;

        // ... so that reallocations are fast
        typedef boost::shared_ptr<msg_type> ptr;
        typedef std::vector<ptr> array;
        array msgs;
    };
}

/** 
@brief Performs all writes on a dedicated thread  - very efficient and <b>thread-safe</b>. 

<tt>\#include <boost/logging/writer/on_dedidcated_thread.hpp> </tt>

Keeps locks in the worker threads to a minimum:
whenever a message is logged, is put into a queue (this is how long the lock lasts).
Then, a dedicated thread reads the queue, and processes it one by one.

To transform a writer into on-dedicated-thread thread-safe writer, simply surround the writer with on_dedicated_thread:

Example:

@code
typedef gather::ostream_like::return_str<> string;

// not thread-safe
process_msg< string, write_to_cout> g_l;

// thread-safe, on dedicated thread
process_msg< string, on_dedicated_thread<string,write_to_cout> > g_l;


// not thread-safe
process_msg< 
    string , 
    format_write< 
        format_base, 
        destination_base, format_and_write::simple<string> > > g_l;

// thread-safe, on dedicated thread
process_msg< 
    string , 
    on_dedicated_thread<string, format_write< 
        format_base, 
        destination_base, format_and_write::simple<string> > > > g_l;
@endcode

*/
template<class msg_type, class base_type> 
struct on_dedicated_thread 
        : base_type, 
          boost::logging::manipulator::non_const_context<detail::dedicated_context<msg_type> > {

    typedef on_dedicated_thread<msg_type,base_type> self_type;
    typedef detail::dedicated_context<msg_type> context_type;

    typedef boost::logging::threading::mutex::scoped_lock scoped_lock;

    on_dedicated_thread() {}
    BOOST_LOGGING_FORWARD_CONSTRUCTOR(on_dedicated_thread,base_type)

    void write_period(int period_ms) {
        scoped_lock lk( context_type::context().cs);
        context_type::context().write_period_ms = period_ms;
    }

    ~on_dedicated_thread() {
        boost::shared_ptr<boost::thread> writer;
        { scoped_lock lk( context_type::context().cs);
          context_type::context().is_working = false;
          writer = context_type::context().writer;
        }

        if ( writer)
            writer->join();
    }

    void operator()(const msg_type & msg) {
        typedef typename context_type::ptr ptr;
        ptr new_msg(new msg_type(msg));

        scoped_lock lk( context_type::context().cs);
        if ( !context_type::context().writer) 
            context_type::context().writer = ptr( new boost::thread( boost::bind(&self_type::do_write,this) ));

        context_type::context().msgs.push_back(new_msg);
    }
private:
    void do_write() {
        const int NANOSECONDS_PER_SECOND = 1000 * 1000 * 1000;

        int sleep_ms = 0;
        while ( true) {
            { scoped_lock lk( context_type::context().cs);
              // refresh it - just in case it got changed...
              sleep_ms = context_type::context().write_period_ms;
              if ( !context_type::context().is_working)
                  break; // we've been destroyed
            }

            boost::xtime to_wait;
            xtime_get(&to_wait, boost::TIME_UTC);
            to_wait.sec += sleep_ms / 1000;
            to_wait.nsec += (sleep_ms % 1000) * (NANOSECONDS_PER_SECOND / 1000);
            to_wait.sec += to_wait.nsec / NANOSECONDS_PER_SECOND ;
            to_wait.nsec %= NANOSECONDS_PER_SECOND ;
            boost::thread::sleep( to_wait);

            typedef typename context_type::array array;
            array msgs;
            { scoped_lock lk( context_type::context().cs);
              std::swap( context_type::context().msgs, msgs);
              // reserve elements - so that we don't get automatically resized often
              context_type::context().msgs.reserve( msgs.size() );
            }

            for ( typename array::iterator b = msgs.begin(), e = msgs.end(); b != e; ++b)
                base_type::operator()(*(b->get()));
        }
    }
};


}}}

#endif

