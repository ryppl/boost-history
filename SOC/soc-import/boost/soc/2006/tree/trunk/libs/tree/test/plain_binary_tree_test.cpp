// Copyright 2006 Bernhard Reiter.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/augmentors/trivial.hpp>
#include <boost/tree/balancers/trivial.hpp>

#include <boost/tree/searcher.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_plain_binary_tree()
{
	using boost::tree::binary_tree;
	using boost::tree::binary_node;
	using boost::tree::tree_cursor;
	using boost::tree::lower_bound;
	
	typedef binary_tree<int> tree_type;
	tree_type mytree;

	tree_type::cursor c, c1, c2, c3, c4;
	tree_type::iterator it;
	
	c = mytree.root();

	BOOST_CHECK(!c.has_child());
	
	c1 = mytree.insert(c, 1);
	BOOST_CHECK(*c1 == 1);
	BOOST_CHECK(c.has_child());
	c2 = mytree.insert(c1, 2);
	BOOST_CHECK(c.has_child());
	BOOST_CHECK(!c2.has_child());
	BOOST_CHECK(*c1 == 1);
	BOOST_CHECK(*c2 == 2);
	*c1 = 14;
	BOOST_CHECK(*c1 == 14);
	BOOST_CHECK(*c2 == 2);
	BOOST_CHECK(c2.parent() == c1);
	BOOST_CHECK(c1.parent() == c);
	
	c3 = c1.end();
	BOOST_CHECK(c3 == c1.end());
	--c3;
	BOOST_CHECK(c3.parity() == 0);
	BOOST_CHECK(c3.parent() != c3);
	BOOST_CHECK(c3.parent() == c1);
	BOOST_CHECK(c3 == c1.begin());
	BOOST_CHECK(*c3 == 2);
	++c3;
	c4 = mytree.insert(c3, 4);
	BOOST_CHECK(*c4 == 4);
	c4 = c4.parent();
	--c4;
	BOOST_CHECK(*c4 == 2);
	BOOST_CHECK(c4.parent() == c1);
	c = boost::tree::lower_bound(mytree.root(), mytree.shoot(), 2, std::less<int>());
	BOOST_CHECK(*c == 2);
	BOOST_CHECK(!c4.has_child());

	mytree.clear();
		
}



int test_main(int, char* [])
{
	test_plain_binary_tree();
	return 0;
}
