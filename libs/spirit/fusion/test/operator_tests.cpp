/*=============================================================================
    Phoenix v2.0
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <boost/test/minimal.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/operator.hpp>

using namespace boost::phoenix;
using namespace std;

int
test_main(int, char*[])
{
    {
        BOOST_TEST((-val(123))() == -123);
        BOOST_TEST((- -val(123))() == 123);
        BOOST_TEST((+val(123))() == 123);
    }

    {
        int x = 123;

        BOOST_TEST((++var(x))() == 124);
        BOOST_TEST(x == 124);
        BOOST_TEST((--var(x))() == 123);
        BOOST_TEST(x == 123);

        BOOST_TEST((var(x)++)() == 123);
        BOOST_TEST(x == 124);
        BOOST_TEST((var(x)--)() == 124);
        BOOST_TEST(x == 123);

        int& r1 = (++var(x))(); // should be an lvalue
        int& r2 = (--var(x))(); // should be an lvalue
        BOOST_TEST(r1 == 123 && r2 == 123);
    }

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
        (var(cout) << arg1)(s);
    }

    {
        BOOST_TEST(!(val(123) == 456)());
        BOOST_TEST((val(123) != 456)());
        BOOST_TEST(!(val(123) > 456)());
        BOOST_TEST((val(123) < 456)());
        BOOST_TEST(!(val(123) > 456)());
        BOOST_TEST((val(123) <= 123)());
        BOOST_TEST((val(123) >= 123)());
    }

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

    {
        bool x = false;
        bool y = true;

        BOOST_TEST((!arg1)(x) == true);
        BOOST_TEST((!arg1)(y) == false);
        BOOST_TEST((arg1 || arg2)(x, y) == true);
        BOOST_TEST((arg1 && arg2)(x, y) == false);

        // short circuiting:
        int i = 1234;
        (arg1 || (arg2 = 4567))(y, i);
        BOOST_TEST(i == 1234);
        (arg1 && (arg2 = 4567))(y, i);
        BOOST_TEST(i == 4567);
    }

    {
        int x = 123;
        BOOST_TEST((&arg1)(x) == &x);
        BOOST_TEST((*&arg1)(x) == 123);

        int y = 968;
        (var(x) = arg1)(y);
        BOOST_TEST(x == y);

        (arg1 = 456)(x);
        BOOST_TEST(x == 456);
        int& r = (arg1 = 456)(x); // must be an lvalue
        BOOST_TEST(&r == &x);

        int c[] = { 1, 2, 3, 4, 5 };
        BOOST_TEST((arg1[3])(c) == 4);

        int& r2 = (arg1[3])(c); // must be an lvalue
        BOOST_TEST(&r2 == &c[3]);

        vector<string> v;
        v.push_back("a");
        v.push_back("b");
        v.push_back("c");
        v.push_back("d");

        BOOST_TEST((arg1[3])(v) == "d");

        map<string, int> m;
        (arg1["Kimpo"] = arg2)(m, x);
        BOOST_TEST(m["Kimpo"] == x);
    }

    {   // From Phoenix 1.1 unary tests

        int i1 = 1, i = 5;

        BOOST_TEST((!val(true))() == false);
        BOOST_TEST((-val(1))() == -1);
        BOOST_TEST((+val(1))() == +1);
        BOOST_TEST((~val(1))() == ~1);
        BOOST_TEST(*(&arg1)(i1) == *(&i1));
        BOOST_TEST((&arg1)(i1) == &i1);

        BOOST_TEST((*val(&i1))() == *(&i1));
        BOOST_TEST((*&arg1)(i1) == *(&i1));
        BOOST_TEST((++var(i))() == 6);
        BOOST_TEST((--var(i))() == 5);
        BOOST_TEST((var(i)++)() == 5);
        BOOST_TEST(i == 6);
        BOOST_TEST((var(i)--)() == 6);
        BOOST_TEST(i == 5);
    }

    {   // From Phoenix 1.1 binary tests

        int i2 = 2, i3 = 3, i = 5;
        const char* world = " world";

        BOOST_TEST((var(i) = var(i))() == 5);
        BOOST_TEST((var(i) = 3)() == 3);
        BOOST_TEST(i == 3);
        i = 5;
        int x, y, z;
        (var(x) = var(y) = var(z) = 10)();
        BOOST_TEST(x == 10 && y == 10 && z == 10);
        BOOST_TEST((val(world)[3])() == world[3]);

        BOOST_TEST((var(i) += 5)() == 10);
        BOOST_TEST((var(i) -= 5)() == 5);
        BOOST_TEST((var(i) *= 5)() == 25);
        BOOST_TEST((var(i) /= 5)() == 5);
        BOOST_TEST((var(i) %= 2)() == 1);

        BOOST_TEST((var(i) <<= 3)() == 8);
        BOOST_TEST((var(i) >>= 1)() == 4);
        BOOST_TEST((var(i) |= 0xFF)() == 0xFF);
        BOOST_TEST((var(i) &= 0xF0)() == 0xF0);
        BOOST_TEST((var(i) ^= 0xFFFFFFFF)() == int(0xFFFFFF0F));

        BOOST_TEST((val(5) == val(5))());
        BOOST_TEST((val(5) == 5)());

        BOOST_TEST((arg1 + arg2)(i2, i3) == i2 + i3);
        BOOST_TEST((arg1 - arg2)(i2, i3) == i2 - i3);
        BOOST_TEST((arg1 * arg2)(i2, i3) == i2 * i3);
        BOOST_TEST((arg1 / arg2)(i2, i3) == i2 / i3);
        BOOST_TEST((arg1 % arg2)(i2, i3) == i2 % i3);
        BOOST_TEST((arg1 & arg2)(i2, i3) == (i2 & i3));
        BOOST_TEST((arg1 | arg2)(i2, i3) == (i2 | i3));
        BOOST_TEST((arg1 ^ arg2)(i2, i3) == (i2 ^ i3));
        BOOST_TEST((arg1 << arg2)(i2, i3) == i2 << i3);
        BOOST_TEST((arg1 >> arg2)(i2, i3) == i2 >> i3);

        BOOST_TEST((val(5) != val(6))());
        BOOST_TEST((val(5) < val(6))());
        BOOST_TEST(!(val(5) > val(6))());
        BOOST_TEST((val(5) < val(6))());
        BOOST_TEST((val(5) <= val(6))());
        BOOST_TEST((val(5) <= val(5))());
        BOOST_TEST((val(7) >= val(6))());
        BOOST_TEST((val(7) >= val(7))());

        BOOST_TEST((val(false) && val(false))() == false);
        BOOST_TEST((val(true) && val(false))() == false);
        BOOST_TEST((val(false) && val(true))() == false);
        BOOST_TEST((val(true) && val(true))() == true);

        BOOST_TEST((val(false) || val(false))() == false);
        BOOST_TEST((val(true) || val(false))() == true);
        BOOST_TEST((val(false) || val(true))() == true);
        BOOST_TEST((val(true) || val(true))() == true);
    }

    {  // From Phoenix 1.1 mixed_binary tests

        int     i1 = 1, i2 = 2, i50 = 50, i100 = 100;
        double  d2_5 = 2.5;
        string hello = "hello";
        const char* world = " world";

        BOOST_TEST((arg1 + arg2)(i100, i50) == (i100 + i50));
        BOOST_TEST((arg1 + 3)(i100) == (3 + i100));
        BOOST_TEST((arg1 + arg2)(hello, world) == "hello world");
        BOOST_TEST((arg1 + arg2)(i1, d2_5) == (i1 + d2_5));

        BOOST_TEST((*(arg1 + arg2))(world, i2) == *(world + i2));
        BOOST_TEST((*(arg1 + arg2))(i2, world) == *(i2 + world));
        BOOST_TEST((*(val(world+i2) - arg1))(i2) == *world);
    }

    return 0;
}
