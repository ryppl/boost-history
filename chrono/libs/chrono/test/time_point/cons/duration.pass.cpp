//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::milliseconds Duration;
    {
    boost::chrono::time_point<Clock, Duration> t(Duration(3));
    BOOST_TEST(t.time_since_epoch() == Duration(3));
    }
    {
    boost::chrono::time_point<Clock, Duration> t(boost::chrono::seconds(3));
    BOOST_TEST(t.time_since_epoch() == Duration(3000));
    }

    return boost::report_errors();
}
