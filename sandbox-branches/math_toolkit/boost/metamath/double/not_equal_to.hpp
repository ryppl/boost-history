// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_NOT_EQUAL_TO_HPP
#define BOOST_METAMATH_DOUBLE_NOT_EQUAL_TO_HPP

#include <boost/mpl/not_equal_to.hpp>
#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/limits.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename N1,typename N2>
        struct not_equal_to_double_double {
            BOOST_STATIC_CONSTANT(bool,is_zero1=(is_zero<N1>::value));
            BOOST_STATIC_CONSTANT(bool,is_zero2=(is_zero<N2>::value));
            BOOST_STATIC_CONSTANT(bool,value=
                (is_zero1&&is_zero2)?false:
                (
                (N1::mantissa::part1!=N2::mantissa::part1) ||
                (N1::mantissa::part2!=N2::mantissa::part2) ||
                (N1::exponent!=N2::exponent) ||
                (N1::sign!=N2::sign)
                )
            );
            typedef mpl::bool_<value> type;
        };
    }//namespace detail
}} //namespace boost::metamath

namespace boost { namespace mpl {
    template<>
    struct not_equal_to_impl< 
         metamath::double_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
    {
        template< typename N1, typename N2 > struct apply
            : metamath::detail::not_equal_to_double_double<N1,N2>::type
        {};
    };

    template<>
    struct not_equal_to_impl< 
         metamath::double_c_tag
        ,integral_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
    >
    {
        template< typename N1, typename N2 > struct apply
            : metamath::detail::not_equal_to_double_double<N1,metamath::integral_to_double<N2> >::type
        {};
    };

    template<>
    struct not_equal_to_impl< 
         integral_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
    {
        template< typename N1, typename N2 > struct apply
            : metamath::detail::not_equal_to_double_double<metamath::integral_to_double<N1>,N2>::type
        {};
    };
}}

#endif //BOOST_METAMATH_DOUBLE_LESS_HPP
