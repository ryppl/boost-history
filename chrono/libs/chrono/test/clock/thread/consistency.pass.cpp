//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/chrono/thread_clock.hpp>
#include "../check_clock_invariants.hpp"

void test()
{
#if defined(BOOST_CHRONO_HAS_THREAD_CLOCK)
    check_clock_invariants<boost::chrono::thread_clock>();
#endif
}
