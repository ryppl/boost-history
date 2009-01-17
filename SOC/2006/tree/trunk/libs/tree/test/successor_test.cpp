//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_fixture<int>)

// TODO: iterate over all elements.

BOOST_AUTO_TEST_CASE( test_successor_preorder )
{
    fake_binary_tree<int>::root_tracking_cursor c = fbt1.root_tracking_root().begin();
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 3);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 1);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 6);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 4);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 7);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 10);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 14);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 13);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 11);
    boost::tree::successor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 12);
}

BOOST_AUTO_TEST_CASE( test_successor_inorder )
{
    fake_binary_tree<int>::root_tracking_cursor c
    = fbt1.root_tracking_root().begin().begin().begin().begin();
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 1);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 3);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 4);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 6);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 7);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 8);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 10);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 11);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 12);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 13);
    boost::tree::successor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 14);
}

BOOST_AUTO_TEST_CASE( test_successor_postorder )
{
    fake_binary_tree<int>::root_tracking_cursor c
    = fbt1.root_tracking_root().begin().begin().begin().begin();
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 1);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 4);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 7);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 6);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 3);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 12);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 11);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 13);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 14);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 10);
    boost::tree::successor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 8);
}

BOOST_AUTO_TEST_CASE( test_successor_ascending )
{
    fake_binary_tree<int>::ascending_cursor c = fbt1.ascending_root();
    c.to_begin().to_end().to_begin().to_begin();
    BOOST_CHECK_EQUAL(*c, 4);
    boost::tree::successor(ascending(), c);
    BOOST_CHECK_EQUAL(*c, 6);
    boost::tree::successor(ascending(), c);
    BOOST_CHECK_EQUAL(*c, 3);
    boost::tree::successor(ascending(), c);
    BOOST_CHECK_EQUAL(*c, 8);
}

BOOST_AUTO_TEST_SUITE_END()