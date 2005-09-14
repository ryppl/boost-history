// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATHMATH_DOUBLE_DIVIDES_HPP
#define BOOST_METAMATHMATH_DOUBLE_DIVIDES_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/detail/normalize.hpp>
#include <boost/metamath/double/limits.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>

#define BOOST_METAMATH_MANTISSA_LESS(above,below)\
    ((BOOST_PP_CAT(above,1)==BOOST_PP_CAT(below,1))?\
        (BOOST_PP_CAT(above,2)<BOOST_PP_CAT(below,2)):\
        (BOOST_PP_CAT(above,1)<BOOST_PP_CAT(below,1)))

#define BOOST_METAMATH_MANTISSA_DIVIDE_BIT(above,below)\
    (BOOST_METAMATH_MANTISSA_LESS(above,below)?0:1)

#define BOOST_METAMATH_MANTISSA_SUBTRACT1(above,below)\
    ((BOOST_PP_CAT(above,1)-BOOST_PP_CAT(below,1))-(BOOST_PP_CAT(above,2)<BOOST_PP_CAT(below,2)?1:0))

#define BOOST_METAMATH_MANTISSA_SUBTRACT2(above,below)\
    ((((BOOST_PP_CAT(above,2)<BOOST_PP_CAT(below,2)?0x80000000:0)|BOOST_PP_CAT(above,2))-BOOST_PP_CAT(below,2))&0x7fffffff)

#define BOOST_METAMATH_MANTISSA_NEXT1(above1,above2)\
    (above1<<1)+(above2>>30)

#define BOOST_METAMATH_MANTISSA_NEXT2(above2)\
    ((above2<<1)&0x7fffffff)

#define BOOST_METAMATH_MANTISSA_NEXT_IMPL(index,arg1,arg2)\
    BOOST_PP_CAT(BOOST_PP_CAT(above,index),1) = BOOST_METAMATH_MANTISSA_LESS(arg1,arg2)\
        ?BOOST_METAMATH_MANTISSA_NEXT1(BOOST_PP_CAT(arg1,1),BOOST_PP_CAT(arg1,2))\
        :BOOST_METAMATH_MANTISSA_NEXT1(BOOST_METAMATH_MANTISSA_SUBTRACT1(arg1,arg2),BOOST_METAMATH_MANTISSA_SUBTRACT2(arg1,arg2)),\
    BOOST_PP_CAT(BOOST_PP_CAT(above,index),2) = BOOST_METAMATH_MANTISSA_LESS(arg1,arg2)\
        ?BOOST_METAMATH_MANTISSA_NEXT2(BOOST_PP_CAT(arg1,2))\
        :BOOST_METAMATH_MANTISSA_NEXT2(BOOST_METAMATH_MANTISSA_SUBTRACT2(arg1,arg2))

#define BOOST_METAMATH_MANTISSA_NEXT(z,index,text)\
    BOOST_STATIC_CONSTANT(boost::uint32_t,BOOST_METAMATH_MANTISSA_NEXT_IMPL(BOOST_PP_INC(index),BOOST_PP_CAT(above,index),below));

#define BOOST_METAMATH_MANTISSA_DIVIDE1(z,index,text)\
    BOOST_STATIC_CONSTANT(boost::uint32_t,BOOST_PP_CAT(BOOST_PP_CAT(result,BOOST_PP_INC(index)),1) = (BOOST_PP_CAT(BOOST_PP_CAT(result,index),1)<<1)+BOOST_METAMATH_MANTISSA_DIVIDE_BIT(BOOST_PP_CAT(above,index),below));

#define BOOST_METAMATH_MANTISSA_DIVIDE2(z,index,text)\
    BOOST_STATIC_CONSTANT(boost::uint32_t,BOOST_PP_CAT(BOOST_PP_CAT(result,BOOST_PP_INC(index)),2) = (BOOST_PP_CAT(BOOST_PP_CAT(result,index),2)<<1)+BOOST_METAMATH_MANTISSA_DIVIDE_BIT(BOOST_PP_CAT(above,BOOST_PP_ADD(index,text)),below));

