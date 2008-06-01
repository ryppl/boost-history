//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>

#include <boost/tree/searcher.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

#include <boost/test/minimal.hpp>

using boost::tree::binary_tree;

//TODO: Make this a test suite.

void test_rotate()
{
	binary_tree<int> a;
	binary_tree<int> mytree(a);

	create_test_data_tree(mytree);
	binary_tree<int>::cursor c = mytree.root().begin().end();
	BOOST_CHECK(*c.begin() == 6);

	BOOST_CHECK(*c.parent() == 8);
	BOOST_CHECK(*c.parent().begin() == 3); // invariant candidate
	
	BOOST_CHECK(*--c == 3); // differently (not invariantly!) spoken
	BOOST_CHECK(*c.begin() == 1);
	BOOST_CHECK(*((++c).begin()).begin() == 4);
	BOOST_CHECK(*(++c.begin()).begin() == 7);

	BOOST_CHECK(c.parity() == 1);	
	BOOST_CHECK(*c.begin() == 6);
		
	mytree.rotate(c); // Left rotate

	BOOST_CHECK(*c.begin() == 6);
	BOOST_CHECK(*c.parent().begin() == 8);
	
	BOOST_CHECK(*c.end().begin() == 7);
	BOOST_CHECK(*c.begin().begin() == 3);
	BOOST_CHECK(*c.begin().begin().begin() == 1);

	BOOST_CHECK(*c.begin().end().begin() == 4);

	c = c.begin();
	BOOST_CHECK(*c.begin() == 3);
	
	mytree.rotate(c); // Right rotate
	
	BOOST_CHECK(*c.begin() == 3);
	c = c.end();

	BOOST_CHECK(*c.begin() == 6);

	BOOST_CHECK(*c.parent() == 8);
	BOOST_CHECK(*c.parent().begin() == 3); // other invariant candidate
	
	BOOST_CHECK(*--c == 3);
	BOOST_CHECK(*c.begin() == 1);
	BOOST_CHECK(*((++c).begin()).begin() == 4);
	BOOST_CHECK(*(++c.begin()).begin() == 7);
	
	BOOST_CHECK(*c.begin() == 6);
	
//	BOOST_CHECK(*c.parent().parent().begin() == 6);
//	BOOST_CHECK(*c.parent().parent().end().begin() == 7);
	
//	BOOST_CHECK(*c.begin() == 1);
//	BOOST_CHECK(*c.parent().begin() == 3); // invariant?
//	
//	BOOST_CHECK(*c.parent().parent().begin() == 6);
//	BOOST_CHECK(*c.parent().parent().parent().begin() == 8);
//	BOOST_CHECK(*c.parent().parent().end().begin() == 7);
	
}


int test_main(int, char* [])
{
	test_rotate();
	return 0;
}
