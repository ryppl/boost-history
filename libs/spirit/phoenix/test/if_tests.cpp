/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/test/minimal.hpp>
#include <boost/spirit/phoenix/statement/if.hpp>
#include <boost/spirit/phoenix/statement/sequence.hpp>
#include <boost/spirit/phoenix/operator.hpp>
#include <boost/spirit/phoenix/core.hpp>

using namespace boost::phoenix;
using namespace std;

int
test_main(int, char*[])
{
    int init[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<int> v(init, init+10);

    cout << dec;
    int x = 0;

    for_each(v.begin(), v.end(),
        if_(arg1 > 3 && arg1 <= 8)
        [
            cout << arg1 << ", ",
            var(x) += arg1
        ]
    );

    cout << endl;
    BOOST_TEST(x == 4+5+6+7+8);

    x = 0;
    int y = 0;
    int z = 0;

    for_each(v.begin(), v.end(),
        if_(arg1 > 5)
        [
            cout << arg1 << " > 5\n",
            var(x) += arg1
        ]
        .else_
        [
            if_(arg1 == 5)
            [
                cout << arg1 << " == 5\n",
                var(z) += arg1
            ]
            .else_
            [
                cout << arg1 << " < 5\n",
                var(y) += arg1
            ]
        ]
    );

    cout << endl;
    BOOST_TEST(x == 6+7+8+9+10);
    BOOST_TEST(y == 1+2+3+4);
    BOOST_TEST(z == 5);

    return 0;
}
