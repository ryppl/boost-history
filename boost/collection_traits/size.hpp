// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_CONTAINER_TRAITS_SIZE_HPP
#define BOOST_CONTAINER_TRAITS_SIZE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/collection_traits/config.hpp>

#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include <boost/collection_traits/detail/size.hpp>
#else

#include <boost/collection_traits/detail/implementation_help.hpp>
#include <cstddef>
#include <iterator>
#include <utility>

namespace boost { 
namespace collection_traits 
{

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template< typename C >
        inline BOOST_CT_DEDUCED_TYPENAME C::size_type
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
            return collection_traits_detail::array_size( array ); 
        }
        
        template< typename T, std::size_t sz >
        inline std::size_t size( T (&array)[sz] )
        {
            return collection_traits_detail::array_size( array );
        }
        
        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////

        inline std::size_t size( const char* s )
        {
            return boost::collection_traits_detail::str_size( s );
        }

        inline std::size_t size( const wchar_t* s )
        {
            return boost::collection_traits_detail::str_size( s );
        }
        
} // namespace 'collection_traits'

using collection_traits::size;

} // namespace 'boost'

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#endif
