// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_BERNOULLI_HPP
#define BOOST_METAMATH_DOUBLE_BERNOULLI_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/divides.hpp>

namespace boost {namespace metamath {
	namespace meta {
        template<int N>
        struct B;

        template<> struct B<1> 
        : detail::divides_double_double<long_to_double<1>,long_to_double<6> >::type
        {};

        template<> struct B<2> 
        : detail::divides_double_double<long_to_double<1>,long_to_double<30> >::type
        {};

        template<> struct B<3> 
        : detail::divides_double_double<long_to_double<1>,long_to_double<42> >::type
        {};

        template<> struct B<4> 
        : detail::divides_double_double<long_to_double<1>,long_to_double<30> >::type
        {};

        template<> struct B<5> 
        : detail::divides_double_double<long_to_double<5>,long_to_double<66> >::type
        {};

        template<> struct B<6> 
        : detail::divides_double_double<long_to_double<691>,long_to_double<2730> >::type
        {};

        template<> struct B<7> 
        : detail::divides_double_double<long_to_double<7>,long_to_double<6> >::type
        {};

        template<> struct B<8> 
        : detail::divides_double_double<long_to_double<3617>,long_to_double<510> >::type
        {};

        template<> struct B<9> 
        : detail::divides_double_double<long_to_double<43867>,long_to_double<798> >::type
        {};

        template<> struct B<10> 
        : detail::divides_double_double<long_to_double<174611>,long_to_double<330> >::type
        {};
    }
}}

#endif