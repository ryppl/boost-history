//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <libs/chrono/test/clock/check_clock_invariants.hpp>

void test()
{
    check_clock_invariants<boost::chrono::high_resolution_clock>();
}
