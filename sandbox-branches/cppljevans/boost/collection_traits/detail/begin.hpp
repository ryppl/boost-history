//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  See http://www.boost.org for updates, documentation, and revision 

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_BEGIN_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_BEGIN_HPP

#include <boost/collection_traits/detail/result_iterator.hpp>
#include <boost/collection_traits/detail/common.hpp>

namespace boost 
{
    
    namespace collection_traits_detail
    {
        template< typename T >
        struct collection_begin;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_begin<std_container_>
        {
            template< typename C >
			static BOOST_CT_DEDUCED_TYPENAME result_iterator_of<C>::type fun( C& c )
            {
                return c.begin();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_begin<std_pair_>
        {
            template< typename P >
            static BOOST_CT_DEDUCED_TYPENAME result_iterator_of<P>::type fun( const P& p )
            {
                return p.first;
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_begin<array_>
        {
            template< typename T, std::size_t sz >
            static T* fun( T BOOST_ARRAY_REF[sz] )
            {
                return array;
            }
        };

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
     
        template<>
        struct collection_begin<char_ptr_>
        {
            static char* fun( char* s )
            {
                return s;
            }
        };

        template<>
        struct collection_begin<const_char_ptr_>
        {
            static const char* fun( const char* s )
            {
                return s;
            }
        };
        
        template<>
        struct collection_begin<wchar_t_ptr_>
        {
            
            static wchar_t* fun( wchar_t* s )
            {
                return s;
            }
        };

        template<>
        struct collection_begin<const_wchar_t_ptr_>
        {
            static const wchar_t* fun( const wchar_t* s )
            {
                return s;
            }
        };

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_begin<iterator_>
        {
            template< typename I >
            static BOOST_CT_DEDUCED_TYPENAME result_iterator_of<I>::type fun( I& i )
            {
                return i;
            }
        };

        } // namespace 'collection_traits_detail'
    
    namespace collection_traits
    {
        
        template< typename C >
        inline BOOST_DEDUCED_TYPENAME result_iterator_of<C>::type 
        begin( C& c )
        {
            return collection_traits_detail::collection_begin< BOOST_DEDUCED_TYPENAME collection_traits_detail::collection<C>::type >::fun( c );
        }
    }
    
    using collection_traits::begin;
    
} // namespace 'boost'


#endif
