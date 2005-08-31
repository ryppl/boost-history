// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATHMATH_DOUBLE_PLUS_HPP
#define BOOST_METAMATHMATH_DOUBLE_PLUS_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/detail/additive.hpp>
#include <boost/metamath/double/detail/shift.hpp>
#include <boost/metamath/double/detail/mantissa.hpp>
#include <boost/metamath/double/limits.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename Interval1,typename Interval2>
        struct plus_double_double_impl {
            template<typename N1,typename N2>
            struct apply {
                BOOST_STATIC_CONSTANT(boost::int16_t,exp_diff=(N1::exponent-N2::exponent));
                BOOST_STATIC_CONSTANT(boost::int16_t,exponent=(exp_diff<0?N2::exponent:N1::exponent));
                typedef detail::shift_mantissa<typename N1::mantissa,(exp_diff<0?exp_diff:0)> mantissa1;
                typedef detail::shift_mantissa<typename N2::mantissa,(exp_diff<0?0:-exp_diff)> mantissa2;
                BOOST_STATIC_CONSTANT(bool,sign=(mpl::if_<
                    detail::less_mantissa<mantissa1,mantissa2>,
                    mpl::bool_<N2::sign>,
                    mpl::bool_<N1::sign>
                >::type::value));

                typedef typename mpl::if_<
                    detail::less_mantissa<mantissa1,mantissa2>,
                    mantissa1,
                    mantissa2
                >::type least_mantissa;

                typedef typename  mpl::if_<
                    detail::less_mantissa<mantissa1,mantissa2>,
                    mantissa2,
                    mantissa1
                >::type greatest_mantissa;
                BOOST_STATIC_CONSTANT(bool,equal_sign=(N1::sign==N2::sign));
                typedef typename mpl::eval_if<
                    mpl::bool_<equal_sign>,
                    detail::add_double<greatest_mantissa,least_mantissa,exponent,sign>,
                    detail::sub_double<greatest_mantissa,least_mantissa,exponent,sign>
                >::type type;
            };
        };

        template<>
        struct plus_double_double_impl<meta::NaN,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct plus_double_double_impl<meta::NaN,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };
        
        template<>
        struct plus_double_double_impl<meta::NaN,double>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct plus_double_double_impl<meta::infinity,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct plus_double_double_impl<double,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };       
        
        template<>
        struct plus_double_double_impl<meta::infinity,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply {
                BOOST_STATIC_CONSTANT(bool,same_sign=(N1::sign==N2::sign));
                typedef typename mpl::if_<
                    mpl::bool_<same_sign>,
                    N1,
                    meta::NaN
                >::type type;
            };
        };

        template<typename N>
        struct plus_double_double_filter {
            typedef typename mpl::if_<
                is_NaN<N>,
                meta::NaN,
                typename mpl::if_<
                    is_finite<N>,
                    double,
                    meta::infinity
                >::type
            >::type type;
        };

        template<typename N1,typename N2>
        struct plus_double_double 
            : mpl::apply2<
                plus_double_double_impl<
                    typename plus_double_double_filter<N1>::type,
                    typename plus_double_double_filter<N2>::type
                >,N1,N2
            >::type
        {};
/*            BOOST_STATIC_CONSTANT(boost::int16_t,exp_diff=(N1::exponent-N2::exponent));
            BOOST_STATIC_CONSTANT(boost::int16_t,exponent=(exp_diff<0?N2::exponent:N1::exponent));
            typedef detail::shift_mantissa<typename N1::mantissa,(exp_diff<0?exp_diff:0)> mantissa1;
            typedef detail::shift_mantissa<typename N2::mantissa,(exp_diff<0?0:-exp_diff)> mantissa2;
            BOOST_STATIC_CONSTANT(bool,sign=(mpl::if_<
                detail::less_mantissa<mantissa1,mantissa2>,
                mpl::bool_<N2::sign>,
                mpl::bool_<N1::sign>
            >::type::value));

            typedef typename mpl::if_<
                detail::less_mantissa<mantissa1,mantissa2>,
                mantissa1,
                mantissa2
            >::type least_mantissa;

            typedef typename  mpl::if_<
                detail::less_mantissa<mantissa1,mantissa2>,
                mantissa2,
                mantissa1
            >::type greatest_mantissa;
            BOOST_STATIC_CONSTANT(bool,equal_sign=(N1::sign==N2::sign));
            typedef typename mpl::eval_if<
                mpl::bool_<equal_sign>,
                detail::add_double<greatest_mantissa,least_mantissa,exponent,sign>,
                detail::sub_double<greatest_mantissa,least_mantissa,exponent,sign>
            >::type type;
        };*/

    } //namespace detail
}} //namespace boost::metamath

namespace boost { namespace mpl {
    template<>
    struct plus_impl< 
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
            metamath::detail::plus_double_double<N1,N2>
        >::type
        {};
    };

    template<>
    struct plus_impl< 
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
            metamath::detail::plus_double_double<N1,metamath::integral_to_double<N2> >
        >::type
        {};
    };

    template<>
    struct plus_impl< 
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
            metamath::detail::plus_double_double<metamath::integral_to_double<N1>,N2>
        >::type
        {};
    };
}}

#endif //BOOST_METAMATHMATH_DOUBLE_PLUS_HPP
