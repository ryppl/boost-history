//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//TODO: Make this a test suite.
//		Add iterator traversal tests - check if proper overloads (if present)
//		are used.

// TODO: get timings. that makes that no testcase anymore, right?
//does boost have timers? what does the austern et al one look like?

#include <boost/tree/binary_tree.hpp>

#include <boost/tree/iterator.hpp>
#include <boost/tree/traversal.hpp>

#include <boost/test/minimal.hpp>

#include <list>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

int test_main(int, char* [])
{
	using boost::forward_traversal_tag;
	
	binary_tree<int> test_tree;
	create_test_data_tree(test_tree);
	
	test::inorder::traversal(inorder::begin(test_tree), 
						   inorder::end(test_tree));
	test::inorder::reverse_traversal(inorder::end(test_tree), 
								   inorder::begin(test_tree));
		
	test::preorder::traversal(preorder::begin(test_tree),
							preorder::end(test_tree));
//FIXME
//	test::preorder::reverse_traversal(preorder::end(test_tree),
//									preorder::begin(test_tree));
	
	test::postorder::traversal(postorder::begin(test_tree), 
							 postorder::end(test_tree));
	test::postorder::reverse_traversal(postorder::end(test_tree), 
									 postorder::begin(test_tree));
	
	test::inorder::traversal(inorder::begin(test_tree, forward_traversal_tag()), 
						   inorder::end(test_tree, forward_traversal_tag()));
	test::inorder::reverse_traversal(inorder::end(test_tree, forward_traversal_tag()), 
								   inorder::begin(test_tree, forward_traversal_tag()));

	binary_tree<int>::cursor c = test_tree.root();
	ascending::iterator<binary_tree<int>::cursor> ai_root(c);
	c = c.begin().end().begin().begin();
	BOOST_CHECK(*c == 4);

	ascending::iterator<binary_tree<int>::cursor> ais(c);
	test::ascending::traversal_from_leaf4(ais, ai_root);

	return 0;
}


