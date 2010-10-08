//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    {
    boost::chrono::nanoseconds ns1(15);
    boost::chrono::nanoseconds ns2(6);
    boost::chrono::nanoseconds r = ns1 % ns2;
    BOOST_TEST(r.count() == 3);
    }
    {
    boost::chrono::microseconds us1(15);
    boost::chrono::nanoseconds ns2(28);
    boost::chrono::nanoseconds r = us1 % ns2;
    BOOST_TEST(r.count() == 20);
    }
    {
    boost::chrono::duration<int, boost::ratio<3, 5> > s1(6);
    boost::chrono::duration<int, boost::ratio<2, 3> > s2(3);
    boost::chrono::duration<int, boost::ratio<1, 15> > r = s1 % s2;
    BOOST_TEST(r.count() == 24);
    }
    return boost::report_errors();
}
