//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    boost::chrono::microseconds us(11);
    us %= 3;
    BOOST_TEST(us.count() == 2);
    return boost::report_errors();
}
