//  Copyright (c) 2006-2009, Bernhard Reiter
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
#include <boost/test/parameterized_test.hpp>

#include <list>
#include <algorithm>
#include <iterator>

#include "helpers.hpp"
#include "fake_binary_tree.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(binary_tree_search_test, test_binary_tree_fixture<int>)

//template <class Cursor>
//void search_single_element(Cursor r, int v)
//{
//    Cursor c, d;
//    c = lower_bound(r, v);
//    d = upper_bound(r, v); 
//    
//    BOOST_CHECK_EQUAL(*c, v);
//    //BOOST_CHECK_EQUAL(next(inorder(), c) , d);
//}

// Use mock cursor for tests, especially position!

BOOST_AUTO_TEST_CASE( lower_bound_test )
{   
    binary_tree<int>::cursor c, d;

    c = lower_bound(bt.root(), 4); // (Left) Leaf
    c = lower_bound(bt.root(), 7); // (Right) Leaf
    c = lower_bound(bt.root(), 6); // Non-Leaf
    c = lower_bound(bt.root(), 8); // root().begin()

    c = lower_bound(bt.root(), 5); // Not in tree
    d = lower_bound(bt.root(), 5);
    BOOST_CHECK_EQUAL(*c, 6);
    BOOST_CHECK_EQUAL(*d, 6);
    
    //*c = 4;
    
    c = lower_bound(bt.root(), 5); // Not in tree
    BOOST_CHECK_EQUAL(*c, 6);

    c = lower_bound(bt.root(), 4); // Twice in tree
    //d = upper_bound(bt.root(), 4);
    BOOST_CHECK_EQUAL(*c, 4);
    BOOST_CHECK_EQUAL(*d, 6);
    //BOOST_CHECK_EQUAL(*c.parent(), 4);
    //BOOST_CHECK(next(inorder(), c, 2) == d);
    
    //*c.to_parent() = 6;
    //validate_test_dataset1_tree(bt.root());
}

BOOST_AUTO_TEST_SUITE_END()