//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  See http://www.boost.org for updates, documentation, and revision 

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_END_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_END_HPP

#include <boost/collection_traits/detail/implementation_help.hpp>
#include <boost/collection_traits/detail/result_iterator.hpp>
#include <boost/collection_traits/detail/common.hpp>

namespace boost 
{
    namespace collection_traits_detail
    {
        template< typename T >
        struct collection_end;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_end<std_container_>
        {
            template< typename C >
            static BOOST_CT_DEDUCED_TYPENAME result_iterator_of<C>::type fun( C& c )
            {
                return c.end();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_end<std_pair_>
        {
            template< typename P >
            static BOOST_CT_DEDUCED_TYPENAME result_iterator_of<P>::type fun( const P& p )
            {
                return p.second;
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_end<array_>  
        {
            template< typename T, std::size_t sz >
            static T* fun( T BOOST_ARRAY_REF[sz] )
            {
                return boost::collection_traits_detail::array_end( array );
            }
        };

                
        template<>
        struct collection_end<char_array_>
        {
            template< typename T, std::size_t sz >
            static std::size_t fun( T BOOST_ARRAY_REF[sz] )
            {
                return boost::collection_traits_detail::array_end( array );
            }
        };
        
        template<>
        struct collection_end<wchar_t_array_>
        {
            template< typename T, std::size_t sz >
            static std::size_t fun( T BOOST_ARRAY_REF[sz] )
            {
                return boost::collection_traits_detail::array_end( array );
            }
        };

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_end<char_ptr_>
        {
            static char* fun( char* s )
            {
                return boost::collection_traits_detail::str_end( s );
            }
        };

        template<>
        struct collection_end<const_char_ptr_>
        {
            static const char* fun( const char* s )
            {
                return boost::collection_traits_detail::str_end( s );
            }
        };

        template<>
        struct collection_end<wchar_t_ptr_>
        {
            static wchar_t* fun( wchar_t* s )
            {
                return boost::collection_traits_detail::str_end( s );
            }
        };


        template<>
        struct collection_end<const_wchar_t_ptr_>
        {
            static const wchar_t* fun( const wchar_t* s )
            {
                return boost::collection_traits_detail::str_end( s );
            }
        };
        

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct collection_end<iterator_>
        {
            template< typename I >
            static BOOST_DEDUCED_TYPENAME result_iterator_of<I>::type fun( I& i )
            {
                return I();
            }
        };

        } // namespace 'collection_traits_detail'
    
    namespace collection_traits
    {
    
        template< typename C >
        inline BOOST_DEDUCED_TYPENAME result_iterator_of<C>::type 
        end( C& c )
        {
            return collection_traits_detail::collection_end<  BOOST_DEDUCED_TYPENAME collection_traits_detail::collection<C>::type >::fun( c );
        }
    }
    
    using collection_traits::end;
    
} // namespace 'boost'


#endif
