//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::microseconds Duration1;
    typedef boost::chrono::milliseconds Duration2;
    {
    boost::chrono::time_point<Clock, Duration2> t2(Duration2(3));
    boost::chrono::time_point<Clock, Duration1> t1 = t2;
    BOOST_TEST(t1.time_since_epoch() == Duration1(3000));
    }

    return boost::report_errors();
}
