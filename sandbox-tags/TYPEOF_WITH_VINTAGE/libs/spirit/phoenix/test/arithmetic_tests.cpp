/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <boost/test/minimal.hpp>
#include <boost/spirit/phoenix/core.hpp>
#include <boost/spirit/phoenix/operator.hpp>

using namespace boost::phoenix;
using namespace std;

int
test_main(int, char*[])
{
    {
        int x = 123;

        BOOST_TEST((var(x) += 456)() == 123 + 456);
        BOOST_TEST(x == 123 + 456);
        BOOST_TEST((var(x) -= 456)() == 123);
        BOOST_TEST(x == 123);
        BOOST_TEST((var(x) *= 456)() == 123 * 456);
        BOOST_TEST(x == 123 * 456);
        BOOST_TEST((var(x) /= 456)() == 123);
        BOOST_TEST(x == 123);

        int& r1 = (var(x) += 456)(); // should be an lvalue
        int& r2 = (var(x) -= 456)(); // should be an lvalue
        int& r3 = (var(x) *= 456)(); // should be an lvalue
        int& r4 = (var(x) /= 456)(); // should be an lvalue
        BOOST_TEST(r1 == 123 && r2 == 123 && r3 == 123 && r4 == 123);

        BOOST_TEST((var(x) %= 456)() == 123 % 456);
        BOOST_TEST(x == 123 % 456);
    }

    {
        BOOST_TEST((val(123) + 456)() == 123 + 456);
        BOOST_TEST((val(123) - 456)() == 123 - 456);
        BOOST_TEST((val(123) * 456)() == 123 * 456);
        BOOST_TEST((val(123) / 456)() == 123 / 456);
        BOOST_TEST((val(123) % 456)() == 123 % 456);

        BOOST_TEST((123 + val(456))() == 123 + 456);
        BOOST_TEST((123 - val(456))() == 123 - 456);
        BOOST_TEST((123 * val(456))() == 123 * 456);
        BOOST_TEST((123 / val(456))() == 123 / 456);
        BOOST_TEST((123 % val(456))() == 123 % 456);
    }

    return 0;
}
