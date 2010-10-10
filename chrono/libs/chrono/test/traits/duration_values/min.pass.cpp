//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <limits>
#include <boost/detail/lightweight_test.hpp>
#include <iostream>

#include "../../rep.h"

int main()
{
    BOOST_TEST(boost::chrono::duration_values<int>::min() ==
           std::numeric_limits<int>::min());
    BOOST_TEST(boost::chrono::duration_values<double>::min() ==
           -std::numeric_limits<double>::max());
    BOOST_TEST(boost::chrono::duration_values<Rep>::min() ==
           std::numeric_limits<Rep>::min());


    return boost::report_errors();
}
