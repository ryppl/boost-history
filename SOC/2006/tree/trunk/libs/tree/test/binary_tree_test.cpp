//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>

#include <boost/tree/algorithm.hpp>

#define BOOST_TEST_MODULE binary_tree test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

BOOST_FIXTURE_TEST_SUITE(graph_algorithms_test, test_binary_tree_with_list_fixture<int>)

using namespace boost::tree;

template <class Tree>
void create_binary_tree(Tree& mytree)
{
    typename Tree::cursor c, c1, c2, c3, c4;
    
    c = mytree.root();

    BOOST_CHECK(c.empty());
    
    c1 = mytree.insert(c, 1);
    BOOST_CHECK(*c1 == 1);
    
    BOOST_CHECK(!c.empty());
    
    BOOST_CHECK(c1.m_node->m_parent != 0);
    BOOST_CHECK(c1.m_node->m_parent != c1.m_node);
    BOOST_CHECK(c1.m_node->m_parent == c.m_node);
    BOOST_CHECK(c1.parent() == c);
    
    c2 = mytree.insert(c1, 2);
    BOOST_CHECK(!c.empty());
    BOOST_CHECK(c2.empty());
    BOOST_CHECK(*c1 == 1);
    BOOST_CHECK(*c2 == 2);
    *c1 = 14;
    BOOST_CHECK(*c1 == 14);
    BOOST_CHECK(*c2 == 2);
    BOOST_CHECK(c2.parent() == c1);
    BOOST_CHECK(c1.parent() == c);
    
    c3 = c1.end();
    BOOST_CHECK(c3 == c1.end());
    --c3;
    BOOST_CHECK(c3.parity() == 0);
    BOOST_CHECK(c3.parent() != c3);
    BOOST_CHECK(c3.parent() == c1);
    BOOST_CHECK(c3 == c1.begin());
    
    *c3 = 3;
    *(c1.begin()) = 2;
    
    BOOST_CHECK(*c3 == 2);
    ++c3;
    c4 = mytree.insert(c3, 4);
    BOOST_CHECK(*c4 == 4);
    c4 = c4.parent();
    --c4;
    BOOST_CHECK(*c4 == 2);
    BOOST_CHECK(c4.parent() == c1);
    c = boost::tree::lower_bound(mytree.root(), 2, std::less<int>());
    BOOST_CHECK(*c == 2);
    BOOST_CHECK(c4.empty());

    BOOST_CHECK(*c1 == 14);
    
    BOOST_CHECK(c1.begin().empty() || c1.end().empty());
    
    //c1 = mytree.erase(c1);
    //BOOST_CHECK(*c1 == 2);

}

template <class Tree>
void inorder_erase_test_data_tree(Tree& mytree)
{
    typename Tree::cursor c = mytree.root().end().end().begin();
    BOOST_CHECK(*c == 14);
    
    c = c.parent().parent();
    BOOST_CHECK(*(c.begin()) == 10);
    BOOST_CHECK(c.begin().empty());
    BOOST_CHECK(!c.end().empty());
    BOOST_CHECK(*c.end().begin() == 14);
    c = c.begin();
    
    // Left child empty
    c = mytree.inorder_erase(c);
    BOOST_CHECK(*c == 11);

    ++c;
    BOOST_CHECK(*c.begin() == 12);
    BOOST_CHECK(c.begin().empty());
    BOOST_CHECK(c.end().empty());
    c = c.begin();
    
    // Both children empty
    c = mytree.inorder_erase(c);
    BOOST_CHECK(*c == 13);
}

template <class Tree>
void destroy_binary_tree(Tree& mytree)
{
    mytree.clear();    
    BOOST_CHECK(mytree.empty());
}

template <class Tree>
void validate_binary_tree(Tree const& mytree)
{
    typename Tree::const_cursor c, c1, c2, c3, c4;

    c = mytree.root();
    BOOST_CHECK(!c.empty());
    
    c1 = c.begin();
    BOOST_CHECK(c1.parent() == c);
    BOOST_CHECK(*c1 == 14);
    
    c2 = c1.begin();
    BOOST_CHECK(c2.parent() == c1);
    BOOST_CHECK(*c2 == 2);
    
    c3 = c1.end();
    BOOST_CHECK(c3.parent() == c1);
    BOOST_CHECK(*c3.begin() == 4);
    
}

template <class Tree>
void test_swap_binary_trees(Tree& one, Tree& two)
{
    using std::swap;
    swap(one, two);
}

BOOST_AUTO_TEST_CASE( constructors_test )
{
    typedef binary_tree<int> tree_t;
    tree_t bt;
    BOOST_CHECK(bt.root().empty());
    //BOOST_CHECK_EQUAL(0, 1);
    
    // test with allocator 
}

