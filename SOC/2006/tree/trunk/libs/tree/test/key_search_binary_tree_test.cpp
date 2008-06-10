//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/test/unit_test.hpp>
#include <boost/test/minimal.hpp>

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balancers/unbalanced.hpp>

#include <boost/tree/searcher.hpp>
#include <boost/tree/algorithm.hpp>

#include "helpers.hpp"

//TODO: Make this a test suite.

void test_key_search_binary_tree()
{
	using namespace boost::tree;
	
	typedef test_searcher<false, balanced_tree<binary_tree<int>, balancers::unbalanced> > searcher_t;
	searcher_t my_tree;
	
	searcher_t::iterator c, c1, c2, c3, c4, c5;
	//std::pair<searcher_t::iterator, std::pair<bool, bool> > ret;

	c = my_tree.end();
	BOOST_CHECK(c == my_tree.end());
	BOOST_CHECK(c == my_tree.begin());
	
//	searcher_t::cursor cur = searcher_t::cursor(c);
//	BOOST_CHECK(cur.empty());
//	BOOST_CHECK(cur == searcher_t::cursor(my_tree.end()));
		
	c1 = my_tree.insert(c, 8);
	
	BOOST_CHECK(*c1 == 8);
//	BOOST_CHECK(searcher_t::cursor(c1).parity() == 0);
	BOOST_CHECK(c != my_tree.end());
	BOOST_CHECK(c1 != my_tree.end());
	
//	cur = searcher_t::cursor(c1);
//	BOOST_CHECK((++cur).empty());
//	BOOST_CHECK(cur.parity());
//	
//	cur = cur.parent(); //header-cursor(,1) (root)
//	BOOST_CHECK(!cur.parity());
//	BOOST_CHECK(searcher_t::iterator(cur) == my_tree.end());	
	BOOST_CHECK(*c1 = 8);

	BOOST_CHECK(++c1 == my_tree.end());


	--c1;
	BOOST_CHECK(*c1 == 8);
	
//	BOOST_CHECK(searcher_t::cursor(my_tree.end()).parity() == 1);
//
//	BOOST_CHECK(cur.end().parity() == 1);
//	
//	cur = searcher_t::cursor(c1);
//	
//	BOOST_CHECK(*cur == 8);
//	
//	BOOST_CHECK((++cur).empty());
//	BOOST_CHECK(!(--cur).parent().parity()); // root's parity...
//
//	BOOST_CHECK(*(searcher_t::cursor(c).begin()) == 8);
	
	BOOST_CHECK(*c1 == 8);
	BOOST_CHECK(++c1 == my_tree.end());
	
	// root (e.g. c) instead of c1 would crash this. but should that be really 
	// illegal?
	c2 = my_tree.insert(c1, 18); 

	BOOST_CHECK(*c2 == 18);

	++c2;
	BOOST_CHECK(c2 == my_tree.end());
	
	c = my_tree.end();
	--c;
	BOOST_CHECK(*c == 18);

	c2 = my_tree.insert(c, 31);

	c2 = my_tree.insert(c, 412);
	c3 = my_tree.insert(c, 39);
	
	c4 = my_tree.insert(c, 7);
	
	BOOST_CHECK(*c4 == 7);

	BOOST_CHECK(++c2 == my_tree.end());
	c = my_tree.end();
	--c;

	BOOST_CHECK(*c != 39);
	BOOST_CHECK(*c == 412);
	--c;
	BOOST_CHECK(*c == 39);
	
	c = my_tree.begin();
//	BOOST_CHECK(searcher_t::cursor(c).parity() == 0);
//	BOOST_CHECK(*(searcher_t::cursor(c).parent()) != 412);
	BOOST_CHECK(*c < 413);	
	
//	searcher_t::container_type& the_tree = my_tree.container();
//	searcher_t::cursor tree_cur = boost::tree::lower_bound(the_tree.root(), 
//		39, std::less<int>());
//
//	BOOST_CHECK(tree_cur.empty());
//	BOOST_CHECK((++tree_cur).empty());
//	--tree_cur;
//	BOOST_CHECK(*tree_cur == 39);
//	
//	tree_cur = boost::tree::lower_bound(the_tree.root(), 18);	
//	BOOST_CHECK(*tree_cur == 18);
//	
//	tree_cur = boost::tree::lower_bound(the_tree.root(), 30);	
//	BOOST_CHECK(tree_cur.empty());
//	BOOST_CHECK(!(++tree_cur).empty());
//	--tree_cur;
//	BOOST_CHECK(*tree_cur == 31);
//	
//	tree_cur = boost::tree::lower_bound(the_tree.root(), 3);	
//	BOOST_CHECK(*tree_cur == 7);
	
	c = my_tree.begin();
	BOOST_CHECK(*c++ == 7);
	BOOST_CHECK(*c++ == 8);
	BOOST_CHECK(*c++ == 18);
	BOOST_CHECK(*c++ == 31);
	BOOST_CHECK(*c++ == 39);
	BOOST_CHECK(*c++ == 412);
	BOOST_CHECK(c == my_tree.end());
	BOOST_CHECK(*--c == 412);
	BOOST_CHECK(*--c == 39);
	BOOST_CHECK(*--c == 31);
	BOOST_CHECK(*--c == 18);
	BOOST_CHECK(*--c == 8);
	BOOST_CHECK(*--c == 7);
	BOOST_CHECK(c == my_tree.begin());
	
	while (c != my_tree.end())
		++c;
	
	------c;
	BOOST_CHECK(*c == 31);

	//my_tree.erase(c);

	//BOOST_CHECK(*c == 39);
	
//	tree_cur = boost::tree::lower_bound(the_tree.root(), 412);	
//	BOOST_CHECK(*tree_cur == 412);
//	BOOST_CHECK(*tree_cur.parent() == 18);
//	
//	c = my_tree.begin();
//	BOOST_CHECK(*c++ == 7);
//	BOOST_CHECK(*c++ == 8);
//	BOOST_CHECK(*c++ == 18);
//	BOOST_CHECK(*c++ == 39);
//	BOOST_CHECK(*c++ == 412);
//	BOOST_CHECK(c == my_tree.end());
	

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
	test_key_search_binary_tree();

	return 0;
}
