//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/test/unit_test.hpp>
#include <boost/test/minimal.hpp>

#include <boost/tree/balanced_tree.hpp>
#include <boost/tree/balancers/unbalanced.hpp>
#include <boost/tree/binary_tree.hpp>

//#include <boost/tree/searcher.hpp>
//#include <boost/tree/algorithm.hpp>

#include "helpers.hpp"

//TODO: Make this a test suite.


void test_balanced_binary_tree()
{
	using namespace boost::tree;
	
	typedef balanced_tree<int, binary_tree<int>, balancers::unbalanced> tree_t;
	tree_t my_tree;
	
	tree_t::iterator c, c1, c2, c3, c4, c5;

	c = my_tree.end();
	BOOST_CHECK(c == my_tree.end());
	BOOST_CHECK(c == my_tree.begin());
		
	c1 = my_tree.insert(c, 8);
	
	BOOST_CHECK(*c1 == 8);

}

//boost::unit_test::test_suite*
//init_unit_test_suite( int argc, char* argv[] )
//{
//    boost::unit_test::test_suite* key_search_test = 
//    		BOOST_TEST_SUITE( "Key search binary vector test" );
//
//    key_search_test->add( BOOST_TEST_CASE( &key_search_binary_tree_test ) );
//
//    return key_search_test;
//}

int test_main(int, char* [])
{
	test_balanced_binary_tree();

	return 0;
}
