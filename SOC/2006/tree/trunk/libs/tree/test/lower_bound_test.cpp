//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(binary_tree_search_test, fake_binary_tree_fixture<int>)

BOOST_AUTO_TEST_CASE( lower_bound_test )
{   
    fake_binary_tree<int>::cursor c(fbt1, 0), d(fbt1, 0);

    c = lower_bound(fbt1.root(), 4); // (Left) Leaf

//    TODO: Test by applying std::lower_bound to the inorder sorted data and comparing
//          with the result of that operation.     
//    test_data_set mpo;
//    mock_cursor_data(mpo);
//    test_data_set::index<inorder>::type::const_iterator ci 
//    = std::lower_bound(mpo.get<inorder>().begin(), mpo.get<inorder>().end(), 4); // Need a predicate
//    BOOST_CHECK_EQUAL(*c, ci->val);
    
    BOOST_CHECK_EQUAL(*c, 4);
    c = lower_bound(fbt1.root(), 7); // (Right) Leaf
    BOOST_CHECK_EQUAL(*c, 7);
    c = lower_bound(fbt1.root(), 6); // Non-Leaf
    BOOST_CHECK_EQUAL(*c, 6);
    c = lower_bound(fbt1.root(), 8); // Non-Leaf: root
    BOOST_CHECK_EQUAL(*c, 8);

    c = lower_bound(fbt1.root(), 5); // Not in tree
    BOOST_CHECK_EQUAL(*c, 6);
}

BOOST_AUTO_TEST_CASE( lower_bound_pred_test )
{   
    fake_binary_tree<int>::cursor c(fbt1, 0), d(fbt1, 0);

    c = lower_bound(fbt1.root(), 4, std::less<int>()); // (Left) Leaf
    BOOST_CHECK_EQUAL(*c, 4);
    c = lower_bound(fbt1.root(), 7, std::less<int>()); // (Right) Leaf
    BOOST_CHECK_EQUAL(*c, 7);
    c = lower_bound(fbt1.root(), 6, std::less<int>()); // Non-Leaf
    BOOST_CHECK_EQUAL(*c, 6);
    c = lower_bound(fbt1.root(), 8, std::less<int>()); // Non-Leaf: root
    BOOST_CHECK_EQUAL(*c, 8);

    c = lower_bound(fbt1.root(), 5, std::less<int>()); // Not in tree
    BOOST_CHECK_EQUAL(*c, 6);
}

BOOST_AUTO_TEST_SUITE_END()