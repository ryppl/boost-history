//
// short: Test cases for container algortihms.
//
//    

// (C) Copyright Thorsten Ottosen 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#include "../container_algo.hpp"

#include <functional>
#include <cassert>
#include <list>
#include <iostream>
#include <string>
#include <vector>

namespace 
{
	//
	// Data types:
	//
	// - pair
	// - STL container
	// - array    
	//

	//
	// Test for:
	//
	// - ambiguity with std algorithms
	// - same result as std algorithms
	// - ability to handle data types
	// - all iterators are non-const?
	//    

	void test_for_each();
	void test_find();
	void test_find_if();
	void test_adjacent_find();
	void test_find_first_of();
	void test_count();
	void test_count_if();
	void test_mismatch();
	void test_equak();
	void test_search();
	void test_search_n();
	void test_find_end();

}


int main()
{
	test_for_each();
	test_find();
	test_find_if();
	test_adjacent_find();
	test_find_first_of();
	test_count();
	test_count_if();
	test_mismatch();
	test_equak();
	test_search();
	test_search_n();
	test_find_end();
	
	return 0;
} // 'main()'

namespace 
{

	//////////////////////////////////////////////////////////////////////////////
	// Implementation
	//////////////////////////////////////////////////////////////////////////////

	// enable ambiguity conflicts
	using namespace std;
	using namespace boost;

	#define TEST_DATA                                                        \
        pair<int, int>      p( 1, 2 );                                       \
		const pair<int,int> cp( 2, 3);                                       \
		int                 a[] = { 1, 2, 4, 2, 6, 7, 9, 3, 2, 5, 5, 3, 3 }; \
		const int           ca[] = { 1, 2, 3, 5, 7, 9, 11 };                 \
		vector<int>         v( a, a + sizeof( a ) / sizeof( a[0] ) );        \
		const vector<int>   cv( v );


	int    unary_function( int i );
	bool   predicate( int i );
	bool   binary_predicate( int i1, int i2 );



	int unary_function( int i )
	{
		return ++i;
	}



	bool predicate( int i )
	{
		return i == 5;
	}



	bool binary_predicate( int x, int y ) 
	{
		return abs( x ) == abs( y );
	}



	void test_for_each()
	{
		TEST_DATA;
		//for_each( p, ptr_fun<int,int>( &unary_function ) );  
		//for_each( cp, ptr_fun<int,int>( &unary_function ) );  
		for_each( a, unary_function );
		for_each( ca, unary_function );
		for_each( v, ptr_fun<int,int>( &unary_function ) );
		for_each( cv, ptr_fun<int,int>( &unary_function ) );
	}

	
	
	void test_find()
	{
		TEST_DATA;
//		find( a
	}
	
	void test_find_if()
	{
	}
	void test_adjacent_find()
	{
	}
	void test_find_first_of()
	{
	}
	void test_count()
	{
	}
	void test_count_if()
	{
	}
	void test_mismatch()
	{
	}
	void test_equak()
	{
	}
	void test_search()
	{
	}
	void test_search_n()
	{
	}
	void test_find_end()
	{
	}

} // namespace ''



