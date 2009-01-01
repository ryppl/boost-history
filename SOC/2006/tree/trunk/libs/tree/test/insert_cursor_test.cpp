//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
//#include <boost/tree/iterator.hpp>
//#include <boost/tree/algorithm.hpp>

#include <boost/tree/insert_cursor.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(insert_cursor_test, test_binary_tree_fixture<int>)

// Maybe suffix this by "_preorder", and add
// emulated inorder and postorder versions, so
// we can check whether insert_cursor works in all cases
// without relying on algorithms. 
template <class Cursor>
void fill_subtree_with_data(Cursor cur)
{
    *cur.begin() = 8;
    *cur.begin().begin() = 3;
    *cur.begin().begin().begin() = 1;  //Leaf
    *cur.begin().end().begin() = 6;
    *cur.begin().end().begin().begin() = 4; //Leaf
    *cur.begin().end().end().begin() = 7; //Leaf
    *cur.end().begin() = 10;
    *cur.end().end().begin() = 14;
    *cur.end().end().begin().begin() = 13;
    *cur.end().end().begin().begin().begin() = 11; 
    *cur.end().end().begin().begin().end().begin() = 12; //Leaf
}

template <class Cursor>
void fill_subtree_with_data_in_preorder(Cursor cur)
{
    *cur.to_begin() = 8;
    *cur.to_begin() = 3;
    *cur.to_begin() = 1;  //Leaf
    *cur.begin().end().begin() = 6;
    *cur.begin().end().begin().begin() = 4; //Leaf
    *cur.begin().end().end().begin() = 7; //Leaf
    *cur.end().begin() = 10;
    *cur.end().end().begin() = 14;
    *cur.end().end().begin().begin() = 13;
    *cur.end().end().begin().begin().begin() = 11; 
    *cur.end().end().begin().begin().end().begin() = 12; //Leaf
}

BOOST_AUTO_TEST_CASE_TEMPLATE ( test_desc_copy_using_insert_cursor, Order, orders )
{
    bt2.clear();

    boost::tree::copy(Order(), bt.root(), tree_inserter(bt2, bt2.root())
                    , boost::tree::descending_vertical_traversal_tag());
//    fill_subtree_with_data(tree_inserter(bt2, bt2.root())); //FIXME

    validate_test_dataset1_tree(bt2.root());
    BOOST_CHECK_EQUAL(size(bt2.root()), size(bt.root()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE ( test_asc_copy_using_insert_cursor, Order, orders )
{    
    bt2.clear();
        
    boost::tree::copy(Order(), bt.root(), tree_inserter(bt2, bt2.root())
                    , boost::tree::ascending_vertical_traversal_tag());

    validate_test_dataset1_tree(bt2.root());
    BOOST_CHECK_EQUAL(size(bt2.root()), size(bt.root())); 
}

BOOST_AUTO_TEST_SUITE_END()