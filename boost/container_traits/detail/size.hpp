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
            static BOOST_DEDUCED_TYPENAME container_size_type<C>::type fun( C& c )
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
            static BOOST_DEDUCED_TYPENAME container_size_type<P>::type fun( const P& p )
            {
                return std::distance( p.first, p.second );
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<array_>; // give up
        /*
        {
            template< typename T, typename std::size_t sz >
             BOOST_DEDUCED_TYPENAME container_size_type<T[sz]>::type fun()( T BOOST_ARRAY_REF[sz] )
            {
                return sz;
            }
        };
        */
        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<string_>
        {
            template< typename S >
            static BOOST_DEDUCED_TYPENAME container_size_type<S>::type fun( S& s )
            {
                if( s == 0 || s[0] == 0 )
                    return 0;
                return std::char_traits<S>::length( s );
            }
        };

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_size<iterator_>;

        } // namespace 'container_traits_detail'
    
    template< typename C >
    BOOST_DEDUCED_TYPENAME container_size_type<C>::type 
    size( C& c )
    {
        return container_traits_detail::container_size<  BOOST_DEDUCED_TYPENAME container_traits_detail::container<C>::type >::fun( c );
    }

} // namespace 'boost'


#endif
