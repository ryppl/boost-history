//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  See http://www.boost.org for updates, documentation, and revision 

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_EMPTY_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_EMPTY_HPP

#include <boost/collection_traits/detail/common.hpp>

namespace boost 
{
    namespace collection_traits_detail
    {
        template< typename T >
        struct collection_empty;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_empty<std_container_>
        {
            template< typename C >
            static bool fun( C& c )
            {
                return c.empty();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_empty<std_pair_>
        {
            template< typename P >
            static bool fun( const P& p )
            {
                return p.first == p.second;
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_empty<array_>
        {
            template< typename T, std::size_t sz >
            static bool fun( T BOOST_ARRAY_REF[sz] )
            {
                if( array == 0 )
                    return true;
                return false;
            }
        };

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_empty<char_ptr_>
        {
            static bool fun( const char* s )
            {
                return s == 0 || s[0] == 0;
            }
        };

        template<>
        struct collection_empty<const_char_ptr_>
        {
            static bool fun( const char* s )
            {
                return  s == 0 || s[0] == 0;
            }
        };

        template<>
        struct collection_empty<wchar_t_ptr_>
        {
            static bool fun( const wchar_t* s )
            {
                return  s == 0 || s[0] == 0;
            }
        };
        
        template<>
        struct collection_empty<const_wchar_t_ptr_>
        {
            static bool fun( const wchar_t* s )
            {
                return  s == 0 || s[0] == 0;
            }
        };

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_empty<iterator_>
        {
            template< typename I >
            static bool fun( I& i )
            {
                return i == I();
            }
        };

        } // namespace 'collection_traits_detail'
    
    namespace collection_traits
    {
        
        template< typename C >
        inline bool 
        empty( const C& c )
        {
            return collection_traits_detail::collection_empty<  BOOST_CT_DEDUCED_TYPENAME collection_traits_detail::collection<C>::type >::fun( c );
        }
    }
    
    using collection_traits::empty;

} // namespace 'boost'


#endif
