//  Boost string_algo library iterator_test.cpp file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <vector>
#include <iostream>

#include <boost/test/test_tools.hpp>
#include <boost/string_algo/split.hpp>
#include <boost/string_algo/classification.hpp>

// equals predicate is used for result comparison
#include <boost/string_algo/predicate.hpp>


using namespace std;
using namespace boost;

namespace sa=boost::string_algo;

template< typename T1, typename T2 >
void deep_compare( const T1& X, const T2& Y )
{
	BOOST_REQUIRE( X.size() == Y.size() );
	for( unsigned int nIndex=0; nIndex<X.size(); nIndex++ )
	{
		BOOST_CHECK( equals( X[nIndex], Y[nIndex] ) );
	}
}

void iterator_test()
{
	string str1("xx-abc--xx-abb");
	string str2("xx");
	char* pch1="xx-abc--xx-abb";
	vector<string> tokens;
	vector< vector<int> > vtokens;

	// find_all tests
	tokens=transform_range< vector<string> >( 
		find_all(str1,str2), 
		sa::copy_rangeF<string>() );
	
	BOOST_REQUIRE( tokens.size()==2 );
	BOOST_CHECK( tokens[0]==string("xx") );
	BOOST_CHECK( tokens[1]==string("xx") );

	tokens=transform_range< vector<string> >( 
		find_all(pch1,"xx"), 
		sa::copy_rangeF<string>() );
	
	BOOST_REQUIRE( tokens.size()==2 );
	BOOST_CHECK( tokens[0]==string("xx") );
	BOOST_CHECK( tokens[1]==string("xx") );

	find_all_copy(
		tokens,
		str1,
		string("xx") );

	BOOST_REQUIRE( tokens.size()==2 );
	BOOST_CHECK( tokens[0]==string("xx") );
	BOOST_CHECK( tokens[1]==string("xx") );

	find_all_copy(
		tokens,
		pch1,
		"xx" );

	BOOST_REQUIRE( tokens.size()==2 );
	BOOST_CHECK( tokens[0]==string("xx") );
	BOOST_CHECK( tokens[1]==string("xx") );

	find_all_copy(
		vtokens,
		str1,
		string("xx") );
	deep_compare( tokens, vtokens );

	// split tests
	tokens=transform_range< vector<string> >( 
		split(
			str1,
			is_of<char>("x") ), 
		sa::copy_rangeF<string>() );
	
	BOOST_REQUIRE( tokens.size()==3 );
	BOOST_CHECK( tokens[0]==string("") );
	BOOST_CHECK( tokens[1]==string("-abc--") );
	BOOST_CHECK( tokens[2]==string("-abb") );

	tokens=transform_range< vector<string> >( 
		split(
			pch1,
			is_of<char>("x") ), 
		sa::copy_rangeF<string>() );
	
	BOOST_REQUIRE( tokens.size()==3 );
	BOOST_CHECK( tokens[0]==string("") );
	BOOST_CHECK( tokens[1]==string("-abc--") );
	BOOST_CHECK( tokens[2]==string("-abb") );

	split_copy(
		tokens,
		str1,
		is_of<char>("x") );

	BOOST_REQUIRE( tokens.size()==3 );
	BOOST_CHECK( tokens[0]==string("") );
	BOOST_CHECK( tokens[1]==string("-abc--") );
	BOOST_CHECK( tokens[2]==string("-abb") );

	split_copy(
		tokens,
		pch1,
		is_of<char>("x") );

	BOOST_REQUIRE( tokens.size()==3 );
	BOOST_CHECK( tokens[0]==string("") );
	BOOST_CHECK( tokens[1]==string("-abc--") );
	BOOST_CHECK( tokens[2]==string("-abb") );

	split_copy(
		vtokens,
		str1,
		is_of<char>("x") );
	deep_compare( tokens, vtokens );

	split_copy(
		tokens,
		str1,
		is_punct<char>(),
		false );

	BOOST_REQUIRE( tokens.size()==5 );
	BOOST_CHECK( tokens[0]==string("xx") );
	BOOST_CHECK( tokens[1]==string("abc") );
	BOOST_CHECK( tokens[2]==string("") );
	BOOST_CHECK( tokens[3]==string("xx") );
	BOOST_CHECK( tokens[4]==string("abb") );
}

// test main 
int test_main( int, char*[] )
{
    iterator_test();
    
    return 0;
}
