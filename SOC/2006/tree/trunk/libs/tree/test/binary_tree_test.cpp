//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>

#define BOOST_TEST_MODULE binary_tree test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include "binary_tree_test_data.hpp"
#include "test_tree_traversal_data.hpp"
#include "fake_binary_tree.hpp"

using boost::tree::binary_tree;

BOOST_AUTO_TEST_SUITE( basic_binary_tree_test )

BOOST_AUTO_TEST_CASE( constructors_test )
{
    binary_tree<int> bt0;
    BOOST_CHECK(bt0.root().is_leaf());
    
//    binary_tree<int>::node_base_type::node_with_children_base** x = bt0.m_header.m_children.data();
//    BOOST_CHECK_EQUAL(++x, &bt0.m_header.m_children.data()[1]);

    //BOOST_CHECK(bt0.root().begin() == bt0.root().end()); //FIXME
    // test with allocator? 
}

BOOST_AUTO_TEST_CASE( insert_value_test )
{
    binary_tree<int> bt0;
    
    binary_tree<int>::cursor c = bt0.insert(bt0.root()/*.begin()*/, 8); //FIXME   
    c.to_begin();

    BOOST_CHECK(c == bt0.root().begin());
    BOOST_CHECK(bt0.root().begin().parent() == bt0.root());
    BOOST_CHECK(!bt0.root().is_leaf());
    BOOST_CHECK_EQUAL(*bt0.root().begin(), 8);
    BOOST_CHECK(bt0.root().begin().is_leaf());
    
    c = bt0.insert(c, 3);
    c.to_begin();
    
    // The 8 valued cursor still ok?
    BOOST_CHECK(bt0.root().begin().parent() == bt0.root());
    BOOST_CHECK(!bt0.root().is_leaf());
    BOOST_CHECK_EQUAL(*bt0.root().begin(), 8);
    
    // The 3 valued cursor.
    BOOST_CHECK(c == bt0.root().begin().begin());
    BOOST_CHECK(bt0.root().begin().begin().parent() == bt0.root().begin());
    BOOST_CHECK(!bt0.root().begin().is_leaf());
    BOOST_CHECK_EQUAL(*bt0.root().begin().begin(), 3);
    BOOST_CHECK(bt0.root().begin().begin().is_leaf());
    
    BOOST_CHECK(++c == bt0.root().begin().end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE( binary_tree_using_fake_binary_tree_test
                        , fake_binary_tree_fixture<int> )

BOOST_AUTO_TEST_CASE( subtree_constructor_test )
{
    binary_tree<int> bt0(fbt1.root());
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_CASE( insert_subtree_test )
{
    binary_tree<int> bt0;
    bt0.insert(bt0.root(), fbt1.root());    
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_CASE( assign_subtree_test )
{
    binary_tree<int> bt0;
    bt0.assign(fbt1.root());    
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE( binary_tree_test, test_binary_tree_fixture<int> )

using namespace boost::tree;

template <class Tree>
void create_test_dataset2_tree(Tree& mytree)
{
    typename Tree::cursor c, c1, c2, c3, c4;
    
    c = mytree.root();

    BOOST_CHECK(c.is_leaf());
    
    c1 = mytree.insert(c, 1);
    c1.to_begin();

    BOOST_CHECK_EQUAL(*c1, 1);
    
    BOOST_CHECK(!c.is_leaf());
    
    BOOST_CHECK(c1.parent() == c);
    
    c2 = mytree.insert(c1, 2);
    c2.to_begin();

    BOOST_CHECK(!c.is_leaf());
    BOOST_CHECK(c2.is_leaf());
    BOOST_CHECK_EQUAL(*c1, 1);
    BOOST_CHECK_EQUAL(*c2, 2);
    *c1 = 14;
    BOOST_CHECK_EQUAL(*c1, 14);
    BOOST_CHECK_EQUAL(*c2, 2);
    BOOST_CHECK(c2.parent() == c1);
    BOOST_CHECK(c1.parent() == c);
    
    c3 = c1.end();
    BOOST_CHECK(c3 == c1.end());
    --c3;
    BOOST_CHECK_EQUAL(index(c3), 0);
    BOOST_CHECK(c3.parent() != c3);
    BOOST_CHECK(c3.parent() == c1);
    BOOST_CHECK(c3 == c1.begin());
    
    *c3 = 3;
    *(c1.begin()) = 2;
    
    BOOST_CHECK_EQUAL(*c3, 2);
    ++c3;
    c4 = mytree.insert(c3, 4);
    c4.to_begin();

    BOOST_CHECK_EQUAL(*c4, 4);
    c4 = c4.parent();
    --c4;
    BOOST_CHECK_EQUAL(*c4, 2);
    BOOST_CHECK(c4.parent() == c1);
    BOOST_CHECK(c4.is_leaf());

    BOOST_CHECK_EQUAL(*c1, 14);
    
    BOOST_CHECK(c1.begin().is_leaf() || c1.end().is_leaf());
}

template <class Cursor>
void validate_test_dataset2_tree(Cursor cur)
{
    Cursor c = cur;

    BOOST_CHECK(!c.is_leaf());
    
    c.to_begin();
    BOOST_CHECK(c.parent() == cur);
    BOOST_CHECK_EQUAL(*c, 14);
    
    c.to_begin();
    BOOST_CHECK(c.parent() == cur.begin());
    BOOST_CHECK_EQUAL(*c, 2);
    
    ++c;
    BOOST_CHECK(c.parent() == cur.begin());
    BOOST_CHECK_EQUAL(*c.begin(), 4);
    
}

//BOOST_AUTO_TEST_SUITE( binary_tree_erase_tests )

BOOST_AUTO_TEST_CASE( erase_right_non_leaf_right_node_test )
{
    binary_tree<int>::cursor c = bt.root().end().end();
    BOOST_CHECK_EQUAL(*--c, 10);

    // c has no left child, but a right one.
    BOOST_CHECK(c.is_leaf());
    BOOST_CHECK(!(++c).is_leaf());
    
    // c is its parent's right child
    binary_tree<int>::size_type sz = size(bt.root());
    c = bt.erase(c);
    BOOST_CHECK_EQUAL(--sz, size(bt.root()));
    
    BOOST_CHECK(c == bt.root().end().end());
    BOOST_CHECK_EQUAL(*--c, 14);
}

BOOST_AUTO_TEST_CASE( erase_right_non_leaf_left_node_test )
{
    binary_tree<int>::cursor c = bt.root().end().end().begin();
    BOOST_CHECK_EQUAL(*c, 14);

    // c has a left child, but no right one.
    BOOST_CHECK(!c.is_leaf());
    BOOST_CHECK((++c).is_leaf());
    --c;

    // c is its parent's right child
    binary_tree<int>::size_type sz = size(bt.root());
    c = bt.erase(c);
    BOOST_CHECK_EQUAL(--sz, size(bt.root()));
    
    BOOST_CHECK(c == bt.root().end().end().begin());
    BOOST_CHECK_EQUAL(*c, 13);
}

BOOST_AUTO_TEST_CASE( erase_left_non_leaf_left_node_test )
{
    binary_tree<int>::cursor c = bt.root().end().end().begin().begin();
    BOOST_CHECK_EQUAL(*c, 13);

    // c has a left child, but no right one.
    BOOST_CHECK(!c.is_leaf());
    BOOST_CHECK((++c).is_leaf());
    --c;

    // c is its parent's left child
    binary_tree<int>::size_type sz = size(bt.root());
    c = bt.erase(c);
    BOOST_CHECK_EQUAL(--sz, size(bt.root()));
    
    BOOST_CHECK(c == bt.root().end().end().begin().begin());
    BOOST_CHECK_EQUAL(*c, 11);
}

BOOST_AUTO_TEST_CASE( erase_left_non_leaf_right_node_test )
{
    binary_tree<int>::cursor c = bt.root().end().end().begin().begin().end();
    BOOST_CHECK_EQUAL(*--c, 11);

    // c has no left child, but a right one.
    BOOST_CHECK(c.is_leaf());
    BOOST_CHECK(!(++c).is_leaf());

    // c is its parent's right child
    binary_tree<int>::size_type sz = size(bt.root());
    c = bt.erase(c);
    BOOST_CHECK_EQUAL(--sz, size(bt.root()));
    
    BOOST_CHECK(c == bt.root().end().end().begin().begin().end());
    BOOST_CHECK_EQUAL(*c, 12);
}

BOOST_AUTO_TEST_CASE( erase_left_leaf_node_test )
{
    binary_tree<int>::cursor c = bt.root().end().end().begin().begin().end().begin();
    BOOST_CHECK_EQUAL(*c, 12);

    // Both children empty
    BOOST_CHECK(c.is_leaf());
    BOOST_CHECK((++c).is_leaf());
    --c;

    // c is its parent's left child
    binary_tree<int>::size_type sz = size(bt.root());
    c = bt.erase(c);
    BOOST_CHECK_EQUAL(--sz, size(bt.root()));

    BOOST_CHECK(c == bt.root().end().end().begin().end().begin());
}

BOOST_AUTO_TEST_CASE( erase_right_leaf_node_test )
{
    binary_tree<int>::cursor c = bt.root().begin().end().end().begin();
    BOOST_CHECK_EQUAL(*c, 7);

    // Both children empty
    BOOST_CHECK(c.is_leaf());
    BOOST_CHECK((++c).is_leaf());
    --c;

    // c is its parent's right child
    binary_tree<int>::size_type sz = size(bt.root());
    c = bt.erase(c);
    BOOST_CHECK_EQUAL(--sz, size(bt.root()));

    BOOST_CHECK(c == bt.root().begin().end().end().begin());
}

//BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE( clear_subtree_test )
{
    binary_tree<int>::size_type sz = size(bt.root());
    binary_tree<int>::cursor c = bt.root().begin();
    BOOST_CHECK(!c.is_leaf());
    sz -= size(c);

    BOOST_CHECK(!c.is_leaf());
    c = bt.clear(c);
    BOOST_CHECK(c == bt.root().begin());
    BOOST_CHECK(c.is_leaf());
    BOOST_CHECK_EQUAL(*c, 8);
    BOOST_CHECK_EQUAL(sz, size(bt.root()));
}

BOOST_AUTO_TEST_CASE( clear_root_test )
{
    binary_tree<int>::cursor c = bt.root();

    BOOST_CHECK(!c.is_leaf());
    c = bt.clear(c);
    BOOST_CHECK(c.is_leaf());
    BOOST_CHECK(c == bt.root());
    BOOST_CHECK(bt.empty());
}

BOOST_AUTO_TEST_CASE( clear_test )
{
    bt.clear();    
    BOOST_CHECK(bt.empty());
    BOOST_CHECK(bt.root().is_leaf());
}

BOOST_AUTO_TEST_CASE( swap_binary_tree_test )
{
    using std::swap;
    typedef binary_tree<int> tree_t;
    tree_t tree1, tree2;
    
    // Filling with test data.
    create_test_dataset2_tree(tree1);
    validate_test_dataset2_tree(tree1.root());

    // Swap tree1 with empty tree2
    swap(tree1, tree2);
    validate_test_dataset2_tree(tree2.root());
    BOOST_CHECK(tree1.empty());
    
    // Swap back
    swap(tree1, tree2);
    validate_test_dataset2_tree(tree1.root());
    BOOST_CHECK(tree2.empty());
    
    // Swap with tree containing different data
    swap(tree1, bt);
    validate_test_dataset1_tree(tree1.root());
    validate_test_dataset2_tree(bt.root());
}

BOOST_AUTO_TEST_CASE( copy_constructor_test )
{
    binary_tree<int> bt0(bt);
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_CASE( assignment_operator_test )
{
    binary_tree<int> bt0;
    bt0 = bt;
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_CASE( comparison_operator_test )
{
    BOOST_CHECK(bt != bt2);
    *bt2.root().begin().end().begin().begin()
        = *bt.root().begin().end().begin().begin();
    BOOST_CHECK(bt == bt2);
}

BOOST_AUTO_TEST_CASE( splice_subtree_test )
{
    binary_tree<int> bt0;
    bt0.insert(bt0.root(), 8);
    bt0.splice(bt0.root().begin(), bt, bt.root().begin());
    BOOST_CHECK(bt.root().begin().is_leaf());
    bt0.splice(bt0.root().end(), bt, bt.root().end());
    BOOST_CHECK(bt.root().end().is_leaf());

    BOOST_CHECK_EQUAL(*bt.root().begin(), 8);
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_CASE( splice_test )
{
    binary_tree<int> bt0;
    bt0.splice(bt0.root(), bt);

    BOOST_CHECK(bt.empty());    
    validate_test_dataset1_tree(bt0.root());
}

BOOST_AUTO_TEST_CASE( rotate_binary_tree_test )
{
    binary_tree<int>::cursor c = bt.root().begin().end();
    BOOST_CHECK_EQUAL(*c.begin(), 6);

    BOOST_CHECK_EQUAL(*c.parent(), 8);
    BOOST_CHECK_EQUAL(*c.parent().begin(), 3); // invariant candidate
    
    BOOST_CHECK_EQUAL(*--c, 3); // differently (not invariantly!) spoken
    BOOST_CHECK_EQUAL(*c.begin(), 1);
    BOOST_CHECK_EQUAL(*((++c).begin()).begin(), 4);
    BOOST_CHECK_EQUAL(*(++c.begin()).begin(), 7);

    BOOST_CHECK_EQUAL(index(c), 1);    
    BOOST_CHECK_EQUAL(*c.begin(), 6);
        
    bt.rotate(c); // Left rotate

    BOOST_CHECK_EQUAL(*c.begin(), 6);
    BOOST_CHECK_EQUAL(*c.parent().begin(), 8);
    
    BOOST_CHECK_EQUAL(*c.end().begin(), 7);
    BOOST_CHECK_EQUAL(*c.begin().begin(), 3);
    BOOST_CHECK_EQUAL(*c.begin().begin().begin(), 1);

    BOOST_CHECK_EQUAL(*c.begin().end().begin(), 4);

    c = c.begin();
    BOOST_CHECK_EQUAL(*c.begin(), 3);
    
    bt.rotate(c); // Right rotate
    
    BOOST_CHECK_EQUAL(*c.begin(), 3);
    c = c.end();

    BOOST_CHECK_EQUAL(*c.begin(), 6);

    BOOST_CHECK_EQUAL(*c.parent(), 8);
    BOOST_CHECK_EQUAL(*c.parent().begin(), 3); // other invariant candidate
    
    BOOST_CHECK_EQUAL(*--c, 3);
    BOOST_CHECK_EQUAL(*c.begin(), 1);
    BOOST_CHECK_EQUAL(*((++c).begin()).begin(), 4);
    BOOST_CHECK_EQUAL(*(++c.begin()).begin(), 7);
    
    BOOST_CHECK_EQUAL(*c.begin(), 6);
    
//    BOOST_CHECK_EQUAL(*c.parent().parent().begin(), 6);
//    BOOST_CHECK_EQUAL(*c.parent().parent().end().begin(), 7);
    
//    BOOST_CHECK_EQUAL(*c.begin(), 1);
//    BOOST_CHECK_EQUAL(*c.parent().begin(), 3); // invariant?
//    
//    BOOST_CHECK_EQUAL(*c.parent().parent().begin(), 6);
//    BOOST_CHECK_EQUAL(*c.parent().parent().parent().begin(), 8);
//    BOOST_CHECK_EQUAL(*c.parent().parent().end().begin(), 7);
    
}

BOOST_AUTO_TEST_SUITE_END()