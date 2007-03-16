// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_DETAIL_ADDITIVE_HPP
#define BOOST_METAMATH_DOUBLE_DETAIL_ADDITIVE_HPP

#include <boost/metamath/double/detail/normalize.hpp>
#include <boost/metamath/double/detail/mantissa.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename Mantissa1,typename Mantissa2,boost::int16_t Exponent,bool Sign>
        struct add_double {
            BOOST_STATIC_CONSTANT(boost::uint32_t,part1=(
                (Mantissa1::part1+Mantissa2::part1)+
                (((Mantissa1::part2+Mantissa2::part2)&0x80000000)?1:0)
            ));
            BOOST_STATIC_CONSTANT(boost::uint32_t,part2=(
                (Mantissa1::part2+Mantissa2::part2)&0x7fffffff
            ));
            typedef typename normalize<mantissa<part1,part2>,Exponent,Sign>::type type;
        };

        template<typename Mantissa1,typename Mantissa2,boost::int16_t Exponent,bool Sign>
        struct sub_double {
            BOOST_STATIC_CONSTANT(boost::uint32_t,part1=(
                (Mantissa1::part1-Mantissa2::part1)-
                (Mantissa1::part2<Mantissa2::part2?1:0)
            ));
            BOOST_STATIC_CONSTANT(boost::uint32_t,part2=(
                (Mantissa1::part2|(Mantissa1::part2<Mantissa2::part2?0x80000000:0))-
                Mantissa2::part2
            ));
            typedef typename normalize<mantissa<part1,part2>,Exponent,Sign>::type type;
        };
    }//namespace detail
}}//namespace boost::metamath

#endif
