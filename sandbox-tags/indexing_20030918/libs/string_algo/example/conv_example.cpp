//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <boost/string_algo/case_conv.hpp>

using namespace std;
using namespace boost;

void conv_example()
{  
    cout << "* Case Conversion Example *" << endl << endl;

    string str1("AbCdEfG");
    vector<char> vec1( str1.begin(), str1.end() );

    cout << "lower-cased copy of vec1: ";
    tolower_copy( ostream_iterator<char>(cout), vec1.begin(), vec1.end() );
    cout << endl;

    cout << "upper-cased copy of str1: " << toupper_copy( str1 ) << endl;

    tolower( str1 );
    cout << "lower-cased str1: " << str1 << endl;

    cout << endl;
}
