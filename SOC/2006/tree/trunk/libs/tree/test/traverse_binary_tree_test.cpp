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

using namespace boost::tree;


//std::vector<int> preorder_data()
//{
//	std::vector ret(9);
//	ret[0] = 8;
//	ret[1] = 3;
//	ret[2] = 1;
//	ret[3] = 6;
//	ret[4] = 4;
//	ret[5] = 7;
//	ret[6] = 10;
//	ret[7] = 14;
//	ret[8] = 13;
//}

template <class Iterator>
void test_inorder_traversal(Iterator a, Iterator b)
{		
	BOOST_CHECK(*a++ == 1);
	BOOST_CHECK(*a++ == 3);
	BOOST_CHECK(*a++ == 4);
	BOOST_CHECK(*a++ == 6);
	BOOST_CHECK(*a++ == 7);
	BOOST_CHECK(*a++ == 8);
	BOOST_CHECK(*a++ == 10);
	BOOST_CHECK(*a++ == 11);
	BOOST_CHECK(*a++ == 12);
	BOOST_CHECK(*a++ == 13);
	BOOST_CHECK(*a++ == 14);
	BOOST_CHECK(a == b);
}

template <class Iterator>
void test_reverse_inorder_traversal(Iterator a, Iterator b)
{	
	BOOST_CHECK(*--a == 14);
	BOOST_CHECK(*--a == 13);
	BOOST_CHECK(*--a == 12);
	BOOST_CHECK(*--a == 11);
	BOOST_CHECK(*--a == 10);
	BOOST_CHECK(*--a == 8);
	BOOST_CHECK(*--a == 7);
	BOOST_CHECK(*--a == 6);
	BOOST_CHECK(*--a == 4);
	BOOST_CHECK(*--a == 3);
	BOOST_CHECK(*--a == 1);
	BOOST_CHECK(a == b);
}

template <class Iterator>
void test_preorder_traversal(Iterator a, Iterator b)
{	
	BOOST_CHECK(*a++ == 8);
	BOOST_CHECK(*a++ == 3);
	BOOST_CHECK(*a++ == 1);
	BOOST_CHECK(*a++ == 6);
	BOOST_CHECK(*a++ == 4);
	BOOST_CHECK(*a++ == 7);
	BOOST_CHECK(*a++ == 10);
	BOOST_CHECK(*a++ == 14);
	BOOST_CHECK(*a++ == 13);
	BOOST_CHECK(*a++ == 11);
	BOOST_CHECK(*a++ == 12);
	BOOST_CHECK(a == b);
}

template <class Iterator>
void test_reverse_preorder_traversal(Iterator a, Iterator b)
{	 
	BOOST_CHECK(*--a == 12);
	BOOST_CHECK(*--a == 11);
	BOOST_CHECK(*--a == 13);
	BOOST_CHECK(*--a == 14);
	BOOST_CHECK(*--a == 10);
	BOOST_CHECK(*--a == 7);
	BOOST_CHECK(*--a == 4);
	BOOST_CHECK(*--a == 6);
	BOOST_CHECK(*--a == 1);
	BOOST_CHECK(*--a == 3);
	BOOST_CHECK(*--a == 8);
	BOOST_CHECK(a == b);
}

template <class Iterator>
void test_postorder_traversal(Iterator a, Iterator b)
{	
	BOOST_CHECK(*a++ == 1);	
	BOOST_CHECK(*a++ == 4);
	BOOST_CHECK(*a++ == 7);
	BOOST_CHECK(*a++ == 6);
	BOOST_CHECK(*a++ == 3);
	BOOST_CHECK(*a++ == 12);
	BOOST_CHECK(*a++ == 11);
	BOOST_CHECK(*a++ == 13);
	BOOST_CHECK(*a++ == 14);
	BOOST_CHECK(*a++ == 10);
	BOOST_CHECK(*a++ == 8);
	BOOST_CHECK(a == b);
}

