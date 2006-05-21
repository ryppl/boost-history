// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_DETAIL_SHIFT_HPP
#define BOOST_METAMATH_DOUBLE_DETAIL_SHIFT_HPP

#include <boost/cstdint.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<int16_t Amount>
        struct shift_sign {
            BOOST_STATIC_CONSTANT(int16_t,value=((Amount>0)?1:(Amount<0)?-1:0));
        };

        template<int16_t sign>
        struct shift_integral_impl {
            template<typename T,T Value,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(T,value=Value);
            };

        };

        template<>
        struct shift_integral_impl<32> {
            template<typename T,T Value,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(T,value=0);
            };
        };

        template<>
        struct shift_integral_impl<1> {
            template<typename T,T Value,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(T,value=(Value<<Amount));
            };

        };

        template<>
        struct shift_integral_impl<-1> {
            template<typename T,T Value,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(T,value=(Value>>-Amount));
            };
        };

        template<>
        struct shift_integral_impl<-32> {
            template<typename T,T Value,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(T,value=0);
            };
        };

        template<int16_t Amount>
        struct shift_sign_overflow {
            BOOST_STATIC_CONSTANT(int16_t,value=(
                (Amount>0)
                    ?((Amount>=32)?32:1)
                    :((Amount<0)
                        ?((Amount<=-32)?-32:-1)
                        :0)
                    )
            );
        };

        template<typename T,T Value,int16_t Amount>
        struct shift_integral : shift_integral_impl<shift_sign_overflow<Amount>::value>::template apply<T,Value,Amount>
        {};

        template<int16_t sign>
        struct shift_mantissa_impl
        {
            template<typename Mantissa,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(uint32_t,part1=Mantissa::part1);
                BOOST_STATIC_CONSTANT(uint32_t,part2=Mantissa::part2);
            };
        };

        template<>//left shift
        struct shift_mantissa_impl<1> {
            template<typename Mantissa,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(uint32_t,part1=
                    (shift_integral<uint32_t,Mantissa::part1,Amount>::value
                    |shift_integral<uint32_t,Mantissa::part2,Amount-31>::value)
                );
                BOOST_STATIC_CONSTANT(uint32_t,part2=
                    (shift_integral<uint32_t,Mantissa::part2,Amount>::value&0x7fffffff)
                );
            };
        };

        template<>//right shift
        struct shift_mantissa_impl<-1> {
            template<typename Mantissa,int16_t Amount>
            struct apply {
                BOOST_STATIC_CONSTANT(uint32_t,part1=
                    (shift_integral<uint32_t,Mantissa::part1,Amount>::value)
                );
                BOOST_STATIC_CONSTANT(uint32_t,part2=
                    (shift_integral<uint32_t,Mantissa::part2,Amount>::value
                    |shift_integral<uint32_t,Mantissa::part1,31+Amount>::value)&0x7fffffff
                );
            };
        };
        template<typename Mantissa,int16_t Amount>
        struct shift_mantissa : shift_mantissa_impl<shift_sign<Amount>::value>::template apply<Mantissa,Amount>
        {};
    }//namespace detail
}}//namespace boost::metamath

#endif
