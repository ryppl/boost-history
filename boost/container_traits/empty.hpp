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

#include <boost/container_traits/config.hpp>
#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include <boost/container_traits/detail/empty.hpp>
#else

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
        inline bool empty( const T BOOST_ARRAY_REF[sz] )
        {
            return sz == 0;
        }

        template< typename T, std::size_t sz >
        inline bool empty( T BOOST_ARRAY_REF[sz] )
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

#ifdef BOOST_MSVC_STD_ITERATOR

        template< typename C, typename T, typename D >
        inline bool
        empty( const std::istream_iterator<C,T,D>& i )
        {
            return i == std::istream_iterator<C,T,D>();
        }

#else
	    template< typename C, typename T, typename D, typename P >
        inline bool
        empty( const std::istream_iterator<C,T,D,P>& i )
        {
            return i == std::istream_iterator<C,T,D,P>();
        }

#endif

} // namepace 'boost'

#endif //  BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#endif
