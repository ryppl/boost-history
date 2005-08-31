// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_DETAIL_NORMALIZE_HPP
#define BOOST_METAMATH_DOUBLE_DETAIL_NORMALIZE_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/detail/shift.hpp>
#include <boost/metamath/double/detail/mantissa.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename Mantissa,boost::int16_t Exponent,bool Sign>
        struct normalize {
            BOOST_STATIC_CONSTANT(boost::int16_t,exp1=(integral_exp<boost::uint32_t,Mantissa::part1>::value));
            BOOST_STATIC_CONSTANT(boost::int16_t,exp2=(integral_exp<boost::uint32_t,Mantissa::part2>::value));
            BOOST_STATIC_CONSTANT(boost::int16_t,exp_shift=
                (
                    (exp1==0 && exp2==0)?
                        (Exponent==1024)?0:
                        -Exponent-1023:
                    (exp1>0)?(exp1-30):
                    (exp2-61)
                )
            );
            typedef shift_mantissa<Mantissa,-exp_shift> shifted_mantissa;
            BOOST_STATIC_CONSTANT(boost::int16_t,exponent=(Exponent+exp_shift>=1024?1024:Exponent+exp_shift));
            BOOST_STATIC_CONSTANT(boost::uint32_t,part1=(exponent==1024?0:shifted_mantissa::part1));
            BOOST_STATIC_CONSTANT(boost::uint32_t,part2=(exponent==1024?0:shifted_mantissa::part2));

            typedef double_<detail::mantissa<part1,part2>,exponent,Sign> type;
        };
        template<typename Double>
        struct round {
            typedef typename Double::mantissa normalized_mantissa;
            BOOST_STATIC_CONSTANT(boost::uint32_t,part2=(
                (normalized_mantissa::part2&0xfffffe00)+
                (((normalized_mantissa::part2&0x00000100)==0x00000100)?0x00000200:0)
            ));
            BOOST_STATIC_CONSTANT(boost::uint32_t,part1=(
                normalized_mantissa::part1+
                ((part2&0x80000000)?1:0)
            ));
            typedef typename normalize<mantissa<part1,part2>,Double::exponent,Double::sign>::type type;
        };
        template<typename Double>
        struct cutoff {
            typedef typename Double::mantissa normalized_mantissa;
            BOOST_STATIC_CONSTANT(boost::uint32_t,part2=(normalized_mantissa::part2&0x7ffffe00));
            typedef typename double_<
                mantissa<normalized_mantissa::part1,part2>,
                Double::exponent,
                Double::sign
            >::type type;
        };
    }//namespace detail

}}//namespace boost::metamath

#endif
