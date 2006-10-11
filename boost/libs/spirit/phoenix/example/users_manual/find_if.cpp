/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/spirit/phoenix/core.hpp>
#include <boost/spirit/phoenix/operator.hpp>

using namespace boost::phoenix;
using namespace std;

int
main()
{
    int init[] = { 2, 10, 4, 5, 1, 6, 8, 3, 9, 7 };
    vector<int> c(init, init + 10);
    typedef vector<int>::iterator iterator;

    //  Find the first odd number in container c
    iterator it = find_if(c.begin(), c.end(), arg1 % 2 == 1);

    if (it != c.end())
        cout << *it;    //  if found, print the result
    return 0;
}
