#ifndef BOOST_CONTAINER_TRAITS_DETAIL_END_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_END_HPP

#include <boost/container_traits/detail/result_iterator.hpp>
#include <boost/container_traits/detail/common.hpp>
#include <string>

namespace boost 
{
    namespace container_traits_detail
    {
        template< typename T >
        struct container_end;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_end<std_container_>
        {
            template< typename C >
            static BOOST_DEDUCED_TYPENAME container_result_iterator<C>::type fun( C& c )
            {
                return c.end();
            };
        };
                    
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_end<std_pair_>
        {
            template< typename P >
            static BOOST_DEDUCED_TYPENAME container_result_iterator<P>::type fun( const P& p )
            {
                return p.second;
            }
        };
 
        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_end<array_>; // give up
		/*
        {
            template< typename T, std::size_t sz >
            BOOST_DEDUCED_TYPENAME container_result_iterator<array_>::type operator()( T BOOST_ARRAY_REF[sz] )
            {
                return array + sz;
            }
        };
        */

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_end<string_>
        {
            template< typename S >
            static BOOST_DEDUCED_TYPENAME container_result_iterator<S>::type fun( S& s )
            {
                if( s == 0 || s[0] == 0 )
                    return s;
                return s + std::char_traits<S>::length( s );
            }
        };

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
        template<>
        struct container_end<iterator_>
        {
            template< typename I >
            static BOOST_DEDUCED_TYPENAME container_result_iterator<I>::type fun( I& i )
            {
                return I();
            }
        };

        } // namespace 'container_traits_detail'
    
    template< typename C >
    inline BOOST_DEDUCED_TYPENAME container_result_iterator<C>::type 
    end( C& c )
    {
        return container_traits_detail::container_end<  BOOST_DEDUCED_TYPENAME container_traits_detail::container<C>::type >::fun( c );
    }

} // namespace 'boost'


#endif
