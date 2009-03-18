/////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/math_limit/miscellanea.hpp>
#include <libs/adaptive_rejection_sampling/src/example/test_gaussian.h>

using namespace std;

int main()
{

    typedef boost::math_limit::limits<double> tmp_t;
    std::cout
        << "limits::negative_infinity()"
        << tmp_t::negative_infinity() << std::endl;
    std::cout
        << "limits::infinity()"
        << tmp_t::infinity() << std::endl;
    std::cout
        << "limits::min_value()"
        << tmp_t::min_value() << std::endl;
    std::cout
        << "limits::log_min_value()"
        << tmp_t::log_min_value() << std::endl;
    std::cout
        << "limits::log_max_value()"
        << tmp_t::log_max_value() << std::endl;

    libs::adaptive_rejection_sampling::example_standard_gaussian();
    return 0;
}
