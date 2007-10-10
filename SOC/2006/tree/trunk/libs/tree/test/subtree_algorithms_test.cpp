//  Copyright (c) 2006, Bernhard Reiter
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
	
	binary_tree<int> test_tree, test_tree2;
	create_test_data_tree(test_tree);
	create_test_data_tree(test_tree2);
//	BOOST_CHECK(test_tree == test_tree2);
	
	binary_tree<int>::cursor c = test_tree.root();
	binary_tree<int>::cursor d = test_tree2.root();

	d = d.begin().end().begin().begin();
	*d = 29;
	
	d = test_tree2.root();

	// output_cursor_iterator_wrapper tests
	
	// preorder::copy tests
	std::list<int> test_list;
	typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
	typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
	back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
	oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
	preorder::copy(c.begin(), oc_test_list);
	test_preorder_traversal(test_list.begin(), test_list.end());

	// inorder::copy tests
	test_list.clear();
	it_test_list = std::back_inserter(test_list);
	oc_test_list = oc_bi_lst_type(it_test_list);
	inorder::copy(c.begin(), oc_test_list);
	test_inorder_traversal(test_list.begin(), test_list.end());
	
	// postorder::copy tests
	test_list.clear();
	it_test_list = std::back_inserter(test_list);
	oc_test_list = oc_bi_lst_type(it_test_list);
	postorder::copy(c.begin(), oc_test_list);
	test_postorder_traversal(test_list.begin(), test_list.end());
		
	// Using a list iterator
	test_list.clear();
	test_list.resize(11);
	typedef output_cursor_iterator_wrapper<std::list<int>::iterator> oc_lst_type;
	std::list<int>::iterator li = test_list.begin();
	oc_lst_type oc_lst(li);
	postorder::copy(c.begin(), oc_lst);
	test_postorder_traversal(test_list.begin(), test_list.end());
		
	return 0;
}


