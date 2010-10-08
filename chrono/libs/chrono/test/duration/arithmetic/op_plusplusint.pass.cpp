//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
    boost::chrono::hours h(3);
    boost::chrono::hours h2 = h++;
    BOOST_TEST(h.count() == 4);
    BOOST_TEST(h2.count() == 3);
    return boost::report_errors();
}
