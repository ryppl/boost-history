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

    return 0;
}
