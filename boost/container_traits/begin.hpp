// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_BEGIN_HPP
#define BOOST_CONTAINER_TRAITS_BEGIN_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/container_traits/config.hpp>
#include <boost/container_traits/result_iterator.hpp>
#include <cstddef>
#include <iterator>
#include <utility>

namespace boost 
{

//////////////////////////////////////////////////////////////////////
// default
//////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< typename C >
inline BOOST_DEDUCED_TYPENAME C::const_iterator
begin( const C& c )
{
    return c.begin(); 
}

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< typename C >
inline BOOST_DEDUCED_TYPENAME container_result_iterator<C>::type
begin( C& c )
{
    return c.begin(); 
}

//////////////////////////////////////////////////////////////////////
// pair
//////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< typename Iterator >
inline Iterator begin( const std::pair<Iterator,Iterator>& p )
{
    return p.first;
}

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< typename Iterator >
inline Iterator begin( std::pair<Iterator,Iterator>& p )
{
    return p.first;
}

//////////////////////////////////////////////////////////////////////
// array
//////////////////////////////////////////////////////////////////////

template< typename T, std::size_t sz >
inline const T* begin( const T (&array)[sz] )
{
    return array;
}

template< typename T, std::size_t sz >
inline T* begin( T (&array)[sz] )
{
    return array;
}

//////////////////////////////////////////////////////////////////////
// string
//////////////////////////////////////////////////////////////////////

inline const char* begin( const char* s )
{
    return s;
}

inline char* begin( char* s )
{
    return s;
}

inline const wchar_t* begin( const wchar_t* s )
{
    return s;
}

inline wchar_t* begin( wchar_t* s )
{
    return s;
}

//////////////////////////////////////////////////////////////////////
// iterator
//////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< typename C, typename T, typename D, typename P >
inline std::istream_iterator<C,T,D,P>
begin( const std::istream_iterator<C,T,D,P>& i )
{
    return i;
}

#endif

template< typename C, typename T, typename D, typename P >
inline std::istream_iterator<C,T,D,P>
begin( std::istream_iterator<C,T,D,P>& i )
{
    return i;
}

} // namespace boost

#endif
