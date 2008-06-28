//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/tree/binary_tree.hpp>

#include <boost/tree/iterator.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/test/minimal.hpp>

#include <list>
#include <algorithm>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

int test_main(int, char* [])
{
	binary_tree<int> test_tree;
	create_test_data_tree(test_tree);
		
	binary_tree<int>::cursor c = test_tree.root();

	c = inorder::lower_bound(test_tree.root(), 4); // Leaf
	BOOST_CHECK(*c == 4);

	c = inorder::lower_bound(test_tree.root(), 7); // Leaf
	BOOST_CHECK(*c == 7);
	
	c = inorder::lower_bound(test_tree.root(), 6); // Non-leaf
	BOOST_CHECK(*c == 6);

	c = inorder::lower_bound(test_tree.root(), 8); // root().begin()
	BOOST_CHECK(*c == 8);

	c = inorder::lower_bound(test_tree.root(), 5); // Not in tree
	BOOST_CHECK(*c == 6);
	
	*c = 4;
	
	c = inorder::lower_bound(test_tree.root(), 5); // Not in tree
	BOOST_CHECK(*c == 7);

	c = inorder::lower_bound(test_tree.root(), 4); // Twice in tree
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(*c.to_parent() == 4);
	
	//binary_tree<int>::cursor d = inorder::upper_bound(test_tree.root(), 4); // Twice in tree
	//BOOST_CHECK(*d == 4);
	
	*c = 6;
	
	validate_test_data_tree(test_tree);
	
	return 0;
}
