//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/string_algo/case_conv.hpp>
#include <boost/string_algo/find.hpp>

using namespace std;
using namespace boost;

int main()
{  
    cout << "* Find Example *" << endl << endl;

    string str1("abc___cde___efg");
    string str2("abc");

    // find "cde" substring
    iterator_range<string::iterator> range=find_first( str1, string("cde") );

    // convert a substring to upper case 
    // note that iterator range can be directly passed to the algorithm
    to_upper( range );

    cout << "str1 with upper-cased part matching cde: " << str1 << endl;

    // get a head of the string
    iterator_range<string::iterator> head=find_head( str1, 3 );
    cout << "head(3) of the str1: " << string( head.begin(), head.end() ) << endl;

	// get the tail
    head=find_tail( str2, 5 );
    cout << "tail(5) of the str2: " << string( head.begin(), head.end() ) << endl;

	// char processing
	char text[]="hello dolly!";
	iterator_range<char*> crange=find_last(text,"ll");

	// transform the range ( add 1 )
	transform( crange.begin(), crange.end(), crange.begin(), bind2nd( plus<char>(), 1 ) );
	// uppercase the range
	to_upper( crange );

	cout << text << endl;

    cout << endl;

	return 0;
}
