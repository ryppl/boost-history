// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_CONTAINER_TRAITS_END_HPP
#define BOOST_CONTAINER_TRAITS_END_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/collection_traits/config.hpp>

#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include <boost/collection_traits/detail/end.hpp>
#else

#include <boost/collection_traits/detail/implementation_help.hpp>
#include <boost/collection_traits/iterator.hpp>
#include <boost/collection_traits/const_iterator.hpp>

namespace boost 
{ 
namespace collection_traits
{

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template< typename C >
        inline BOOST_DEDUCED_TYPENAME const_iterator_of<C>::type
        end( const C& c )
        {
            return c.end();
        }
                
        template< typename C >
        inline BOOST_DEDUCED_TYPENAME iterator_of<C>::type
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
        
        inline char* end( char* s )
        {
            return boost::collection_traits_detail::str_end( s );
        }

        inline wchar_t* end( wchar_t* s )
        {
            return boost::collection_traits_detail::str_end( s );
        }

        inline const char* end( const char* s )
        {
            return boost::collection_traits_detail::str_end( s );
        }

        inline const wchar_t* end( const wchar_t* s )
        {
            return boost::collection_traits_detail::str_end( s );
        }
        
} // namespace 'collection_traits'

using collection_traits::end;

} // namespace 'boost'

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#endif
