//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/tree/binary_tree.hpp>

#include <boost/tree/iterator.hpp>
#include <boost/tree/algorithm.hpp>

#define BOOST_TEST_MODULE binary_tree test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include <list>
#include <algorithm>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

void search_single_element(binary_tree<int>::const_cursor r, int v)
{
    binary_tree<int>::const_cursor c, d;
    c = lower_bound(r, v);
    d = upper_bound(r, v); 
    
    BOOST_CHECK(*c == v);
    //BOOST_CHECK(inorder::next(c) == d);
}

BOOST_AUTO_TEST_CASE( binary_tree_search_test )
{
    binary_tree<int> test_tree;
    create_test_data_tree(test_tree);
        
    binary_tree<int>::cursor c, d;

    search_single_element(test_tree.root(), 4); // (Left) Leaf
    search_single_element(test_tree.root(), 7); // (Right) Leaf
    search_single_element(test_tree.root(), 6); // Non-Leaf
    search_single_element(test_tree.root(), 8); // root().begin()

    c = lower_bound(test_tree.root(), 5); // Not in tree
    d = lower_bound(test_tree.root(), 5);
    BOOST_CHECK(*c == 6);
    BOOST_CHECK(*d == 6);
    
    *c = 4;
    
    c = lower_bound(test_tree.root(), 5); // Not in tree
    BOOST_CHECK(*c == 7);

    c = lower_bound(test_tree.root(), 4); // Twice in tree
    d = upper_bound(test_tree.root(), 4);
    BOOST_CHECK(*c == 4);
    BOOST_CHECK(*d == 7);
    BOOST_CHECK(*c.parent() == 4);
    //BOOST_CHECK(inorder::next(c, 2) == d);
    
    *c.to_parent() = 6;
    validate_test_data_tree(test_tree);
}
