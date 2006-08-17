// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/test/unit_test.hpp>
#include <boost/test/minimal.hpp>


#include <boost/tree/binary_tree.hpp>
//#include <boost/tree/augmentors/trivial.hpp>
#include <boost/tree/balancers/trivial.hpp>

#include <boost/tree/searcher.hpp>
#include <boost/tree/algorithm.hpp>

//TODO: Make this a test suite.


void test_key_search_binary_tree()
{
	using boost::tree::searcher;
	using boost::tree::binary_tree;
	using boost::tree::lower_bound;
	
	typedef searcher<false, binary_tree<int> > searcher_t;
	searcher_t my_tree;
	
	searcher_t::iterator c, c1, c2, c3, c4, c5;
	std::pair<searcher_t::cursor, std::pair<bool, bool> > ret;

	c = my_tree.end();
	BOOST_CHECK(c == my_tree.end());
	//BOOST_CHECK(c == my_tree.begin()); // current FIXME
	
	searcher_t::cursor cur = searcher_t::cursor(c);
	BOOST_CHECK(!cur.has_child());
	BOOST_CHECK(cur == searcher_t::cursor(my_tree.end()));
	
	
	c1 = my_tree.insert(c, 8);
	
	BOOST_CHECK(*c1 == 8);
	BOOST_CHECK(searcher_t::cursor(c1).parity() == 0);
	BOOST_CHECK(c != my_tree.end());
	BOOST_CHECK(c1 != my_tree.end());
	
	BOOST_CHECK(++c1 == my_tree.end());
	
	--c1;
	BOOST_CHECK(searcher_t::cursor(my_tree.end()).parity() == 1);

	BOOST_CHECK(cur.end().parity() == 1);
	BOOST_CHECK(cur.end() == searcher_t::cursor(my_tree.end()));
	
	BOOST_CHECK(cur.begin() == searcher_t::cursor(c1));
	BOOST_CHECK(cur.end() == ++searcher_t::cursor(c1));
	
	cur = searcher_t::cursor(c1);
	
	BOOST_CHECK(!(++cur).has_child());
	BOOST_CHECK((--cur).parent().parity());

	BOOST_CHECK(*(searcher_t::cursor(c).begin()) == 8);
	
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
	
	searcher_t::container_type the_tree = my_tree.get_container();
	searcher_t::cursor it = boost::tree::lower_bound(the_tree.root(), 
		the_tree.shoot(), 39, std::less<int>());

	BOOST_CHECK(*it == 39);
	it = boost::tree::lower_bound(the_tree.root(), the_tree.shoot(), 18);	
	BOOST_CHECK(*it == 18);
	
	it = boost::tree::lower_bound(the_tree.root(), the_tree.shoot(), 30);	
	BOOST_CHECK(*it == 31);
	
	it = boost::tree::lower_bound(the_tree.root(), the_tree.shoot(), 3);	
	BOOST_CHECK(*it == 7);
	
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
