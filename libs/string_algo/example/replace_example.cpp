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
#include <boost/string_algo/case_conv.hpp>

//Following two includes contain second-layer function.
//They are already included by first-layer header

//#include <boost/string_algo/replace2.hpp>
//#include <boost/string_algo/find2.hpp>

using namespace std;
using namespace boost;
namespace sa=boost::string_algo;

// uppercase formatter
/*
    Convert an input to upper case. 
    Note, that this formatter can be used only on std::string inputs.
*/
inline string upcase_formatter( 
    const iterator_range<string::const_iterator>& Replace )
{
    string Temp(Replace.begin(), Replace.end());
    return to_upper(Temp);
}

int main()
{  
    cout << "* Replace Example *" << endl << endl;

    string str1("abc___cde___efg");

    // Erase 6-9th characters from the string
    cout << "str1 without 6th to 9th character:" <<
        erase_range_copy( str1, make_range(str1.begin()+6, str1.begin()+9) ) << endl;

    // Replace 6-9th character with '+++'
    cout << "str1 with 6th to 9th character replaced with '+++': " << 
        replace_range_copy( 
            str1, make_range(str1.begin()+6, str1.begin()+9), "+++" ) << endl;

    cout << "str1 with 'cde' replaced with 'XYZ': ";
    
    // Replace first 'cde' with 'XYZ'. Modify the input
    replace_first_copy( ostream_iterator<char>(cout), str1, "cde", "XYZ" );
    cout << endl;
    
    // Replace all '___'
    cout << "str1 with all '___' replaced with '---': " << 
        replace_all_copy( str1, "___", "---" ) << endl;

    // Erase all '___'
    cout << "str1 without all '___': " << 
        erase_all_copy( str1, "___" ) << endl;

    // replace third and 5th occurence of _ in str1
    // note that nth argument is 0-based
    replace_nth( str1, "_", 4, "+" );
    replace_nth( str1, "_", 2, "+" );

    cout << "str1 with third and 5th occurence of _ replace: " << str1 << endl;

    // Custom formatter examples
    string str2("abC-xxxx-AbC-xxxx-abc");

    // Find string 'abc' ignoring the case and convert it to upper case
    cout << "Upcase all 'abc'(s) in the str2: " <<
        sa::replace_all_copy( 
            str2,
            sa::first_finder("abc", sa::is_iequal()), 
            upcase_formatter );
    
    cout << endl;

    return 0;
}
