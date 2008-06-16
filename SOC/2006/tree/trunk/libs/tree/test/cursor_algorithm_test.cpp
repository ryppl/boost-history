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
#include <algorithm>
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

int test_main(int, char* [])
{
	using boost::forward_traversal_tag;
	
	binary_tree<int> test_tree, test_tree2;
	create_test_data_tree(test_tree);
	create_test_data_tree(test_tree2);
		
	binary_tree<int>::cursor c = test_tree.root();
	binary_tree<int>::cursor d = test_tree2.root();

	// Just to make sure we won't be getting any false positives when 
	// copying test_tree1 to test_tree2, we'll change one of test_tree2's
	// values.
	d = d.begin().end().begin().begin();
	++*d;
	BOOST_CHECK(test_tree != test_tree2);
	d = test_tree2.root();

	test::preorder::algorithms (test_tree.root(), test_tree2.root());
	test::inorder::algorithms  (test_tree.root(), test_tree2.root());
	test::postorder::algorithms(test_tree.root(), test_tree2.root());
			
	return 0;
}
