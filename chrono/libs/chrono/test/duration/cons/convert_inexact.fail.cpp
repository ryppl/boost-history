//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// inexact conversions disallowed for integral reps

#include <boost/chrono.hpp>

void test()
{
    boost::chrono::microseconds us(1);
    boost::chrono::milliseconds ms = us;
}
