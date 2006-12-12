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
#include <boost/tree/balanced_tree.hpp>
#include <boost/tree/searcher.hpp>

#include <boost/tree/balancers/unbalanced.hpp>

#include <boost/tree/traversal.hpp>
#include <boost/tree/iterators.hpp>

#include <vector>
#include <algorithm>

#include <boost/test/minimal.hpp>

#include "helpers.hpp"

using namespace boost::tree;
typedef test_searcher<false, test_balancer<binary_tree<int>, balancers::unbalanced> > searcher_t;

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

// orphaned == no parent() links...
// TODO: const parameter
void test_orphaned_inorder_traversal(binary_tree<int>& the_tree)
{	
	boost::tree::inorder::iterator<binary_tree<int>::cursor, boost::forward_traversal_tag>
		ci = boost::tree::inorder::begin(the_tree, boost::forward_traversal_tag());
	BOOST_CHECK(*ci == 1);
	++ci;
	BOOST_CHECK(*ci == 3);
	++ci;
	BOOST_CHECK(*ci == 4);	
	++ci;
	BOOST_CHECK(*ci == 6);
	++ci;
	BOOST_CHECK(*ci == 7);
	++ci;
	BOOST_CHECK(*ci == 8);
	++ci;
	BOOST_CHECK(*ci == 10);
	++ci;
	BOOST_CHECK(*ci == 11);
	++ci;
	BOOST_CHECK(*ci == 12);
	++ci;
	BOOST_CHECK(*ci == 13);
	++ci;	
	BOOST_CHECK(*ci == 14);
	++ci;
	BOOST_CHECK(ci == boost::tree::inorder::end(the_tree, boost::forward_traversal_tag()));
}

void test_orphaned_reverse_inorder_traversal(binary_tree<int>& the_tree)
{	
	boost::tree::inorder::iterator<binary_tree<int>::cursor, boost::forward_traversal_tag>
		ci = boost::tree::inorder::end(the_tree, boost::forward_traversal_tag());
	--ci;
	BOOST_CHECK(*ci == 14);
	--ci;
	BOOST_CHECK(*ci == 13);
	--ci;
	BOOST_CHECK(*ci == 12);	
	--ci;
	BOOST_CHECK(*ci == 11);
	--ci;
	BOOST_CHECK(*ci == 10);
	--ci;
	BOOST_CHECK(*ci == 8);
	--ci;
	BOOST_CHECK(*ci == 7);
	--ci;
	BOOST_CHECK(*ci == 6);
	--ci;
	BOOST_CHECK(*ci == 4);
	--ci;
	BOOST_CHECK(*ci == 3);
	--ci;	
	BOOST_CHECK(*ci == 1);
	BOOST_CHECK(ci == boost::tree::inorder::begin(the_tree, boost::forward_traversal_tag()));
}

int test_main(int, char* [])
{
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
	
	test_orphaned_inorder_traversal(test_tree);
	test_orphaned_reverse_inorder_traversal(test_tree);
	
	return 0;
}


