//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/nary_tree.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_nary_tree()
{
	using namespace boost::tree;
	
	typedef nary_tree<int> tree_type;
	tree_type mytree;
	tree_type::cursor c = mytree.shoot();
	BOOST_CHECK(mytree.root().empty());
	BOOST_CHECK(c.empty());

	c = mytree.insert(c, 4);
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c == mytree.root());
	BOOST_CHECK(c.empty());
	
	c = mytree.insert(c, 5);
	BOOST_CHECK(*c == 5);
	BOOST_CHECK(c == mytree.root());
	++c;
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c != mytree.root());
	BOOST_CHECK(c.empty());
//	BOOST_CHECK(c.m_cur != tree_type::node_type::nil());

	mytree.insert(c.end(), 3);
	BOOST_CHECK(*(c.begin()) == 3);
	BOOST_CHECK(!c.empty());
	BOOST_CHECK(c == c.begin().parent());

}

int test_main(int, char* [])
{
	test_nary_tree();
	return 0;
}
