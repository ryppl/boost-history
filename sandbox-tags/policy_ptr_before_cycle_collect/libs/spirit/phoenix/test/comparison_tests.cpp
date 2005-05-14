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
        BOOST_TEST(!(val(123) == 456)());
        BOOST_TEST((val(123) != 456)());
        BOOST_TEST(!(val(123) > 456)());
        BOOST_TEST((val(123) < 456)());
        BOOST_TEST(!(val(123) > 456)());
        BOOST_TEST((val(123) <= 123)());
        BOOST_TEST((val(123) >= 123)());
    }

    return 0;
}
