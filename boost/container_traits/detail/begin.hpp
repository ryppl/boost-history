#ifndef BOOST_CONTAINER_TRAITS_DETAIL_BEGIN_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_BEGIN_HPP

#include <boost/container_traits/detail/result_iterator.hpp>
#include <boost/container_traits/detail/common.hpp>

namespace boost 
{
    
    namespace container_traits_detail
    {
        template< typename T >
        struct container_begin;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_begin<std_container_>
        {
            template< typename C >
			static BOOST_DEDUCED_TYPENAME result_iterator_of<C>::type fun( C& c )
            {
                return c.begin();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_begin<std_pair_>
        {
            template< typename P >
            static BOOST_DEDUCED_TYPENAME result_iterator_of<P>::type fun( const P& p )
            {
                return p.first;
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_begin<array_>
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
        struct container_begin<char_ptr_>
        {
            static char* fun( char* s )
            {
                return s;
            }
        };

        template<>
        struct container_begin<const_char_ptr_>
        {
            static const char* fun( const char* s )
            {
                return s;
            }
        };
        
        template<>
        struct container_begin<wchar_t_ptr_>
        {
            
            static wchar_t* fun( wchar_t* s )
            {
                return s;
            }
        };

        template<>
        struct container_begin<const_wchar_t_ptr_>
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
        struct container_begin<iterator_>
        {
            template< typename I >
            static BOOST_DEDUCED_TYPENAME result_iterator_of<I>::type fun( I& i )
            {
                return i;
            }
        };

        } // namespace 'container_traits_detail'
    
    template< typename C >
    inline BOOST_DEDUCED_TYPENAME result_iterator_of<C>::type 
    begin( C& c )
    {
        return container_traits_detail::container_begin< BOOST_DEDUCED_TYPENAME container_traits_detail::container<C>::type >::fun( c );
    }

} // namespace 'boost'


#endif
