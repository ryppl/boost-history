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

#include <boost/container_traits/config.hpp>
#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include <boost/container_traits/detail/end.hpp>
#else

#include <boost/container_traits/iterator.hpp>
#include <boost/container_traits/const_iterator.hpp>
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
        inline BOOST_DEDUCED_TYPENAME container_const_iterator<C>::type
        end( const C& c )
        {
            return c.end();
        }
                
        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_iterator<C>::type
        end( C& c )
        {
            return c.end();
        }
      
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

        template< typename Iterator >
        inline Iterator end( const std::pair<Iterator,Iterator>& p )
        {
            return p.second;
        }
        
        template< typename Iterator >
        inline Iterator end( std::pair<Iterator,Iterator>& p )
        {
            return p.second;
        }
        
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////

        template< typename T, std::size_t sz >
        inline const T* end( const T BOOST_ARRAY_REF[sz] )
        {
            return array + sz;
        }
        
        template< typename T, std::size_t sz >
        inline T* end( T BOOST_ARRAY_REF[sz] )
        {
            return array + sz;
        }

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
        
        //
        // @note: borland workaround
        //
#define BOOST_END_IMPL( T )                        \
    if( s == 0 || s[0] == 0 )                      \
        return s;                                  \
    return s + std::char_traits<T>::length( s ) 

        inline char* end( char* s )
        {
            BOOST_END_IMPL( char );
        }

        inline wchar_t* end( wchar_t* s )
        {
            BOOST_END_IMPL( wchar_t );
        }

        inline const char* end( const char* s )
        {
            BOOST_END_IMPL( char );
        }

        inline const wchar_t* end( const wchar_t* s )
        {
            BOOST_END_IMPL( wchar_t );
        }
        
#undef BOOST_END_IMPL

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////

        template< typename C, typename T, typename D, typename P >
        inline std::istream_iterator<C,T,D,P>
        end( const std::istream_iterator<C,T,D,P>& i )
        {
            return std::istream_iterator<C,T,D,P>();
        }
	
#ifdef BOOST_MSVC_STD_ITERATOR

        template< typename C, typename T, typename D >
        inline std::istream_iterator<C,T,D>
        end( std::istream_iterator<C,T,D>& i )
        {
            return std::istream_iterator<C,T,D>();
        }

#else // BOOST_MSVC_STD_ITERATOR

        template< typename C, typename T, typename D, typename P >
        inline std::istream_iterator<C,T,D,P>
        end( std::istream_iterator<C,T,D,P>& i )
        {
            return std::istream_iterator<C,T,D,P>();
        }

#endif // BOOST_MSVC_STD_ITERATOR	

} // namespace 'boost'

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#endif
