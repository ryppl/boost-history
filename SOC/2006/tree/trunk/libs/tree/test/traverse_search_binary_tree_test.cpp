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

#include "helpers.hpp"

using namespace boost::tree;
using std::list;

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

template <class T> class list_push_back {
	std::list<T> l;
public:
	list_push_back() { }
	void operator() (T x) { l.push_back(x); }
	std::list<T> const& result() const { return l; }
	void reset() { l.clear(); }
};

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

	list_push_back<int> lpb;
	//list<int> res;

	// Each of the following blocks should be a 
	// "recursive_*order_algorithm_test" of its own
	lpb = inorder::for_each(test_tree.root().begin(), lpb);
	//res = lpb.result();
	test_inorder_traversal(lpb.result().begin(), lpb.result().end());
	lpb.reset();

	lpb = preorder::for_each(test_tree.root().begin(), lpb);
	//res = lpb.result();
	test_preorder_traversal(lpb.result().begin(), lpb.result().end());

	lpb.reset();
	lpb = postorder::for_each(test_tree.root().begin(), lpb);
	//res = lpb.result();
	test_postorder_traversal(lpb.result().begin(), lpb.result().end());

	return 0;
}


