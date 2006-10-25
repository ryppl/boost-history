//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/forest.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_forest()
{
	using namespace boost::tree;
	
	typedef forest<int> tree_type;
	tree_type mytree;
	
	tree_type::cursor c = mytree.root();
	tree_type::base_cursor cur = tree_type::base_cursor(c);
	BOOST_CHECK(!cur.parity());
	cur = cur.parent();
	BOOST_CHECK(cur.parity());
	
	c = mytree.insert(c, 6);	
	BOOST_CHECK(*c == 6);
	cur = tree_type::base_cursor(c);
	BOOST_CHECK(*cur == 6);
	
//	BOOST_CHECK(cur == mytree.h.root().begin());
	
	c = mytree.insert(c, 5);	
	BOOST_CHECK(*c == 5);
	cur = tree_type::base_cursor(c);
//	BOOST_CHECK(cur == mytree.h.root().begin());

	c = mytree.insert(c, 4);	
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c == mytree.root().begin());
	
	cur = tree_type::base_cursor(c);
//	BOOST_CHECK(cur == mytree.h.root().begin());

//	++cur;
//	cur = cur.begin();	
//	BOOST_CHECK(*cur == 5);
//	cur = cur.parent();
//	--cur;

	++c;
	BOOST_CHECK(*c == 5);
	++c;
	BOOST_CHECK(*c == 6);
//	--c;
//	BOOST_CHECK(*c == 5);
//	--c;
//	BOOST_CHECK(*c == 4);
	
	
//	cur = tree_type::base_cursor(c);
//	BOOST_CHECK(*cur == 4);
//	//cur = cur.parent().parent().parent().begin();
//	BOOST_CHECK(*cur == 4);
	
	//BOOST_CHECK(*++c == 5);

}



int test_main(int, char* [])
{
	test_forest();
	return 0;
}
