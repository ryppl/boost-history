//-----------------------------------------------------------------------------
// boost detail/templated_streams.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_DETAIL_TEMPLATED_STREAMS_HPP
#define BOOST_DETAIL_TEMPLATED_STREAMS_HPP

#include "boost/config.hpp"

// add this to Boost.Config, but for now:
#if !defined(BOOST_NO_TEMPLATED_STREAMS)

#if (defined(__GNUC__) && __GNUC__ == 2 && __GNUC_MINOR__ <= 97 && !defined(__SGI_STL_PORT)) \
 || (defined(BOOST_MSVC) && BOOST_MSVC <= 1200 && defined(__SGI_STL_PORT))
#   define BOOST_NO_TEMPLATED_STREAMS
#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// (detail) BOOST_TEMPLATED_STREAM_* macros
//
// Provides workaround platforms without stream class templates.
//

#if !defined(BOOST_NO_TEMPLATED_STREAMS)

#define BOOST_TEMPLATED_STREAM_TEMPLATE(E,T) \
    template < typename E , typename T >

#define BOOST_TEMPLATED_STREAM_TEMPLATE_ALLOC(E,T,A) \
    template < typename E , typename T , typename A >

#define BOOST_TEMPLATED_STREAM_ARGS(E,T) \
    typename E , typename T 

#define BOOST_TEMPLATED_STREAM_ARGS_ALLOC(E,T,A) \
    typename E , typename T , typename A 

#define BOOST_TEMPLATED_STREAM_COMMA        ,

#define BOOST_TEMPLATED_STREAM_ELEM(E)      E
#define BOOST_TEMPLATED_STREAM_TRAITS(T)    T
#define BOOST_TEMPLATED_STREAM_ALLOC(A)     A

#define BOOST_TEMPLATED_STREAM(X,E,T) \
    BOOST_JOIN(std::basic_,X)< E , T >

#define BOOST_TEMPLATED_STREAM_WITH_ALLOC(X,E,T,A) \
    BOOST_JOIN(std::basic_,X)< E , T , A >

#else // defined(BOOST_NO_TEMPLATED_STREAMS)

#define BOOST_TEMPLATED_STREAM_TEMPLATE(E,T) /**/

#define BOOST_TEMPLATED_STREAM_TEMPLATE_ALLOC(E,T,A) /**/

#define BOOST_TEMPLATED_STREAM_ARGS(E,T) /**/

#define BOOST_TEMPLATED_STREAM_ARGS_ALLOC(E,T,A) /**/

#define BOOST_TEMPLATED_STREAM_COMMA        /**/

#define BOOST_TEMPLATED_STREAM_ELEM(E)      char
#define BOOST_TEMPLATED_STREAM_TRAITS(T)    std::char_traits<char>
#define BOOST_TEMPLATED_STREAM_ALLOC(A)     std::allocator<char>

#define BOOST_TEMPLATED_STREAM(X,E,T) \
    std::X

#define BOOST_TEMPLATED_STREAM_WITH_ALLOC(X,E,T,A) \
    std::X

#endif // BOOST_NO_TEMPLATED_STREAMS

#endif // BOOST_DETAIL_TEMPLATED_STREAMS_HPP
