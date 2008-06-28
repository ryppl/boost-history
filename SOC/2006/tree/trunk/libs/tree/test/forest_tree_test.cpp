//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/forest_tree.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/test/minimal.hpp>

#include <list>

#include "test_tree_traversal_data.hpp"

//TODO: Make this a test suite.

void test_forest_tree()
{
	using namespace boost::tree;
	
	typedef forest_tree<int> tree_type;
	tree_type mytree;
	
	tree_type::cursor c = mytree.root();
	c = mytree.insert(c, 6);	
	BOOST_CHECK(*c == 6);

	c = mytree.insert(c, 5);	
	BOOST_CHECK(*c == 5);

	c = mytree.insert(c, 4);	
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c == mytree.root().begin());

	++c;
	BOOST_CHECK(*c == 5);
	++c;
	BOOST_CHECK(*c == 6);
	
	tree_type forest;
	//create_test_data_tree(forest);
	c = forest.insert(forest.root(), 8);
	BOOST_CHECK(c == forest.root().begin());
	BOOST_CHECK(*c == 8);
	c = forest.insert(c, 3);
	BOOST_CHECK(*c == 3);
	BOOST_CHECK(*++c == 8);
//	BOOST_CHECK(*forest.root().begin().begin() == 3);

}

void test_natural_correspondence()
{
	using namespace boost::tree;
	
	typedef binary_tree<int> binary_tree_type;
	binary_tree_type bt;
	create_test_data_tree(bt);
	
	typedef forest_tree<int> forest_tree_type;
	forest_tree_type ft(bt);
	
	validate_corresponding_forest_tree(ft);
	
	// Now test *order correspondence:
	// forest	binary
	// pre		pre
	// post		in
	std::list<int> test_list;
	typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
	typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
	back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
	oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
	
//  Preliminary checks, delete as soon as issues are resolved

//	forest_tree_type::cursor s = ft.root();
//	forest_tree_type::cursor r = s.end();
//	*t.to_begin() = *s.to_begin();
//		BOOST_CHECK(!s.empty());
//		if (!s.empty()) {
//			r = s.end();
//			*t.to_begin() = *s.to_begin();
//		}
//		
//		BOOST_CHECK(!s.empty());
//		if (!s.empty()) {
//			r = s.end();
//			*t.to_begin() = *s.to_begin();
//		}
//	
//	BOOST_CHECK(s.empty());
//	++t;
//	
//	BOOST_CHECK(s != r);
//	BOOST_CHECK(s++ != r);
//	BOOST_CHECK(s == r);
//	
//	BOOST_CHECK(s.empty());
//	++t;

	//BOOST_CHECK(s++ == r);
		
//	do {
//		if (!s.empty())
//			*t = *s;
//			//copy(s, t);
//		++t;
//	} while (s++ != r);
	
	
	boost::tree::preorder::copy(ft.root(), oc_test_list);
	//test::preorder::traversal(test_list.begin(), test_list.end());
	
	BOOST_CHECK(test_list.size() == 6);
	
	std::list<int>::const_iterator lci = test_list.begin();
	BOOST_CHECK(*lci == 8);
	BOOST_CHECK(*++lci == 3);
	BOOST_CHECK(*++lci == 1);
//	BOOST_CHECK(*++lci != 4); // wrong!
//	BOOST_CHECK(*++lci != 13 ); // wrong!
//	BOOST_CHECK(*++lci != 11); // wrong!
	
	test_list.clear();
	//boost::tree::postorder::copy(ft.root(), oc_test_list);
	//test::inorder::traversal(test_list.begin(), test_list.end());
}

int test_main(int, char* [])
{
	test_forest_tree();
	test_natural_correspondence();
	return 0;
}
