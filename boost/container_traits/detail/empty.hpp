#ifndef BOOST_CONTAINER_TRAITS_DETAIL_EMPTY_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_EMPTY_HPP

#include <boost/container_traits/detail/common.hpp>

namespace boost 
{
    namespace container_traits_detail
    {
        template< typename T >
        struct container_empty;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_empty<std_container_>
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
        struct container_empty<std_pair_>
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
        struct container_empty<array_>
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
        struct container_empty<char_ptr_>
        {
            static bool fun( const char* s )
            {
                return s == 0 || s[0] == 0;
            }
        };

        template<>
        struct container_empty<const_char_ptr_>
        {
            static bool fun( const char* s )
            {
                return  s == 0 || s[0] == 0;
            }
        };

        template<>
        struct container_empty<wchar_t_ptr_>
        {
            static bool fun( const wchar_t* s )
            {
                return  s == 0 || s[0] == 0;
            }
        };
        
        template<>
        struct container_empty<const_wchar_t_ptr_>
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
        struct container_empty<iterator_>
        {
            template< typename I >
            static bool fun( I& i )
            {
                return i == I();
            }
        };

        } // namespace 'container_traits_detail'
    
    template< typename C >
    inline bool 
    empty( const C& c )
    {
        return container_traits_detail::container_empty<  BOOST_DEDUCED_TYPENAME container_traits_detail::container<C>::type >::fun( c );
    }

} // namespace 'boost'


#endif
