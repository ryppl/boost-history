//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// exact conversions allowed for integral reps

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    boost::chrono::milliseconds ms(1);
    boost::chrono::microseconds us = ms;
    BOOST_TEST(us.count() == 1000);
    return boost::report_errors();
}
