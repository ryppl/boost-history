/*=============================================================================
    Phoenix v2.0
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <iostream>
#include <boost/test/minimal.hpp>
#include <boost/spirit/statement/sequence.hpp>
#include <boost/spirit/core/argument.hpp>

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
