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
        int x;
        int y;

        x = 123;
        y = 123;
        (var(x) &= 456)();
        y &= 456;
        BOOST_TEST(x == y);

        x = 123;
        y = 123;
        (var(x) |= 456)();
        y |= 456;
        BOOST_TEST(x == y);

        x = 123;
        y = 123;
        (var(x) ^= 456)();
        y ^= 456;
        BOOST_TEST(x == y);

        x = 123;
        y = 123;
        (var(x) <<= 4)();
        y <<= 4;
        BOOST_TEST(x == y);

        x = 1230000;
        y = 1230000;
        (var(x) >>= 4)();
        y >>= 4;
        BOOST_TEST(x == y);

        int& r1 = (var(x) &= 456)(); // should be an lvalue
        int& r2 = (var(x) |= 456)(); // should be an lvalue
        int& r3 = (var(x) ^= 456)(); // should be an lvalue
        int& r4 = (var(x) <<= 4)(); // should be an lvalue
        int& r5 = (var(x) >>= 4)(); // should be an lvalue
        BOOST_TEST(&r1 == &r2 && &r2 == &r3 && &r3 == &r4 && &r4 == &r5);
    }

    {
        BOOST_TEST((val(123) & 456)() == (123 & 456));
        BOOST_TEST((val(123) | 456)() == (123 | 456));
        BOOST_TEST((val(123) ^ 456)() == (123 ^ 456));
        BOOST_TEST((val(123) << 4)() == (123 << 4));
        BOOST_TEST((val(1230000) >> 4)() == (1230000 >> 4));

        char const* s = "Yabadabadoo!!!\n";
        (cout << arg1)(s);
    }

    return 0;
}
