#ifndef BOOST_UTILITY_COMMON_TYPE_HPP
#define BOOST_UTILITY_COMMON_TYPE_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/front.hpp>
#include <boost/type_traits.hpp>

namespace boost
{
    
namespace detail
{
    template<int, typename A, typename>
    struct Sel
    {
        typedef A type;
    };
    
    template<typename A, typename B>
    struct Sel<2, A, B>
    {
        typedef B type;
    };
    
    template<typename A, typename B>
    struct most_converted_type
    {
        static char (&deduce(const A&))[1];
        static char (&deduce(const B&))[2];
        typedef typename Sel<sizeof deduce(0 ? *(A*)0 : *(B*)0), A, B>::type type;
    };
    
    template<typename A>
    struct most_converted_type<A, A>
    {
        typedef A type;
    };
    
    template<typename A>
    struct most_converted_type<A, const A>
    {
        typedef A type;
    };
    
    template<typename A>
    struct most_converted_type<const A, A> : most_converted_type<A, const A>
    {
    };

} // namespace detail
    
    template<typename T, typename U>
    struct common_type2
    {
        typedef typename mpl::if_<
            mpl::and_<
                is_reference<T>,
                mpl::and_<
                    is_reference<U>,
                    is_same<
                        typename remove_cv<typename remove_reference<T>::type>::type,
                        typename remove_cv<typename remove_reference<U>::type>::type
                    >
                >
            >,
            typename add_reference<
                typename mpl::if_<
                    mpl::or_<
                        is_const<typename remove_reference<T>::type>,
                        is_const<typename remove_reference<U>::type>
                    >,
                    typename add_const<
                        typename detail::most_converted_type<
                            typename remove_reference<T>::type,
                            typename remove_reference<U>::type
                        >::type
                    >::type,
                    typename detail::most_converted_type<
                        typename remove_reference<T>::type,
                        typename remove_reference<U>::type
                    >::type
                >::type
            >::type,
            typename detail::most_converted_type<
                typename remove_reference<T>::type,
                typename remove_reference<U>::type
            >::type
        >::type type;
    };
    
    template<typename Sequence>
    struct common_type_seq : mpl::fold<
        Sequence,
        typename mpl::front<Sequence>::type,
        common_type2<mpl::_1, mpl::_2>
    >
    {
    };
} // namespace boost

#endif
