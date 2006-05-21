// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_EULER_HPP
#define BOOST_METAMATH_DOUBLE_EULER_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>

namespace boost {namespace metamath {
	namespace meta {
        template<int N>
        struct E;

        template<> struct E<1> 
        : long_to_double<1>
        {};

        template<> struct E<2> 
        : long_to_double<5>
        {};

        template<> struct E<3> 
        : long_to_double<61>
        {};

        template<> struct E<4> 
        : long_to_double<1385>
        {};

        template<> struct E<5> 
        : long_to_double<50521>
        {};

        template<> struct E<6> 
        : long_to_double<2702765>
        {};

    }
}}

#endif