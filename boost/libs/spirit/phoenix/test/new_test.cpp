/*=============================================================================
    Phoenix v1.1
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2003 Vaclav Vesely

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
==============================================================================*/
#include <iostream>
#include <cassert>

#define PHOENIX_LIMIT 15
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/new.hpp>

using namespace phoenix;
using namespace std;

class X
{
public:
    X(int i_ = 1)
    :   i(i_)
    {}

    int i;
};

///////////////////////////////////////////////////////////////////////////////
int
main()
{
    int i2 = 2;
    X   x3(3);

    assert(new_<int>()() != NULL);
    assert(*new_<int>(arg1)(i2) == 2);

    assert(new_<X>()() != NULL);
    assert(new_<X>()()->i == 1);
    assert(new_<X>(arg1)(i2)->i == 2);
    assert(new_<X>(arg1)(x3)->i == 3);

    cout << "///////////////////////////////////////////////////////////////////////////////\n";
    cout << "\t\tTests concluded\n";
    cout << "\t\tSUCCESS!!!\n";
    cout << "///////////////////////////////////////////////////////////////////////////////\n";
}
