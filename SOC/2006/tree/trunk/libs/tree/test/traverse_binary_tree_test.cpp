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
#include <boost/tree/algorithm.hpp>

#include <boost/test/minimal.hpp>

#include <list>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

// Some macro magic, to save us from all too tedious redundant calls
// to each of the three types of order algorithms and checks.

#define ORDER preorder
#include "subtree_algorithms_checks.hpp"

#undef ORDER
#define ORDER inorder
#include "subtree_algorithms_checks.hpp"

#undef ORDER
#define ORDER postorder
#include "subtree_algorithms_checks.hpp"

#undef ORDER

void comparisons(binary_tree<int> const& test_tree) {
	test::preorder::compare_cursor_to_iterator_traversal(test_tree);
	test::inorder::compare_cursor_to_iterator_traversal(test_tree);	
	test::postorder::compare_cursor_to_iterator_traversal(test_tree);
	return;
}

/** 
 * Check all iterator traversals by comparing them to a recursive cursor
 * algorithm output. Do that at different stages of the tree while adding
 * elements to it, so different tree shapes are checked to be catered for
 * by the iterator algorithms.
 */ 
void compare_cursor_to_iterator_traversal() {
	binary_tree<int> test_tree2;

	binary_tree<int>::cursor c = test_tree2.insert(test_tree2.root(), 8);
	comparisons(test_tree2);

	c = test_tree2.insert(c, 3);
	comparisons(test_tree2);

	test_tree2.insert(c, 1);
	comparisons(test_tree2);
	
	c = test_tree2.insert(++c, 6);
	comparisons(test_tree2);

	test_tree2.insert(c, 4);
	comparisons(test_tree2);
	
	test_tree2.insert(++c, 7);	
	comparisons(test_tree2);

	c = test_tree2.insert(test_tree2.root().end(), 10);
	comparisons(test_tree2);

	c = test_tree2.insert(test_tree2.root().end().end(), 14);	
	comparisons(test_tree2);

	c = test_tree2.insert(c, 13);
	comparisons(test_tree2);

	c = test_tree2.insert(c, 11);
	comparisons(test_tree2);

	c = test_tree2.insert(++c, 12);
	comparisons(test_tree2);
}

int test_main(int, char* [])
{
	using boost::forward_traversal_tag;
	
	binary_tree<int> test_tree;	
	create_test_data_tree(test_tree);

	//Preorder	
	test::preorder::traversal(preorder::begin(test_tree.root()),
							  preorder::end(test_tree.root()));

	test::preorder::reverse_traversal(preorder::end(test_tree.root()),
									  preorder::begin(test_tree.root()));
									
	BOOST_CHECK(std::distance(preorder::begin(test_tree.root()), 
						      preorder::end(test_tree.root())) == 11);

	// Inorder
	test::inorder::traversal(inorder::begin(test_tree.root()), 
						   inorder::end(test_tree.root()));

	test::inorder::reverse_traversal(inorder::end(test_tree.root()), 
								   inorder::begin(test_tree.root()));
	
	// TODO: Also check with binary_tree-specialized inorder begin()!

	BOOST_CHECK(std::distance(inorder::begin(test_tree.root()), 
						      inorder::end(test_tree.root())) == 11);

	//Postorder
	test::postorder::traversal(postorder::begin(test_tree.root()), 
							 postorder::end(test_tree.root()));
	test::postorder::reverse_traversal(postorder::end(test_tree.root()), 
									 postorder::begin(test_tree.root()));

	BOOST_CHECK(std::distance(postorder::begin(test_tree.root()), 
						      postorder::end(test_tree.root())) == 11);

	// Now the stack-based iterators (that don't use cursor.to_parent())

	test::preorder::traversal(preorder::begin(test_tree.root(), forward_traversal_tag()), 
						   	  preorder::end(test_tree.root(), forward_traversal_tag()));
	test::preorder::reverse_traversal(preorder::end(test_tree.root(), forward_traversal_tag()), 
								   	  preorder::begin(test_tree.root(), forward_traversal_tag()));	
	
	test::inorder::traversal(inorder::begin(test_tree.root(), forward_traversal_tag()), 
						   inorder::end(test_tree.root(), forward_traversal_tag()));
	test::inorder::reverse_traversal(inorder::end(test_tree.root(), forward_traversal_tag()), 
								   inorder::begin(test_tree.root(), forward_traversal_tag()));

	test::postorder::traversal(postorder::begin(test_tree.root(), forward_traversal_tag()), 
						   	   postorder::end(test_tree.root(), forward_traversal_tag()));
	test::postorder::reverse_traversal(postorder::end(test_tree.root(), forward_traversal_tag()), 
								   	   postorder::begin(test_tree.root(), forward_traversal_tag()));

	//Ascending iterator.
	binary_tree<int>::cursor c = test_tree.root();
	ascending::iterator<binary_tree<int>::cursor> ai_root(c);
	c = c.begin().end().begin().begin();
	BOOST_CHECK(*c == 4);

	ascending::iterator<binary_tree<int>::cursor> ais(c);
	test::ascending::traversal_from_leaf4(ais, ai_root);

	//Now the advanced stuff
	compare_cursor_to_iterator_traversal();
		
	return 0;
}


