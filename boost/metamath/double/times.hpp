// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATHMATH_DOUBLE_TIMES_HPP
#define BOOST_METAMATHMATH_DOUBLE_TIMES_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/times.hpp>
#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/detail/normalize.hpp>
#include <boost/metamath/double/limits.hpp>
#include <boost/detail/workaround.hpp>


namespace boost { namespace metamath { 
    namespace detail {

        template<typename Interval1,typename Interval2>
        struct times_double_double_impl {
            template<typename N1,typename N2>
            struct apply {
                BOOST_STATIC_CONSTANT(boost::uint32_t, lhs1=(N1::mantissa::part1>>15));
                BOOST_STATIC_CONSTANT(boost::uint32_t, lhs2=(N1::mantissa::part1&0x7fff));
                BOOST_STATIC_CONSTANT(boost::uint32_t, lhs3=(N1::mantissa::part2>>16));
                BOOST_STATIC_CONSTANT(boost::uint32_t, lhs4=((N1::mantissa::part2>>1)&0x7fff));

                BOOST_STATIC_CONSTANT(boost::uint32_t, rhs1=(N2::mantissa::part1>>15));
                BOOST_STATIC_CONSTANT(boost::uint32_t, rhs2=(N2::mantissa::part1&0x7fff));
                BOOST_STATIC_CONSTANT(boost::uint32_t, rhs3=(N2::mantissa::part2>>16));
                BOOST_STATIC_CONSTANT(boost::uint32_t, rhs4=((N2::mantissa::part2>>1)&0x7fff));

                BOOST_STATIC_CONSTANT(boost::uint32_t, result0=lhs1*rhs1);
                BOOST_STATIC_CONSTANT(boost::uint32_t, result15=lhs1*rhs2+lhs2*rhs1);
                BOOST_STATIC_CONSTANT(boost::uint32_t, result30=lhs1*rhs3+lhs3*rhs1+lhs2*rhs2);
                BOOST_STATIC_CONSTANT(boost::uint32_t, result45=lhs1*rhs4+lhs3*rhs2+lhs2*rhs3+lhs4*rhs1);
                BOOST_STATIC_CONSTANT(boost::uint32_t, result60=lhs2*rhs4+lhs3*rhs3+lhs4*rhs2);

                BOOST_STATIC_CONSTANT(boost::uint32_t, part1_pre=result0+(result15>>15)+(result30>>30));
                BOOST_STATIC_CONSTANT(boost::uint32_t, part2_pre=(
                        ((result15<<15)&0x3fffffff)+
                        (result30&0x3fffffff)+
                        (result45>>15)+
                        (result60>>30)
                    )<<1);
                BOOST_STATIC_CONSTANT(boost::uint32_t, part1=part1_pre+((part2_pre&0x80000000)?1:0));
                BOOST_STATIC_CONSTANT(boost::uint32_t, part2=part2_pre&0x7fffffff);
                BOOST_STATIC_CONSTANT(boost::int16_t, exponent=N1::exponent+N2::exponent);
                BOOST_STATIC_CONSTANT(bool, sign=N1::sign!=N2::sign);

                typedef typename normalize<mantissa<part1,part2>,exponent,sign>::type type;
            };
        };

        template<>
        struct times_double_double_impl<meta::NaN,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct times_double_double_impl<meta::NaN,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };
        
        template<>
        struct times_double_double_impl<meta::NaN,meta::zero>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct times_double_double_impl<meta::NaN,double>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct times_double_double_impl<meta::infinity,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct times_double_double_impl<meta::zero,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct times_double_double_impl<double,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };       
        
        template<>
        struct times_double_double_impl<meta::infinity,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply {
                BOOST_STATIC_CONSTANT(bool,same_sign=(N1::sign==N2::sign));
                typedef typename mpl::if_<
                    mpl::bool_<same_sign>,
                    meta::infinity,
                    meta::negative_infinity
                >::type type;
            };
        };

        template<>
        struct times_double_double_impl<meta::infinity,meta::zero>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct times_double_double_impl<meta::zero,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<typename N>
        struct times_double_double_filter {
            typedef typename mpl::if_<
                is_NaN<N>,
                meta::NaN,
                typename mpl::if_<
                    is_zero<N>,
                    meta::zero,
                    typename mpl::if_<
                        is_finite<N>,
                        double,
                        meta::infinity
                    >::type
                >::type
            >::type type;
        };


        template<typename N1,typename N2>
        struct times_double_double :
            mpl::apply2<
                times_double_double_impl<
                    typename times_double_double_filter<N1>::type,
                    typename times_double_double_filter<N2>::type
                >,N1,N2
            >::type
        {};

    } //namespace detail
}}//namespace boost::metamath

namespace boost { namespace mpl {
    template<>
    struct times_impl< 
         metamath::double_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
    >
    {
        template< typename N1, typename N2 > struct apply
        : metamath::detail::round<
            metamath::detail::times_double_double<N1,N2>
        >::type
        {};
    };

   template<>
   struct times_impl< 
         metamath::double_c_tag
        ,integral_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
    >
   {
       template< typename N1, typename N2 > struct apply
       : metamath::detail::round<
            metamath::detail::times_double_double<N1,metamath::integral_to_double<N2> >
       >::type
       {};
   };

   template<>
   struct times_impl< 
        integral_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
    >
   {
       template< typename N1, typename N2 > struct apply
       : metamath::detail::round<
            metamath::detail::times_double_double<metamath::integral_to_double<N1>,N2>
       >::type
       {
       };
   };
}}

#endif //BOOST_METAMATHMATH_DOUBLE_TIMES_HPP
