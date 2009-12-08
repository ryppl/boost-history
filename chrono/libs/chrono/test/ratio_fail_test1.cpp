//  ratio_fail_test1.cpp  ----------------------------------------------------//

//  Copyright 2008 Beman Dawes

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/ratio.hpp>

typedef boost::ratio<INTMAX_C(0x7FFFFFFFFFFFFFFF), INTMAX_C(0x7FFFFFFFFFFFFFF0)> R1;
typedef boost::ratio<INTMAX_C(0x7FFFFFFFFFFFFFFE), INTMAX_C(0x7FFFFFFFFFFFFFF0)> R2;
typedef boost::ratio_multiply<R1, R2>::type RT;
