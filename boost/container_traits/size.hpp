// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_SIZE_HPP
#define BOOST_CONTAINER_TRAITS_SIZE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <cstddef>
#include <iterator>
#include <utility>
#include <string>

namespace boost 
{

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template< typename C >
        inline BOOST_DEDUCED_TYPENAME C::size_type
        size( const C& c )
        {
            return c.size(); 
        }

        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

        template< typename Iterator >
        inline std::size_t size( const std::pair<Iterator,Iterator>& p )
        {
            return std::distance( p.first, p.second );
        }

        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////

        template< typename T, std::size_t sz >
        inline std::size_t size( const T (&array)[sz] )
        {
            return sz;
        }
        
        template< typename T, std::size_t sz >
        inline std::size_t size( T (&array)[sz] )
        {
            return sz;
        }

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////

        inline std::size_t size( const char* s )
        {
            if ( s == 0 || s[0] == 0 )
                return 0;
            return std::char_traits<char>::length( s );
        }

        inline std::size_t size( const wchar_t* s )
        {
            if ( s == 0 || s[0] == 0 )
                return 0;
            return std::char_traits<wchar_t>::length( s );
        }
        
        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template< typename C, typename T, typename D, typename P >
        inline std::size_t size( const std::istream_iterator<C,T,D,P>& );

} // namespace 'boost'

#endif
