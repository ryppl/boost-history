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
        int m;
    };
}

int
test_main(int, char*[])
{
    test::x x_;
    bind(&test::x::m, var(x_))() = 123;
    BOOST_TEST(x_.m == 123);

    return 0;
}
