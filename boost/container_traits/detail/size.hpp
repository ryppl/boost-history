#ifndef BOOST_CONTAINER_TRAITS_DETAIL_SIZE_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_SIZE_HPP

#include <boost/container_traits/detail/size_type.hpp>
#include <boost/container_traits/detail/common.hpp>
#include <iterator>

namespace boost 
{
    namespace container_traits_detail
    {
        template< typename T >
        struct container_size;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<std_container_>
        {
            template< typename C >
            static BOOST_DEDUCED_TYPENAME C::size_type fun( const C& c )
            {
                return c.size();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<std_pair_>
        {
            template< typename P >
            static BOOST_DEDUCED_TYPENAME size_type_of<P>::type fun( const P& p )
            {
                return std::distance( p.first, p.second );
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<array_>
        {
            template< typename T, std::size_t sz >
            static std::size_t fun( T BOOST_ARRAY_REF[sz] )
            {
                return sz;
            }
        };
        
        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////

        template<>
        struct container_size<char_ptr_>
        {
            static std::size_t fun( const char* s )
            {
                if( s == 0 || s[0] == 0 )
                    return 0;
                return std::char_traits<char>::length( s );
            }
        };

        template<>
        struct container_size<const_char_ptr_>
        {
            static std::size_t fun( const char* s )
            {
                if( s == 0 || s[0] == 0 )
                    return 0;
                return std::char_traits<char>::length( s );
            }
        };
        
        template<>
        struct container_size<wchar_t_ptr_>
        {
            static std::size_t fun( const wchar_t* s )
            {
                if( s == 0 || s[0] == 0 )
                    return 0;
                return std::char_traits<wchar_t>::length( s );
            }
        };

        template<>
        struct container_size<const_wchar_t_ptr_>
        {
            static std::size_t fun( const wchar_t* s )
            {
                if( s == 0 || s[0] == 0 )
                    return 0;
                return std::char_traits<wchar_t>::length( s );
            }
        };
  
        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<iterator_>; // not defined

        } // namespace 'container_traits_detail'
    
    template< typename C >
    BOOST_DEDUCED_TYPENAME size_type_of<C>::type 
    size( const C& c )
    {
        return container_traits_detail::container_size<  BOOST_DEDUCED_TYPENAME container_traits_detail::container<C>::type >::fun( c );
    }

} // namespace 'boost'


#endif
