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

/** @page gather_the_message Gathering the message

A class that implements gathering the message needs 2 things:
- a function that will gather the data - called <tt>.out()</tt>
- define a function called <tt>.msg()</tt> that will return the gathered data (once all data has been gathered).


*/

namespace optimize {
    template<class char_type_ > struct cache_string_one_str ;
}

/** 
    @brief Classes that implement gathering the message

    @copydoc gather_the_message

    Implementing a gather class is rather easy, here's a simple example:

    @code
    struct return_str {
        typedef std::basic_ostringstream<char_type> stream_type;

        stream_type & out() { return m_out; }
        std::basic_string<char_type> msg() { return m_out.str(); }
    private:
        stream_type m_out;
    };
    @endcode

    @sa gather::ostream_like
*/
namespace gather {


/** 
    @brief Gathering the message: Allows you to write to a log using the cool "<<" operator.

    The <tt>.msg()</tt> function returns the gathered message.

    @copydoc gather_the_message

*/
namespace ostream_like {

/** 
    @brief Allows you to write to a log using the cool "<<" operator. The @c .msg() returns the stream itself.
    
    Note that this is a very simple class - meant only as an example.

    @copydoc gather_the_message

    See also:
    - boost::logging::gather
    - ostream_like

*/
template<class stream_type = std::basic_ostringstream<char_type> > struct return_raw_stream {
    // what does the gather_msg class return?
    typedef stream_type& param;

    return_raw_stream() {}
    return_raw_stream(const return_raw_stream& other) : m_out( other.m_out.str() ) {}

    /** 
    note: we return the whole stream - we don't return out().str() , because the user might use a better ostream class,
    which could have better access to its buffer/internals
    */
    stream_type & msg() { return m_out; }
    stream_type & out() { return m_out; }
private:
    stream_type m_out;
};


/** 
    @brief Allows you to write to a log using the cool "<<" operator. The .msg() returns a string.

    Note that this is a very simple class.

    @copydoc gather_the_message

    See also:
    - boost::logging::gather
    - ostream_like

*/
template<class stream_type = std::basic_ostringstream<char_type> > struct return_str {
    // what does the gather_msg class return?
    typedef std::basic_string<char_type> & param;

    return_str() {}
    return_str(const return_str& other) : m_out(other.m_out.str()) {}

    stream_type & out() { return m_out; }
    /** returns a string */
    std::basic_string<char_type> msg() { return m_out.str(); }
private:
    stream_type m_out;
};



/** 
    @brief Allows you to write to a log using the cool "<<" operator. The .msg() returns a @ref boost::logging::optimize::cache_string_one_str "cache_string".

    Returns a cache string

    @copydoc gather_the_message

    See also:
    - boost::logging::gather
    - ostream_like


    @bug right now prepend_size and append_size are ignored; because we can also return a cache_string_several_str<>. When fixing, watch the find_gather class!
*/
template<
        class cache_string = boost::logging::optimize::cache_string_one_str<hold_string_type> , 
        class stream_type = std::basic_ostringstream<char_type> > struct return_cache_str {

    // what does the gather_msg class return?
    typedef cache_string & param;
    
    return_cache_str() {}
    return_cache_str(const return_cache_str& other) : m_out(other.m_out.str()) {}

    stream_type & out() { return m_out; }
    /** returns a cache_string */
    cache_string msg() { return cache_string( m_out.str() ); }
private:
    stream_type m_out;
};




}}}}

#endif

