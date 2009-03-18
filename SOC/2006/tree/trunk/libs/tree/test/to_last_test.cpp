//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_fixture<int>)

BOOST_AUTO_TEST_CASE( test_to_last_preorder )
{
    fake_binary_tree<int>::root_tracking_cursor c = fbt1.root_tracking_root();
    boost::tree::to_last(preorder(), c);
    boost::tree::predecessor(preorder(), c);
    BOOST_CHECK_EQUAL(*c, 12);
}

BOOST_AUTO_TEST_CASE( test_to_last_inorder )
{
    fake_binary_tree<int>::root_tracking_cursor c = fbt1.root_tracking_root();
    boost::tree::to_last(inorder(), c);
    boost::tree::predecessor(inorder(), c);
    BOOST_CHECK_EQUAL(*c, 14);
}

BOOST_AUTO_TEST_CASE( test_to_last_postorder )
{
    fake_binary_tree<int>::root_tracking_cursor c = fbt1.root_tracking_root();
    boost::tree::to_last(postorder(), c);
    boost::tree::predecessor(postorder(), c);
    BOOST_CHECK_EQUAL(*c, 8);
}

BOOST_AUTO_TEST_SUITE_END()