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
#include <boost/spirit/phoenix/function.hpp>

using namespace boost::phoenix;
using namespace std;

    struct test_impl
    {
        typedef void result_type;
        void operator()() const
        {
            cout << "Test lazy functions...\n";
        }
    };

    function<test_impl> test;

    struct sqr_impl
    {
        template <typename Arg>
        struct apply
        {
            typedef Arg type;
        };

        template <typename Arg>
        Arg operator()(Arg n) const
        {
            return n * n;
        }
    };

    function<sqr_impl> sqr;

    struct fact_impl
    {
        template <typename Arg>
        struct apply
        {
            typedef Arg type;
        };

        template <typename Arg>
        Arg operator()(Arg n) const
        {
            return (n <= 0) ? 1 : n * (*this)(n-1);
        }
    };

    function<fact_impl> fact;

    struct pow_impl
    {
        template <typename Arg1, typename Arg2>
        struct apply
        {
            typedef Arg1 type;
        };

        template <typename Arg1, typename Arg2>
        Arg1 operator()(Arg1 a, Arg2 b) const
        {
            return pow(a, b);
        }
    };

    function<pow_impl> power;

int
test_main(int, char*[])
{
    int     i5 = 5;
    double  d5 = 5, d3 = 3;

    test()();
    BOOST_TEST(sqr(arg1)(i5) == (i5*i5));
    BOOST_TEST(fact(4)() == 24);
    BOOST_TEST(fact(arg1)(i5) == 120);
    BOOST_TEST((int)power(arg1, arg2)(d5, d3) == (int)pow(d5, d3));
    BOOST_TEST((sqr(arg1) + 5)(i5) == ((i5*i5)+5));

    return 0;
}
