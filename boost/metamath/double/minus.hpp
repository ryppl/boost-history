// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_MINUS_HPP
#define BOOST_METAMATH_DOUBLE_MINUS_HPP

#include <boost/mpl/minus.hpp>
#include <boost/metamath/double/plus.hpp>
#include <boost/metamath/double/negate.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename N1,typename N2>
        struct minus_double_double :
        plus_double_double<N1,mpl::negate<N2> >::type
        {
        };

    } //namespace detail
}} //namespace boost::metamath

namespace boost { namespace mpl {
    template<>
    struct minus_impl<
        metamath::double_c_tag
        ,metamath::double_c_tag
    #if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
    #endif
    >
    {
        template< typename N1, typename N2 > struct apply
        : metamath::detail::cutoff<
            typename metamath::detail::plus_double_double<N1,typename mpl::negate<N2>::type>::type
        >::type
        {};
    };

    template<>
    struct minus_impl<
        metamath::double_c_tag
        ,integral_c_tag
    #if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
    #endif
    >
    {
        template< typename N1, typename N2 > struct apply
        : metamath::detail::cutoff<
            typename metamath::detail::plus_double_double<N1,metamath::integral_to_double<typename mpl::negate<N2>::type> >::type
        >::type 
        {};
    };

    template<>
    struct minus_impl<
        integral_c_tag
        ,metamath::double_c_tag
    #if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
    #endif
    >
    {
        template< typename N1, typename N2 > struct apply
        : metamath::detail::cutoff<
            typename metamath::detail::plus_double_double<metamath::integral_to_double<N1>,typename mpl::negate<N2>::type>::type
        >::type
        {};
    };
}}

#endif //BOOST_METAMATH_DOUBLE_MINUS_HPP
