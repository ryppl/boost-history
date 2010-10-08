//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    boost::chrono::seconds s(3);
    s += boost::chrono::seconds(2);
    BOOST_TEST(s.count() == 5);
    s += boost::chrono::minutes(2);
    BOOST_TEST(s.count() == 125);
    return boost::report_errors();
}
