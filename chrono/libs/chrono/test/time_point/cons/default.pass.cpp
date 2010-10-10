//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "../../rep.h"

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::duration<Rep, boost::milli> Duration;
    boost::chrono::time_point<Clock, Duration> t;
    BOOST_TEST(t.time_since_epoch() == Duration::zero());

    return boost::report_errors();
}
