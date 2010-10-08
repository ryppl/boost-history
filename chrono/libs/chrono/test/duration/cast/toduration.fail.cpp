//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// ToDuration must be an instantiation of duration.

#include <boost/chrono.hpp>

void test()
{
    boost::chrono::duration_cast<int>(boost::chrono::milliseconds(3));
}
