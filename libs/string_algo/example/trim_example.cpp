//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/string_algo/trim.hpp>
#include <boost/string_algo/trim2.hpp>
#include <boost/string_algo/classification.hpp>

using namespace std;
using namespace boost;
namespace sa=boost::string_algo;

int main()
{
    cout << "* Trim Example *" << endl << endl;

    string str1("     1x x x x1     ");
    string str2("<>trim<>");
    string str3("123abs343");

	// Simple left trim
    cout << "trim_left copy of str1: " << "\"" << trim_left_copy( str1 ) << "\"" << endl;

	// Inplace right trim
    trim_right( str1 );
    cout << "trim_right on str1: " << "\"" << str1 << "\"" << endl;

	// Paramteric trim. 'Space' is defined using is_of predicate
    cout 
        << "trimmed copy of str4 ( space='<>' ): " 
		<< "\""<< sa::trim_copy( str2, is_of<char>("<>") ) << "\"" << endl;
    

	// Paramteric trim. 'Space' is defined using is_digit predicate
    cout 
        << "trimmed copy of str5 ( space=digit ): " 
		<< "\"" << sa::trim_copy( str3, is_digit<char>() ) << "\"" << endl;

    cout << endl;

	return 0;
}
