//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// inexact conversions allowed for floating point reps

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    boost::chrono::duration<double, boost::micro> us(1);
    boost::chrono::duration<double, boost::milli> ms = us;
    BOOST_TEST(ms.count() == 1./1000);
    return boost::report_errors();
}
