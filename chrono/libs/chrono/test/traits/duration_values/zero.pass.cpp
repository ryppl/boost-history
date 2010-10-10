//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "../../rep.h"

int main()
{
    BOOST_TEST(boost::chrono::duration_values<int>::zero() == 0);
    BOOST_TEST(boost::chrono::duration_values<Rep>::zero() == 0);

    return boost::report_errors();
}