namespace boost { namespace metamath { 
    namespace detail {

        template<typename Interval1,typename Interval2>
        struct divides_double_double_impl {
            template<typename N1,typename N2>
            struct apply {
                BOOST_STATIC_CONSTANT(boost::uint32_t,above01=N1::mantissa::part1);
                BOOST_STATIC_CONSTANT(boost::uint32_t,above02=N1::mantissa::part2);
                BOOST_STATIC_CONSTANT(boost::uint32_t,below1=N2::mantissa::part1);
                BOOST_STATIC_CONSTANT(boost::uint32_t,below2=N2::mantissa::part2);
                BOOST_STATIC_CONSTANT(boost::uint32_t,result01=0);
                BOOST_STATIC_CONSTANT(boost::uint32_t,result02=0);
                BOOST_PP_REPEAT(63,BOOST_METAMATH_MANTISSA_NEXT,~)
                BOOST_PP_REPEAT(31,BOOST_METAMATH_MANTISSA_DIVIDE1,~)
                BOOST_PP_REPEAT(31,BOOST_METAMATH_MANTISSA_DIVIDE2,31)
                BOOST_STATIC_CONSTANT(boost::uint32_t,part1=result311);
                BOOST_STATIC_CONSTANT(boost::uint32_t,part2=result312);
                BOOST_STATIC_CONSTANT(boost::int16_t, exponent=N1::exponent-N2::exponent);
                BOOST_STATIC_CONSTANT(bool, sign=N1::sign!=N2::sign);

                typedef typename normalize<mantissa<part1,part2>,exponent,sign>::type type;
            };
        };

        template<>
        struct divides_double_double_impl<meta::NaN,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct divides_double_double_impl<meta::NaN,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };
        
        template<>
        struct divides_double_double_impl<meta::NaN,meta::zero>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct divides_double_double_impl<meta::NaN,double>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct divides_double_double_impl<meta::infinity,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct divides_double_double_impl<meta::zero,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct divides_double_double_impl<double,meta::NaN>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };       
        
        template<>
        struct divides_double_double_impl<meta::infinity,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<>
        struct divides_double_double_impl<meta::infinity,meta::zero>
        {
            template<typename N1,typename N2>
            struct apply {
                typedef N1 type;
            };
        };

        template<>
        struct divides_double_double_impl<double,meta::zero>
        {
            template<typename N1,typename N2>
            struct apply {
                BOOST_STATIC_CONSTANT(bool,sign=N1::sign);
                typedef typename mpl::if_<
                    mpl::bool_<sign>,
                    meta::negative_infinity,
                    meta::infinity
                >::type type;
            };
        };

        template<>
        struct divides_double_double_impl<meta::zero,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::zero{};
        };

        template<>
        struct divides_double_double_impl<double,meta::infinity>
        {
            template<typename N1,typename N2>
            struct apply : meta::zero{};
        };

        template<>
        struct divides_double_double_impl<meta::zero,meta::zero>
        {
            template<typename N1,typename N2>
            struct apply : meta::NaN{};
        };

        template<typename N>
        struct divides_double_double_filter {
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
        struct divides_double_double :
            mpl::apply2<
                divides_double_double_impl<
                    typename divides_double_double_filter<N1>::type,
                    typename divides_double_double_filter<N2>::type
                >,N1,N2
            >::type
        {};
    } //namespace detail
}}//namespace boost::metamath

namespace boost { namespace mpl {
   template<>
   struct divides_impl< 
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
            boost::metamath::detail::divides_double_double<N1,N2>
      >::type
      {};
   };

   template<>
   struct divides_impl< 
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
            boost::metamath::detail::divides_double_double<N1,boost::metamath::integral_to_double<N2> >
       >::type
       {};
   };

   template<>
   struct divides_impl< 
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
            boost::metamath::detail::divides_double_double<boost::metamath::integral_to_double<N1>,N2>
       >::type
       {};
   };
}}

#endif //BOOST_METAMATHMATH_DOUBLE_DIVIDES_HPP