template <class Iterator>
void test_reverse_postorder_traversal(Iterator a, Iterator b)
{	
	BOOST_CHECK(*--a == 8);	
	BOOST_CHECK(*--a == 10);
	BOOST_CHECK(*--a == 14);
	BOOST_CHECK(*--a == 13);
	BOOST_CHECK(*--a == 11);
	BOOST_CHECK(*--a == 12);
	BOOST_CHECK(*--a == 3);
	BOOST_CHECK(*--a == 6);
	BOOST_CHECK(*--a == 7);
	BOOST_CHECK(*--a == 4);
	BOOST_CHECK(*--a == 1);
	BOOST_CHECK(a == b);
}

int test_main(int, char* [])
{
	using boost::forward_traversal_tag;
	
	binary_tree<int> test_tree;
	create_test_data_tree(test_tree);
	
	test_inorder_traversal(inorder::begin(test_tree), 
						   inorder::end(test_tree));
	test_reverse_inorder_traversal(inorder::end(test_tree), 
								   inorder::begin(test_tree));
		
	test_preorder_traversal(preorder::begin(test_tree),
							preorder::end(test_tree));
//FIXME
//	test_reverse_preorder_traversal(preorder::end(test_tree),
//									preorder::begin(test_tree));
	
	test_postorder_traversal(postorder::begin(test_tree), 
							 postorder::end(test_tree));
	test_reverse_postorder_traversal(postorder::end(test_tree), 
									 postorder::begin(test_tree));
	
	test_inorder_traversal(inorder::begin(test_tree, forward_traversal_tag()), 
						   inorder::end(test_tree, forward_traversal_tag()));
	test_reverse_inorder_traversal(inorder::end(test_tree, forward_traversal_tag()), 
								   inorder::begin(test_tree, forward_traversal_tag()));

	binary_tree<int>::cursor c = test_tree.root();
	ascending::iterator<binary_tree<int>::cursor> ai_root(c);
	c = c.begin().end().begin().begin();
	BOOST_CHECK(*c == 4);

	ascending::iterator<binary_tree<int>::cursor> ais(c);
	BOOST_CHECK(*ais == 4);
	BOOST_CHECK(*++ais == 6);
	BOOST_CHECK(*++ais == 3);
	BOOST_CHECK(*++ais == 8);
	BOOST_CHECK(++ais == ai_root);

	binary_tree<int> test_tree2;
	create_test_data_tree(test_tree2);
//	BOOST_CHECK(test_tree == test_tree2);
	
	binary_tree<int>::cursor d = test_tree2.root();
	d = d.begin().end().begin().begin();
	*d = 29;
	
	c = test_tree.root();
	d = test_tree2.root();

	// output_cursor_iterator_wrapper tests
	
	// preorder::copy tests
	std::list<int> pre_lst;
	typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
	typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
	back_insert_iter_list_int it_pre_lst = std::back_inserter(pre_lst);
	oc_bi_lst_type oc_pre_lst = oc_bi_lst_type(it_pre_lst);
	preorder::copy(c.begin(), oc_pre_lst);
	test_preorder_traversal(pre_lst.begin(), pre_lst.end());

	// inorder::copy tests
	std::list<int> in_lst;
	back_insert_iter_list_int it_in_lst = std::back_inserter(in_lst);
	oc_bi_lst_type oc_in_lst = oc_bi_lst_type(it_in_lst);
	inorder::copy(c.begin(), oc_in_lst);
	test_inorder_traversal(in_lst.begin(), in_lst.end());
	
	// postorder::copy tests	
	// Using a list iterator
	std::list<int> lst(11);	
	typedef output_cursor_iterator_wrapper<std::list<int>::iterator> oc_lst_type;
	std::list<int>::iterator li = lst.begin();
	oc_lst_type oc_lst(li);
	postorder::copy(c.begin(), oc_lst);
	test_postorder_traversal(lst.begin(), lst.end());

	// Using a list<int> back_inserter
	std::list<int> lst2;
	back_insert_iter_list_int bi_lst = std::back_inserter(lst2);
	oc_bi_lst_type oc_bi_lst = oc_bi_lst_type(bi_lst);
	postorder::copy(c.begin(), oc_bi_lst);
	test_postorder_traversal(lst2.begin(), lst2.end());
		
	return 0;
}


