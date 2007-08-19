/*=============================================================================
    Copyright (c) 2002-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  This sample demontrates a parser for a comma separated list of numbers
//  This is the phoenix version of number_list.cpp.
//  This is discussed in the "Phoenix" chapter in the Spirit User's Guide.
//
//  [ JDG 1/12/2004 ]
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/functions.hpp>
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost::spirit;
using namespace phoenix;

///////////////////////////////////////////////////////////////////////////////
//
//  Our comma separated list parser
//
///////////////////////////////////////////////////////////////////////////////
struct push_back_impl
{
    template <typename Container, typename Item>
    struct result
    {
        typedef void type;
    };

    template <typename Container, typename Item>
    void operator()(Container& c, Item const& item) const
    {
        c.push_back(item);
    }
};

function<push_back_impl> const push_back = push_back_impl();

bool
parse_numbers(char const* str, vector<double>& v)
{
    return parse(str,

        //  Begin grammar
        (
            real_p[push_back(var(v), arg1)]
                >> *(',' >> real_p[push_back(var(v), arg1)])
        )
        ,
        //  End grammar

        space_p).full;
}

////////////////////////////////////////////////////////////////////////////
//
//  Main program
//
////////////////////////////////////////////////////////////////////////////
int
main()
{
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "\t\tA comma separated list parser for Spirit...\n\n";
    cout << "/////////////////////////////////////////////////////////\n\n";

    cout << "Give me a comma separated list of numbers.\n";
    cout << "The numbers will be inserted in a vector of numbers\n";
    cout << "Type [q or Q] to quit\n\n";

    string str;
    while (getline(cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        vector<double> v;
        if (parse_numbers(str.c_str(), v))
        {
            cout << "-------------------------\n";
            cout << "Parsing succeeded\n";
            cout << str << " Parses OK: " << endl;

            for (vector<double>::size_type i = 0; i < v.size(); ++i)
                cout << i << ": " << v[i] << endl;

            cout << "-------------------------\n";
        }
        else
        {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
        }
    }

    cout << "Bye... :-) \n\n";
    return 0;
}


