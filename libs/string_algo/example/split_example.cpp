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
#include <functional>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/classification.hpp>
#include <boost/string_algo/split.hpp>

using namespace std;
using namespace boost;

namespace sa = boost::string_algo;

int main()
{  
    cout << "* Split Example *" << endl << endl;

    string str1("abc-*-ABC-*-aBc");

	cout << "Before: " << str1 << endl;

    // find all 'abc' substrings ( ignoring the case )
	// We will have reference to the input
	vector< iterator_range<string::iterator> > Result;
	ifind_all( Result, str1, "abc" );

	// do something with matches
	unsigned int nIndex=0;
	for( nIndex=0; nIndex<Result.size(); nIndex++ )
	{
		cout << nIndex << ":" << copy_range<std::string>( Result[nIndex] ) << endl;
		// shift all chars in the match by one
		transform( 
			Result[nIndex].begin(), Result[nIndex].end(), 
			Result[nIndex].begin(), 
			bind2nd( plus<char>(), 1 ) );
	}

	// Print the string now
	cout << "After: " << str1 << endl;
	
	// Split the string into tokens ( use '-' and '*' as delimiters )
	// We need copies of the input only, and adjencent tokens are compressed
	vector< std::string > ResultCopy;
	split( ResultCopy, str1, is_of<char>("-*") );

	for( nIndex=0; nIndex<Result.size(); nIndex++ )
	{
		cout << nIndex << ":" << ResultCopy[nIndex] << endl;
	};

	cout << endl;

	return 0;
}
