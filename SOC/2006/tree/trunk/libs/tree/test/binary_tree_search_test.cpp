//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


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

void search_single_element(binary_tree<int>::const_cursor r, int v)
{
    binary_tree<int>::const_cursor c, d;
    c = inorder::lower_bound(r, v);
    d = inorder::upper_bound(r, v); 
    
    BOOST_CHECK(*c == v);
    //BOOST_CHECK(inorder::next(c) == d);
}

int test_main(int, char* [])
{
    binary_tree<int> test_tree;
    create_test_data_tree(test_tree);
        
    binary_tree<int>::cursor c, d;

    search_single_element(test_tree.root(), 4); // (Left) Leaf
    search_single_element(test_tree.root(), 7); // (Right) Leaf
    search_single_element(test_tree.root(), 6); // Non-Leaf
    search_single_element(test_tree.root(), 8); // root().begin()

    c = inorder::lower_bound(test_tree.root(), 5); // Not in tree
    d = inorder::lower_bound(test_tree.root(), 5);
    BOOST_CHECK(*c == 6);
    BOOST_CHECK(*d == 6);
    
    *c = 4;
    
    c = inorder::lower_bound(test_tree.root(), 5); // Not in tree
    BOOST_CHECK(*c == 7);

    c = inorder::lower_bound(test_tree.root(), 4); // Twice in tree
    d = inorder::upper_bound(test_tree.root(), 4);
    BOOST_CHECK(*c == 4);
    BOOST_CHECK(*d == 7);
    BOOST_CHECK(*c.parent() == 4);
    //BOOST_CHECK(inorder::next(c, 2) == d);
    
    *c.to_parent() = 6;
    validate_test_data_tree(test_tree);
    
    return 0;
}
