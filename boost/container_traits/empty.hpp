// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_EMPTY_HPP
#define BOOST_CONTAINER_TRAITS_EMPTY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <cstddef>
#include <iterator>
#include <utility>

namespace boost 
{
        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////

        template< typename C >
        inline bool 
        empty( const C& c )
        {
            return c.empty();
        }

        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

        template< typename Iterator >
        inline bool empty( const std::pair<Iterator,Iterator>& p )
        {
            return p.first == p.second;
        }

        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////

        template< typename T, std::size_t sz >
        inline bool empty( const T (&array)[sz] )
        {
            return sz == 0;
        }

        template< typename T, std::size_t sz >
        inline bool empty( T (&array)[sz] )
        {
            return sz == 0;
        }

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////

        inline bool empty( const char* s )
        {
            return s == 0 || s[0] == 0;
        }

        inline bool empty( const wchar_t* s )
        {
            return s == 0 || s[0] == 0;
        }
        
        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template< typename C, typename T, typename D, typename P >
        inline bool
        empty( const std::istream_iterator<C,T,D,P>& i )
        {
            return i == std::istream_iterator<C,T,D,P>();
        }

} // namepace 'boost'

#endif
