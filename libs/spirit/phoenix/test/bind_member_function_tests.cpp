/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <cmath>
#include <boost/test/minimal.hpp>
#include <boost/spirit/phoenix/core.hpp>
#include <boost/spirit/phoenix/operator.hpp>
#include <boost/spirit/phoenix/bind.hpp>

using namespace boost::phoenix;
using namespace std;
namespace phx = boost::phoenix;

namespace test
{
    struct x
    {
        void
        test()
        {
            cout << "Test binding member functions...\n";
        }
    };

    struct y
    {
        int
        negate(int n)
        {
            return -n;
        }
    };

    struct z
    {
        int
        plus(int a, int b)
        {
            return a + b;
        }
    };

    struct zz
    {
        int
        plus3(int a, int b, int c)
        {
            return a + b + c;
        }
    };
}

int
test_main(int, char*[])
{
    int a = 123;
    int b = 256;
    test::x x_;
    test::y y_;
    test::z z_;
    test::zz zz_;

    bind(&test::x::test, var(x_))();
    BOOST_TEST(bind(&test::y::negate, var(y_), arg1)(a) == -a);
    BOOST_TEST(bind(&test::z::plus, arg1, arg2, arg3)(z_, a, b) == a+b);
    BOOST_TEST(bind(&test::zz::plus3, var(zz_), arg1, arg2, arg3)(a, b, a) == a+b+a);
    BOOST_TEST(bind(&test::y::negate, &y_, 777)(a) == -777);

    return 0;
}
