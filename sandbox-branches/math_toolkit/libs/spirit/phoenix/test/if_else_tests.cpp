/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <boost/test/minimal.hpp>
#include <boost/spirit/phoenix/core.hpp>
#include <boost/spirit/phoenix/operator.hpp>

using namespace boost::phoenix;
using namespace std;

int
test_main(int, char*[])
{
    {
        int x = 0;
        int y = 0;
        bool c = false;

        BOOST_TEST(if_else(arg1, 1234, 5678)(c) == 5678);
        BOOST_TEST(if_else(arg1, 1234, 'x')(c) == 'x');

        int& r = if_else(arg1, var(x), var(y))(c); // should be an lvalue
        BOOST_TEST(&y == &r);

        (if_else(arg1, var(x), var(y)) = 986754321)(c);
        BOOST_TEST(y == 986754321);
    }

    return 0;
}
