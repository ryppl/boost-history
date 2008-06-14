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

#include <boost/tree/ascending_cursor.hpp>

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

template <class Cursor, class Op>
void underefed_for_each_recursive(Cursor s, Op& f)
{
	Cursor t = s.end();
	f(s);			// Caution: f(s) comes before s.to_begin(), as opposed to
	s.to_begin();	// "normal" preorder for_each
	do
		if (!s.empty())
			underefed_for_each_recursive(s, f);
	while (s++ != t);
}

template <class Cursor, class Op>
Op underefed_for_each(Cursor s, Op f)
{
	Cursor t = s.end();
	f(s);			// Caution: f(s) comes before s.to_begin(), as opposed to
	s.to_begin();	// "normal" preorder for_each
	do
		if (!s.empty())
			underefed_for_each_recursive(s, f);
	while (s++ != t);
		
	return f;
}

template <class Cursor>
void comparisons(Cursor c) {
	test::preorder::compare_cursor_to_iterator_traversal(c);
	test::inorder::compare_cursor_to_iterator_traversal(c);
	test::postorder::compare_cursor_to_iterator_traversal(c);
	return;
}

void comparisons_using_ac(binary_tree<int>::cursor c) {
	comparisons(make_ascending_cursor(c));
	return;
}

/** 
 * Check all iterator traversals by comparing them to a recursive cursor
 * algorithm output. Do that at different stages of the tree while adding
 * elements to it, so different tree shapes are checked to be catered for
 * by the iterator algorithms.
 * Do all that also using iterators wrapped around "explicit stack"-based cursors
 */ 
void compare_cursor_to_iterator_traversal() {
	using boost::tree::make_ascending_cursor;
	
	binary_tree<int> test_tree2;
	//comparisons(test_tree2.root());

	binary_tree<int>::cursor c = test_tree2.insert(test_tree2.root(), 8);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));

	c = test_tree2.insert(c, 3);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	test_tree2.insert(c, 1);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
		
	c = test_tree2.insert(++c, 6);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	test_tree2.insert(c, 4);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
		
	test_tree2.insert(++c, 7);	
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	c = test_tree2.insert(test_tree2.root().end(), 10);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	c = test_tree2.insert(test_tree2.root().end().end(), 14);	
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	c = test_tree2.insert(c, 13);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	c = test_tree2.insert(c, 11);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));
	
	c = test_tree2.insert(++c, 12);
	comparisons(test_tree2.root());
	comparisons(make_ascending_cursor(test_tree2.root()));

	typedef ascending_cursor<binary_tree<int>::cursor> ac;
	
	// FIXME: This requires subtree cursors to know their root.
	//underefed_for_each(test_tree2.root(), comparisons<binary_tree<int>::cursor>);
	
	underefed_for_each(test_tree2.root(), comparisons_using_ac);
}

int test_main(int, char* [])
{
	typedef boost::tree::binary_tree<int>::cursor cursor;
	
	binary_tree<int> test_tree;	
//	std::list<int> test_list;
//	
//	// TODO: Put this into a better testing context.
//	boost::tree::preorder::for_each(
//		test_tree.root(), 
//		boost::lambda::bind(&std::list<int>::push_back, &test_list, boost::lambda::_1)
//	);
//	BOOST_CHECK(test_list.empty());
	
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

	// Now the iterators based on stack-based cursors (that don't use cursor.to_parent())

	test::preorder::traversal(preorder::begin(ascending_cursor<cursor>(test_tree.root())), 
						   	  preorder::end(ascending_cursor<cursor>(test_tree.root())));
	test::preorder::reverse_traversal(preorder::end(ascending_cursor<cursor>(test_tree.root())), 
								   	  preorder::begin(ascending_cursor<cursor>(test_tree.root())));	
	
	test::inorder::traversal(inorder::begin(ascending_cursor<cursor>(test_tree.root())), 
							 inorder::end(ascending_cursor<cursor>(test_tree.root())));
	test::inorder::reverse_traversal(inorder::end(ascending_cursor<cursor>(test_tree.root())), 
									 inorder::begin(ascending_cursor<cursor>(test_tree.root())));

	test::postorder::traversal(postorder::begin(ascending_cursor<cursor>(test_tree.root())), 
						   	   postorder::end(ascending_cursor<cursor>(test_tree.root())));
	test::postorder::reverse_traversal(postorder::end(ascending_cursor<cursor>(test_tree.root())), 
								   	   postorder::begin(ascending_cursor<cursor>(test_tree.root())));

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


