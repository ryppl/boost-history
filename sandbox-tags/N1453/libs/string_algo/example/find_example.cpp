//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/string_algo/case_conv.hpp>
#include <boost/string_algo/find.hpp>

using namespace std;
using namespace boost;

void find_example()
{  
    cout << "* Find Example *" << endl << endl;

    string str1("abc___cde___efg");

    // find "cde" substring
    iterator_range<string::iterator> range=find_first( str1, string("cde") );

    // convert a substring to upper case 
    // note that iterator range can be directly passed to the algorithm
    toupper( range );

    cout << "str1 with upper-cased part matching cde: " << str1 << endl;

    cout << endl;
}
