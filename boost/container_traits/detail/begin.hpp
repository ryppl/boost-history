#ifndef BOOST_CONTAINER_TRAITS_DETAIL_BEGIN_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_BEGIN_HPP

#include <boost/container_traits/detail/result_iterator.hpp>

namespace boost 
{
    namespace container_traits_detail
    {
        template< typename T >
        struct container_begin;

        template<>
        struct container_begin<std_container_>
        {
            template< typename C >
            static BOOST_DEDUCED_TYPENAME container_result_iterator<C>::type begin( C& c )
            {
                return c.begin();
            };
        };
                    

        template< typename P > 
            static const_iterator begin( const P& p )
        {
                    return p.first;
                }

                template< typename A >
                static iterator begin( A& a )
                {
                    return a;
                }



                template< typename P >
                static iterator begin( P& p )
                {
                    return p;
                }

                template< typename P >
                static const_iterator begin( const P& p )
                {
                    return p;
                }

                template< typename P >
                static iterator end( P& p )
                {
                    if ( p==0 )
                        return p;
                    else
                        return p+char_traits::length(p);
                }

                template< typename P >
                static const_iterator end( const P& p )
                {
                    if ( p==0 )
                        return p;
                    else
                        return p+char_traits::length(p);
                }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

                template< typename P >
                static result_iterator begin( P& p )
                {
                    return p;
                }

                template< typename P >
                static result_iterator end( P& p )
                {
                    if ( p==0 )
                        return p;
                    else
                        return p+char_traits::length(p);
                }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING    
            }; 

            template<typename T>
            struct pointer_container_traits_selector
            {
                typedef pointer_container_traits<T> type;
            };

        } // namespace detail
    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_DETAIL_CONTAINER_HPP