BOOST_AUTO_TEST_CASE( binary_tree_test )
{
    typedef binary_tree<int> tree_t;
    tree_t tree1, tree2;
    
    // Filling with test data.
    create_binary_tree(tree1);
    validate_binary_tree(tree1);

    // Swap tree1 with empty tree2
    test_swap_binary_trees(tree1, tree2);
    validate_binary_tree(tree2);
    BOOST_CHECK(tree1.empty());

    // Swap back
    test_swap_binary_trees(tree1, tree2);
    validate_binary_tree(tree1);
    BOOST_CHECK(tree2.empty());
    
    // Fill empty tree2 with different data
    //create_test_data_tree(tree2);
    validate_test_data_tree(bt);
    BOOST_CHECK(tree1 != bt);
    
    // Swap
    test_swap_binary_trees(tree1, bt);
    validate_test_data_tree(tree1);
    validate_binary_tree(bt);
    
    destroy_binary_tree(bt);

    // Insert subtree
    tree_t::cursor c = bt.insert(bt.root(), tree1.root());    
    BOOST_CHECK(*c == 8);
    validate_test_data_tree(bt);
    
    // Copy constructor
    tree_t tree3(bt);
    validate_test_data_tree(tree3);
    BOOST_CHECK(bt == tree3);
    
    // Change one value in test_tree3
    c = tree3.root().begin().end().begin().begin();
    int tmp = *c;
    *c = tmp + 1;
    BOOST_CHECK(bt != tree3);

    // Change it back
    c = tree3.root().begin().end().begin().begin();
    *c = tmp;
    BOOST_CHECK(bt == tree3);
    
    c = tree3.inorder_first();
    BOOST_CHECK(*c == 1);
    c = tree3.root();
    //inorder::back(c);
    //BOOST_CHECK(*c == 14);    
    
    destroy_binary_tree(bt);
    bt.splice(bt.root(), tree3);

    BOOST_CHECK(tree3.empty());    
    validate_test_data_tree(bt);
    c = bt.inorder_first();
    BOOST_CHECK(*c == 1);

    inorder_erase_test_data_tree(bt);
}

BOOST_AUTO_TEST_CASE( rotate_binary_tree_test )
{
    binary_tree<int>::cursor c = bt.root().begin().end();
    BOOST_CHECK(*c.begin() == 6);

    BOOST_CHECK(*c.parent() == 8);
    BOOST_CHECK(*c.parent().begin() == 3); // invariant candidate
    
    BOOST_CHECK(*--c == 3); // differently (not invariantly!) spoken
    BOOST_CHECK(*c.begin() == 1);
    BOOST_CHECK(*((++c).begin()).begin() == 4);
    BOOST_CHECK(*(++c.begin()).begin() == 7);

    BOOST_CHECK(c.parity() == 1);    
    BOOST_CHECK(*c.begin() == 6);
        
    bt.rotate(c); // Left rotate

    BOOST_CHECK(*c.begin() == 6);
    BOOST_CHECK(*c.parent().begin() == 8);
    
    BOOST_CHECK(*c.end().begin() == 7);
    BOOST_CHECK(*c.begin().begin() == 3);
    BOOST_CHECK(*c.begin().begin().begin() == 1);

    BOOST_CHECK(*c.begin().end().begin() == 4);

    c = c.begin();
    BOOST_CHECK(*c.begin() == 3);
    
    bt.rotate(c); // Right rotate
    
    BOOST_CHECK(*c.begin() == 3);
    c = c.end();

    BOOST_CHECK(*c.begin() == 6);

    BOOST_CHECK(*c.parent() == 8);
    BOOST_CHECK(*c.parent().begin() == 3); // other invariant candidate
    
    BOOST_CHECK(*--c == 3);
    BOOST_CHECK(*c.begin() == 1);
    BOOST_CHECK(*((++c).begin()).begin() == 4);
    BOOST_CHECK(*(++c.begin()).begin() == 7);
    
    BOOST_CHECK(*c.begin() == 6);
    
//    BOOST_CHECK(*c.parent().parent().begin() == 6);
//    BOOST_CHECK(*c.parent().parent().end().begin() == 7);
    
//    BOOST_CHECK(*c.begin() == 1);
//    BOOST_CHECK(*c.parent().begin() == 3); // invariant?
//    
//    BOOST_CHECK(*c.parent().parent().begin() == 6);
//    BOOST_CHECK(*c.parent().parent().parent().begin() == 8);
//    BOOST_CHECK(*c.parent().parent().end().begin() == 7);
    
}

BOOST_AUTO_TEST_SUITE_END()