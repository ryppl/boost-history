/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <string>

#include <boost/spirit/phoenix/core/argument.hpp>
#include <boost/spirit/phoenix/core/value.hpp>
#include <boost/spirit/phoenix/core/variable.hpp>
#include <boost/test/minimal.hpp>

using namespace boost::phoenix;
using namespace std;

int
test_main(int, char*[])
{
    char c1 = '1';
    int i1 = 1, i2 = 2, i = 4;
    const char* s2 = "2";

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Values, references and arguments
    //
    ///////////////////////////////////////////////////////////////////////////

    //  argument
    BOOST_TEST(arg1(c1) == c1);
    BOOST_TEST(arg1(i1, i2) == i1);
    BOOST_TEST(arg2(i1, s2) == s2);

    //  value
    cout << val("Hello,")() << val(' ')() << val("World")() << endl;
    BOOST_TEST(val(3)() == 3);
    BOOST_TEST(val("Hello, world")() == std::string("Hello, world"));

    //  reference
    BOOST_TEST(invar(i)() == var(i)());
    BOOST_TEST(invar(i)() == 4);
    BOOST_TEST(i == 4);
    BOOST_TEST(var(++i)() == 5);
    BOOST_TEST(i == 5);

    // should not compile:
#ifdef PHOENIX_SHOULD_NOT_COMPILE_TEST
    var(arg1);
#endif

    return 0;
}
