// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_SIZE_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_SIZE_HPP

#include <boost/collection_traits/detail/implementation_help.hpp>
#include <boost/collection_traits/detail/size_type.hpp>
#include <boost/collection_traits/detail/common.hpp>
#include <iterator>

namespace boost 
{
    namespace collection_traits_detail
    {
        template< typename T >
        struct collection_size;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_size<std_container_>
        {
            template< typename C >
            static BOOST_CT_DEDUCED_TYPENAME C::size_type fun( const C& c )
            {
                return c.size();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_size<std_pair_>
        {
            template< typename P >
            static BOOST_CT_DEDUCED_TYPENAME size_type_of<P>::type fun( const P& p )
            {
                return std::distance( p.first, p.second );
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_size<array_>
        {
            template< typename T, std::size_t sz >
            static std::size_t fun( T BOOST_ARRAY_REF[sz] )
            {
                return sz;
            }
        };
        
        template<>
        struct collection_size<char_array_>
        {
            template< typename T, std::size_t sz >
            static std::size_t fun( T BOOST_ARRAY_REF[sz] )
            {
                return boost::collection_traits_detail::array_size( array );
            }
        };
        
        template<>
        struct collection_size<wchar_t_array_>
        {
            template< typename T, std::size_t sz >
            static std::size_t fun( T BOOST_ARRAY_REF[sz] )
            {
                return boost::collection_traits_detail::array_size( array );
            }
        };

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////

        template<>
        struct collection_size<char_ptr_>
        {
            static std::size_t fun( const char* s )
            {
                return boost::collection_traits_detail::str_size( s );
            }
        };

        template<>
        struct collection_size<const_char_ptr_>
        {
            static std::size_t fun( const char* s )
            {
                return boost::collection_traits_detail::str_size( s );
            }
        };
        
        template<>
        struct collection_size<wchar_t_ptr_>
        {
            static std::size_t fun( const wchar_t* s )
            {
                return boost::collection_traits_detail::str_size( s );
            }
        };

        template<>
        struct collection_size<const_wchar_t_ptr_>
        {
            static std::size_t fun( const wchar_t* s )
            {
                return boost::collection_traits_detail::str_size( s );
            }
        };
  
        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_size<iterator_>; // not defined

        } // namespace 'collection_traits_detail'
    
    namespace collection_traits
    {
    
        template< typename C >
        BOOST_CT_DEDUCED_TYPENAME size_type_of<C>::type 
        size( const C& c )
        {
            return collection_traits_detail::collection_size<  BOOST_CT_DEDUCED_TYPENAME collection_traits_detail::collection<C>::type >::fun( c );
        }
    }
    
    using collection_traits::size;
    
} // namespace 'boost'


#endif
