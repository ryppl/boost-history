//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balancers/treap.hpp>
#include <boost/tree/augmentors/unaugmented.hpp>

#include <boost/tree/searcher.hpp>

#include "helpers.hpp"

#include <boost/test/minimal.hpp>

using boost::tree::binary_tree;

//TODO: Make this a test suite.

void test_treap()
{
	using namespace boost::tree;
	
	typedef binary_tree<int, balancers::treap> tree_t;
	typedef test_searcher<false, tree_t> searcher_t;
	searcher_t my_searcher;
	
	test_data_searcher(my_searcher);
	
	test_inorder_iterator_traversal(my_searcher);
	
	tree_t& my_tree = my_searcher.container();
	
	// TODO: a level-order algo? 
	
	for (tree_t::const_iterator ci = my_tree.cbegin(); ci != my_tree.cend(); ++ci)
		BOOST_CHECK(tree_t::const_cursor(ci).metadata().get_priority() 
				  < tree_t::const_cursor(ci).parent().metadata().get_priority());
	
	searcher_t::iterator c = my_searcher.begin();
	BOOST_CHECK(*c++ == 1);
	BOOST_CHECK(*c++ == 3);
	BOOST_CHECK(*c++ == 4);
	BOOST_CHECK(*c == 6);

	my_searcher.erase(c);
	
	for (tree_t::const_iterator ci = my_tree.cbegin(); ci != my_tree.cend(); ++ci)
		BOOST_CHECK(tree_t::const_cursor(ci).metadata().get_priority() 
				  < tree_t::const_cursor(ci).parent().metadata().get_priority());
	
	c = my_searcher.begin();
	BOOST_CHECK(*c++ == 1);
	BOOST_CHECK(*c++ == 3);
	BOOST_CHECK(*c++ == 4);
//	BOOST_CHECK(*c++ == 7); //FIXME
//	BOOST_CHECK(*c++ == 8);
//	BOOST_CHECK(*c++ == 10);
//	BOOST_CHECK(*c++ == 11);
//	BOOST_CHECK(*c++ == 12);
//	BOOST_CHECK(*c++ == 13);
//	BOOST_CHECK(*c++ == 14);
//	BOOST_CHECK(c == my_searcher.end());
}


int test_main(int, char* [])
{
	test_treap();
	return 0;
}
