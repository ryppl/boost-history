// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_END_HPP
#define BOOST_CONTAINER_TRAITS_END_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/container_traits/size.hpp>
#include <boost/container_traits/result_iterator.hpp>
#include <boost/container_traits/config.hpp>
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
        end( const C& c )
        {
            return c.end();
        }
                
#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_result_iterator<C>::type
        end( C& c )
        {
            return c.end();
        }
      
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename Iterator >
        inline Iterator end( const std::pair<Iterator,Iterator>& p )
        {
            return p.second;
        }
        
#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename Iterator >
        inline Iterator end( std::pair<Iterator,Iterator>& p )
        {
            return p.second;
        }
        
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////

        template< typename T, std::size_t sz >
        inline const T* end( const T (&array)[sz] )
        {
            return array + sz;
        }
        
        template< typename T, std::size_t sz >
        inline T* end( T (&array)[sz] )
        {
            return array + sz;
        }

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////

        inline const char* end( const char* s )
        {
            return s + size( s );
        }
        
        inline char* end( char* s )
        {
            return s + size( s );
        }
        
        inline const wchar_t* end( const wchar_t* s )
        {
            return s + size( s );
        }
        
        inline wchar_t* end( wchar_t* s )
        {
            return s + size( s );
        }

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING     

        template< typename C, typename T, typename D, typename P >
        inline std::istream_iterator<C,T,D,P>
        end( const std::istream_iterator<C,T,D,P>& i )
        {
            return std::istream_iterator<C,T,D,P>();
        }
#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename C, typename T, typename D, typename P >
        inline std::istream_iterator<C,T,D,P>
        end( std::istream_iterator<C,T,D,P>& i )
        {
            return std::istream_iterator<C,T,D,P>();
        }

} // namespace 'boost'

#endif
