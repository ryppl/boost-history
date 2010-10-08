//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    {
    boost::chrono::nanoseconds ns1(15);
    boost::chrono::nanoseconds ns2(5);
    BOOST_TEST(ns1 / ns2 == 3);
    }
    {
    boost::chrono::microseconds us1(15);
    boost::chrono::nanoseconds ns2(5);
    BOOST_TEST(us1 / ns2 == 3000);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(30);
    boost::chrono::duration<int, boost::ratio<3, 5> > s2(5);
    BOOST_TEST(s1 / s2 == 6);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(30);
    boost::chrono::duration<double, boost::ratio<3, 5> > s2(5);
    BOOST_TEST(s1 / s2 == 20./3);
    }
    return boost::report_errors();
}
