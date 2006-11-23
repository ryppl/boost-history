//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balancers/treap.hpp>
#include <boost/tree/augmentors/unaugmented.hpp>

//#include <boost/tree/searcher.hpp>

#include <vector>
#include <algorithm>

#include <boost/test/minimal.hpp>

#include "helpers.hpp"

//TODO: Make this a test suite.

void test_treap()
{
	using namespace boost::tree;
	
	std::vector<int> my_vector;
	typedef binary_tree<int> tree_t;
	//typedef test_searcher<false, tree_t> searcher_t;
	typedef test_balancer<binary_tree<int>, balancers::unbalanced> treap_t;
	
	//searcher_t my_searcher;
	treap_t my_balancer;
	tree_t my_tree;
	
	//create_test_data_searcher(my_searcher);
	create_test_data_sequence(my_balancer);
	create_test_data_sequence(my_vector);
	
	BOOST_CHECK(std::equal(my_balancer.begin(), my_balancer.end(), my_vector.begin()));
	
	// TODO: const_iterator !
	treap_t::iterator bcit = my_balancer.begin();
	BOOST_CHECK(*bcit == 8);
	treap_t::hierarchy_type::cursor c = treap_t::hierarchy_type::cursor(inorder::iterator<treap_t::hierarchy_type::cursor>(bcit)); //bcit.base().base();
	BOOST_CHECK(bcit.base().base() == my_balancer.hierarchy().root().begin());
	BOOST_CHECK(c.parent() == my_balancer.hierarchy().root());
	
	//BOOST_CHECK(c.parity() != 0);
	//BOOST_CHECK(c->data != 8);
		
	c = my_balancer.hierarchy().insert(c, treap_t::data_type(78));
	BOOST_CHECK(c->data == 78);
	
//	treap_t::hierarchy_type h = my_balancer.hierarchy();
//	h = my_balancer.hierarchy();
//	c = 
	
	//my_balancer.hierarchy().rotate(c);
	
	//my_balancer.rotate(bcit);
	
//	BOOST_CHECK(*++bcit == 10);
//	BOOST_CHECK(*++bcit == 14);
//	
//	c = bcit.base().base();
//	BOOST_CHECK(c.parent().parent().parent() == my_balancer.hierarchy().root());
	//my_balancer.hierarchy().rotate(c);
	
//	test_inorder_iterator_traversal(my_searcher);
	
//	tree_t& my_tree = my_searcher.container();
	
	// TODO: a level-order algo? 
	
//	for (tree_t::const_iterator ci = my_tree.cbegin(); ci != my_tree.cend(); ++ci)
//		BOOST_CHECK(tree_t::const_cursor(ci).metadata().get_priority() 
//				  < tree_t::const_cursor(ci).parent().metadata().get_priority());
	
//	searcher_t::iterator c = my_searcher.begin();
//	BOOST_CHECK(*c++ == 1);
//	BOOST_CHECK(*c++ == 3);
//	BOOST_CHECK(*c++ == 4);
//	BOOST_CHECK(*c == 6);
//
//	my_searcher.erase(c);
	
//	for (tree_t::const_iterator ci = my_tree.cbegin(); ci != my_tree.cend(); ++ci)
//		BOOST_CHECK(tree_t::const_cursor(ci).metadata().get_priority() 
//				  < tree_t::const_cursor(ci).parent().metadata().get_priority());
	
//	c = my_searcher.begin();
//	BOOST_CHECK(*c++ == 1);
//	BOOST_CHECK(*c++ == 3);
//	BOOST_CHECK(*c++ == 4);
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
