// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_CONSTANTS_HPP
#define BOOST_METAMATH_DOUBLE_CONSTANTS_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/string_to_double.hpp>
#include <boost/metamath/double/limits.hpp>

namespace boost {namespace metamath {
	 namespace meta {
		  typedef long_to_double<1> one;
		  typedef long_to_double<2> two;
		  typedef long_to_double<3> three;
		  typedef long_to_double<10> ten;
        typedef string_c_to_double<3,'.',1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5>::type pi;
        typedef string_c_to_double<2,'.',7,1,8,2,8,1,8,2,8,4,5,9,0,4,5,2,3,5,3,6,0,2,8,7,5>::type e;

    }
    namespace {
        const meta::one one;
        const meta::two two;
        const meta::three three;
        const meta::ten ten;
        const meta::pi pi;
        const meta::e e;
    }
}}

#endif
