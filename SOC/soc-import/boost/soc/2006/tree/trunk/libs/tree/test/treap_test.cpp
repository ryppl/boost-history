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
	typedef test_balancer<binary_tree<int>, balancers::treap> treap_t;
	
	//searcher_t my_searcher;
	treap_t my_balancer;
	//tree_t my_tree;
	
	//create_test_data_searcher(my_searcher);
	create_test_data_sequence(my_balancer);
	create_test_data_sequence(my_vector);
	
	BOOST_CHECK(std::equal(my_balancer.begin(), my_balancer.end(), my_vector.begin()));
		
	for (treap_t::iterator ci = my_balancer.begin(); ci != my_balancer.end(); ++ci)
		BOOST_CHECK(
			treap_t::hierarchy_type::cursor(inorder::iterator<treap_t::hierarchy_type::cursor>(ci))->metadata().get_priority() <
			treap_t::hierarchy_type::cursor(inorder::iterator<treap_t::hierarchy_type::cursor>(ci)).parent()->metadata().get_priority());
//FIXME: The following would be nicer instead.
//		BOOST_CHECK(ci.base().base()->metadata().get_priority() 
//				  < ci.base().base().parent()->metadata().get_priority());

	//treap_t::iterator ci = my_balancer.begin();
	//treap_t::hierarchy_type::cursor c;
	//c = ci.base().base();
	//c = c.parent();
	//treap_t::metadata_type m = c->metadata();
	//int i = ci.base().base().parent()->metadata().get_priority();//m.get_priority();
}


int test_main(int, char* [])
{
	test_treap();
	return 0;
}
