// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_DETAIL_MANTISSA_HPP
#define BOOST_METAMATH_DOUBLE_DETAIL_MANTISSA_HPP

#include <boost/cstdint.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<boost::uint32_t Part1,boost::uint32_t Part2>
        struct mantissa {
            BOOST_STATIC_CONSTANT(boost::uint32_t,part1=Part1);
            BOOST_STATIC_CONSTANT(boost::uint32_t,part2=Part2);        
        };

        template<typename Mantissa1,typename Mantissa2>
        struct less_mantissa 
        {
            BOOST_STATIC_CONSTANT(bool,value=(Mantissa1::part1==Mantissa2::part1?Mantissa1::part2<Mantissa2::part2:Mantissa1::part1<Mantissa2::part1));
            typedef boost::mpl::bool_<value> type;
        };

    }//namespace detail

}}//namespace boost::metamath

#endif
