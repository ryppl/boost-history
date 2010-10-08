//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    {
    boost::chrono::seconds s1(3);
    boost::chrono::seconds s2(5);
    boost::chrono::seconds r = s1 + s2;
    BOOST_TEST(r.count() == 8);
    }
    {
    boost::chrono::seconds s1(3);
    boost::chrono::microseconds s2(5);
    boost::chrono::microseconds r = s1 + s2;
    BOOST_TEST(r.count() == 3000005);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(3);
    boost::chrono::duration<int, boost::ratio<3, 5> > s2(5);
    boost::chrono::duration<int, boost::ratio<1, 15> > r = s1 + s2;
    BOOST_TEST(r.count() == 75);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(3);
    boost::chrono::duration<double, boost::ratio<3, 5> > s2(5);
    boost::chrono::duration<double, boost::ratio<1, 15> > r = s1 + s2;
    BOOST_TEST(r.count() == 75);
    }
    return boost::report_errors();
}
