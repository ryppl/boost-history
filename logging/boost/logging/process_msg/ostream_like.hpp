// ostream_like.hpp

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


#ifndef JT28092007_ostream_like_HPP_DEFINED
#define JT28092007_ostream_like_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

#include <iostream>
#include <sstream>

namespace boost { namespace logging {

namespace optimize {
    template<class char_type_ > struct cache_string_one_str ;
}

/** 
    @brief Classes that implement gathering the message

    A class that implements gathering the message needs 2 things:
    - a function that will gather the data - give it any name you wish, and call that in your @ref macros "macro"
    - define a function called <tt>.msg()</tt> that will return the gathered data.

    The gatherer class is used by the process_msg class. When doing %logging, it's usually used like this:

    @code
    logger< process_msg<some_gather_class, some_writer_class> ... > g_log;

    #define L_ if ( !g_log) ; else g_log->read_msg().gather().gatherer_function...

    // usage
    L_ << "cool " << "message";
    @endcode

    Implementing that @c gatherer_function is rather easy, here's a simple example:

    @code
    struct return_str {
        typedef std::basic_ostringstream<char_type> stream_type;

        stream_type & out() { return m_out; }
        std::basic_string<char_type> msg() { return m_out.str(); }
    private:
        stream_type m_out;
    };
    @endcode

    In the above case, defining the @ref macros "macro" is easy:

    @code
    #define L_ if ( !g_log) ; else g_log->read_msg().gather().out()

    // usage
    L_ << "cool " << "message";
    @endcode

    @sa gather::ostream_like
*/
namespace gather {


/** 
    @brief Allows you to write to a log using the cool "<<" operator.

    The <tt>.msg()</tt> function returns the gathered message.

    Example:
    @code
    struct dump_to_cout {
        void operator()(const std::stringstream & out) {
            cout << out.str();
        }
    };

    // define the logger
    typedef process_msg< gather::ostream_like::return_str<>, dump_to_cout> processor;
    logger<processor, filter::no_ts> g_single_log("out.txt");

    #define L_ if ( g_single_log) ; else g_single_log->read_msg().gather().msg()

    // code
    i = 100;
    L_ << "this is " << i << " times better that the average bear... ";

    @endcode

*/
namespace ostream_like {

/** 
    @brief Allows you to write to a log using the cool "<<" operator. The @c .msg() returns the stream itself.
    
    @sa ostream_like

    Note that this is a very simple class - meant only as an example.
    Just remember:
    1. you need a function that will gather the data - give it any name you wish, and call that in your macro
    2. define a function called msg() that will return the gathered data.

    In our case, 1. and 2. are the same
*/
template<class stream_type = std::basic_ostringstream<char_type> > struct return_raw_stream {
    /** 
    note: we return the whole stream - we don't return out().str() , because the user might use a better ostream class,
    which could have better access to its buffer/internals
    */
    stream_type & msg() { return m_out; }
private:
    stream_type m_out;
};


/** 
    @brief Allows you to write to a log using the cool "<<" operator. The .msg() returns a string.

    Note that this is a very simple class.

    Just remember:
    1. you need a function that will gather the data - give it any name you wish, and call that in your macro (in this case, it's called "out()")
    2. define a function called msg() that will return the gathered data.

    Usage:

    @code
    struct dump_to_cout {
        void operator()(const std::string & msg) { cout << msg; }
    };

    // define the logger
    typedef process_msg< ostream_like<>, dump_to_cout> processor;
    logger<processor, filter::no_ts> g_single_log("out.txt");

    #define L_ if ( negate_sink<processor> s = g_single_log) s->read_msg().gather().out()

    // code
    i = 100;
    L_ << "this is " << i << " times better that the average bear... ";

    @endcode


*/
template<class stream_type = std::basic_ostringstream<char_type> > struct return_str {
    stream_type & out() { return m_out; }
    std::basic_string<char_type> msg() { return m_out.str(); }
private:
    stream_type m_out;
};



/** 
    @brief Allows you to write to a log using the cool "<<" operator. The .msg() returns a @ref boost::logging::optimize::cache_string_one_str "cache_string".

    @sa ostream_like

    returns a cache string
*/
template<class cache_string = boost::logging::optimize::cache_string_one_str<> , int prepend_size = 10, int append_size = 10, class stream_type = std::basic_ostringstream<char_type> > struct return_cache_str {
    stream_type & out() { return m_out; }
    cache_string msg() { return cache_string( m_out.str(), prepend_size, append_size ); }
private:
    stream_type m_out;
};




}}}}

#endif

