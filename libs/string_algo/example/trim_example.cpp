//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/string_algo/trim.hpp>

using namespace std;
using namespace boost;

void trim_example()
{
    cout << "* Trim Example *" << endl << endl;

    string str1("     1x x x x1     ");
    string str2("<>trim<>");
    string str3("123abs343");

    cout << "trim_left copy of str1: " << "\"" << trim_left_copy( str1 ) << "\"" << endl;

    trim_right( str1 );
    cout << "trim_right on str1: " << "\"" << str1 << "\"" << endl;

    cout 
        << "trimmed copy of str4 ( space='<>' ): " 
        << "\""<< trim_copy_if( str2, isfrom( string("<>") ) ) << "\"" << endl;
    
    cout 
        << "trimmed copy of str5 ( space=digit ): " 
        << "\"" << trim_copy_if( str3, isclassified<char>(ctype_base::digit) ) << "\"" << endl;

    cout << endl;
}
