//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <iterator>
#include <boost/string_algo/replace.hpp>
#include <boost/string_algo/erase.hpp>

using namespace std;
using namespace boost;

void replace_example()
{  
    cout << "* Replace Example *" << endl << endl;

    string str1("abc___cde___efg");

    cout << "str1 without 6th to 9th character:" <<
        erase_range_copy( 
            str1, 
            str1.begin()+6,
            str1.begin()+9 ) << endl;

    cout << "str1 with 6th to 9th character replaced with '+++': " << 
        replace_range_copy( 
            str1, 
            str1.begin()+6,
            str1.begin()+9,
            string("+++") ) << endl;

    cout << "str1 with 'cde' replaced with 'XYZ': ";
    
    replace_first_copy( 
        ostream_iterator<char>(cout),
        str1,
        string("cde"),
        string("XYZ") );
    cout << endl;

    cout << "str1 with all '___' replaced with '---': " << 
        replace_all_copy( str1, string("___"), string("---") ) << endl;

    cout << "str1 without all '___': " << 
        erase_all_copy( str1, string("___") ) << endl;

    // replace third and 5th occurence of _ in str1
    // note that nth argument is 0-based
    replace_nth( str1, string("_"), 4, string("+") );
    replace_nth( str1, string("_"), 2, string("+") );

    cout << "str1 with third and 5th occurence of _ replace: " << str1 << endl;

    cout << endl;
}
