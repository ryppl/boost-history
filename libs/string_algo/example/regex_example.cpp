//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <iterator>
#include <boost/regex.hpp>
#include <boost/string_algo/regex.hpp>

using namespace std;
using namespace boost;

int main()
{  
    cout << "* Regex Example *" << endl << endl;

    string str1("abc__(456)__123__(123)__cde");

    // Replace all substrings matching (digit+) 
    cout << 
        "replace all (digit+) in str1 with #digit+# :" <<
        replace_all_regex_copy( str1, regex("\\(([0-9]+)\\)"), string("#$1#") ) << endl;
    
    // Erase all substrings matching (digit+) 
    cout << 
        "remove all sequences of letters from str1 :" <<
        erase_all_regex_copy( str1, regex("[[:alpha:]]+") ) << endl;

    // in-place regex transformation
    replace_all_regex( str1, regex("_(\\([^\\)]*\\))_"), string("-$1-") );
    cout << "transformad str1: " << str1 << endl;

    cout << endl;

    return 0;
}
