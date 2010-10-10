//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// Duration shall be an instance of duration.

#include <boost/chrono.hpp>

void test()
{
    typedef boost::chrono::time_point<boost::chrono::system_clock, int> T;
    T t;
}
