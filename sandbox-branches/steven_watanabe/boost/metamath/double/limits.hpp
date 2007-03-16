// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_LIMITS_HPP
#define BOOST_METAMATH_DOUBLE_LIMITS_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/mpl/bool.hpp>

namespace boost {namespace metamath {
	namespace meta {
        typedef double_<detail::mantissa<0x20000000,0>,0x0400,true> NaN;
        typedef double_<detail::mantissa<0,0>,0x0400,false> infinity;
        typedef double_<detail::mantissa<0,0>,0x0400,true> negative_infinity;
        typedef double_<detail::mantissa<0x7fffffff,0x7ffffe00>,0x03ff,false> float_max;
        typedef double_<detail::mantissa<0x40000000,0>,-0x03fe,false> float_min;
        typedef double_<detail::mantissa<0,0>,-0x03ff,false> zero;
    }

    template<typename N>
    struct is_NaN {
        BOOST_STATIC_CONSTANT(bool,value=(
            (N::exponent==0x400) && (N::mantissa::part1!=0 || N::mantissa::part2!=0)
        ));
        typedef mpl::bool_<value> type;
    };

    template<typename N>
    struct is_finite {
        BOOST_STATIC_CONSTANT(bool,value=(N::exponent<0x400));
        typedef mpl::bool_<value> type;
    };
    template<typename N>
    struct is_positive_infinity {
        BOOST_STATIC_CONSTANT(bool,value=(N::exponent==0x400 && N::mantissa::part1==0 && N::mantissa::part2==0 && !N::sign));
        typedef mpl::bool_<value> type;
    };

    template<typename N>
    struct is_negative_infinity {
        BOOST_STATIC_CONSTANT(bool,value=(N::exponent==0x400 && N::mantissa::part1==0 && N::mantissa::part2==0 && N::sign));
        typedef mpl::bool_<value> type;
    };

    template<typename N>
    struct is_zero {
        BOOST_STATIC_CONSTANT(bool,value=((N::exponent==-0x3ff) && N::mantissa::part1==0 && N::mantissa::part2==0));
        typedef mpl::bool_<value> type;
    };
    template<typename N>
    struct is_negative {
        BOOST_STATIC_CONSTANT(bool,value=(N::sign==true));
        typedef mpl::bool_<value> type;
    };
}}

#endif
