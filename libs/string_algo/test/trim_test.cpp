//  Boost string_algo library trim_test.cpp file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/string_algo.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void trim_test()
{
	string str1("     1x x x x1     ");
	string str2("     2x x x x2     ");
	string str3("    ");

	// *** value passing tests *** //

	// general string test
	BOOST_CHECK( ltrim( str1 )=="1x x x x1     " ) ;
	BOOST_CHECK( rtrim( str1 )=="     1x x x x1" ) ;
	BOOST_CHECK( trim( str1 )=="1x x x x1" ) ;

	// spaces-only string test
	BOOST_CHECK( ltrim( str3 )=="" );
	BOOST_CHECK( rtrim( str3 )=="" );
	BOOST_CHECK( trim( str3 )=="" );

	// empty string check 
	BOOST_CHECK( ltrim( string("") )=="" );
	BOOST_CHECK( rtrim( string("") )=="" );
	BOOST_CHECK( trim( string("") )=="" );

	// *** inplace tests *** //

	// general string test
	ltrim_in( str1 );
	BOOST_CHECK( str1=="1x x x x1     " );
	rtrim_in( str1 );
	BOOST_CHECK( str1=="1x x x x1" );
	trim_in( str2 );
	BOOST_CHECK( str2=="2x x x x2" );
	
	// spaces-only string test
	str3 = "    "; ltrim_in( str3 );
	BOOST_CHECK( str3=="" );
	str3 = "    "; rtrim_in( str3 );
	BOOST_CHECK( str3=="" );
	str3 = "    "; trim_in( str3 );
	BOOST_CHECK( str3=="" );

	// empty string check 
	str3 = ""; ltrim_in( str3 );
	BOOST_CHECK( str3=="" );
	str3 = ""; rtrim_in( str3 );
	BOOST_CHECK( str3=="" );
	str3 = ""; trim_in( str3 );
	BOOST_CHECK( str3=="" );
}
