/*=============================================================================
    Phoenix
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <boost/test/minimal.hpp>
#include <boost/spirit/phoenix/statement/sequence.hpp>
#include <boost/spirit/phoenix/core/argument.hpp>

using namespace boost::phoenix;
using namespace std;

int
test_main(int, char*[])
{
    //  testing statement
    int i = 3;
    (arg1, arg1)(i);

    return 0;
}
