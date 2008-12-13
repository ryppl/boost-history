//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/iterator.hpp>
#include <boost/tree/algorithm.hpp>
#include <boost/tree/ascending_cursor.hpp>
#include <boost/tree/root_tracking_cursor.hpp>

#include <list>
#include <iterator>

#define BOOST_TEST_MODULE iterator_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE( iterator_algorithms_test, test_binary_tree_with_list_fixture<int> )

BOOST_AUTO_TEST_CASE_TEMPLATE( test_iterator_algorithms, Order, orders )
{
    test_traversal(Order(), begin(Order(), bt.root())
                          , end(Order(), bt.root()));

    test_reverse_traversal(Order(), end(Order(), bt.root())
                                  , begin(Order(), bt.root()));
                                    
    BOOST_CHECK_EQUAL(std::distance(begin(Order(), bt.root()) 
                                  , end(Order(), bt.root())), 11);

    // TODO: Also check with binary_tree-specialized inorder begin()!

    // Now the iterators based on stack-based cursors (that don't use cursor.to_parent())

    test_traversal(Order(), begin(Order(), make_ascending_cursor(bt.root()))
                          , end(Order(), make_ascending_cursor(bt.root())));
    test_reverse_traversal(Order(), end(Order(), make_ascending_cursor(bt.root()))
                                  , begin(Order(), make_ascending_cursor(bt.root())));
    BOOST_CHECK_EQUAL(std::distance(
                        begin(Order(), make_ascending_cursor(bt.root())) 
                      , end(Order(), make_ascending_cursor(bt.root()))), 11);
}

BOOST_AUTO_TEST_CASE( test_subtree3_iterator_algorithms )
{
    test_subtree_traversal(preorder(), begin(preorder(), bt.root().begin())
                                     , end(preorder(), bt.root().begin()), 1);
    BOOST_CHECK_EQUAL(std::distance(begin(preorder(), bt.root().begin())
                                  , end(preorder(), bt.root().begin())), 5);

    test_subtree_traversal(inorder(), begin(inorder(), bt.root().begin())
                                    , end(inorder(), bt.root().begin()), 0);
    BOOST_CHECK_EQUAL(std::distance(begin(inorder(), bt.root().begin())
                                  , end(inorder(), bt.root().begin())), 5);

    test_subtree_traversal(postorder(), begin(postorder(), bt.root().begin())
                                      , end(postorder(), bt.root().begin()), 0);
    BOOST_CHECK_EQUAL(std::distance(begin(postorder(), bt.root().begin())
                                  , end(postorder(), bt.root().begin())), 5);
}

BOOST_AUTO_TEST_CASE( test_subtree6_iterator_algorithms )
{
    test_subtree_traversal(preorder(), begin(preorder(), bt.root().begin().end())
                                     , end(preorder(), bt.root().begin().end()), 3);
    BOOST_CHECK_EQUAL(std::distance(begin(preorder(), bt.root().begin().end())
                                  , end(preorder(), bt.root().begin().end())), 3);

    test_subtree_traversal(inorder(), begin(inorder(), bt.root().begin().end())
                                    , end(inorder(), bt.root().begin().end()), 2);
    BOOST_CHECK_EQUAL(std::distance(begin(inorder(), bt.root().begin().end())
                                  , end(inorder(), bt.root().begin().end())), 3);

    test_subtree_traversal(postorder(), begin(postorder(), bt.root().begin().end())
                                      , end(postorder(), bt.root().begin().end()), 1);
    BOOST_CHECK_EQUAL(std::distance(begin(postorder(), bt.root().begin().end())
                                  , end(postorder(), bt.root().begin().end())), 3);
}

BOOST_AUTO_TEST_CASE( test_subtree10_iterator_algorithms )
{
    test_subtree_traversal(preorder(), begin(preorder(), bt.root().end())
                                     , end(preorder(), bt.root().end()), 6);
    BOOST_CHECK_EQUAL(std::distance(begin(preorder(), bt.root().end())
                                  , end(preorder(), bt.root().end())), 5);

    test_subtree_traversal(inorder(), begin(inorder(), bt.root().end())
                                    , end(inorder(), bt.root().end()), 6);
    BOOST_CHECK_EQUAL(std::distance(begin(inorder(), bt.root().end())
                                  , end(inorder(), bt.root().end())), 5);

    test_subtree_traversal(postorder(), begin(postorder(), bt.root().end())
                                      , end(postorder(), bt.root().end()), 5);
    BOOST_CHECK_EQUAL(std::distance(begin(postorder(), bt.root().end())
                                  , end(postorder(), bt.root().end())), 5);
}

BOOST_AUTO_TEST_CASE( test_ascending_iterator_algorithms )
{
    binary_tree<int>::cursor c = bt.root();
    typedef boost::tree::root_tracking_cursor<binary_tree<int>::cursor> rtc;
    typedef boost::tree::iterator<ascending, rtc> ai;
    c.to_begin().to_end().to_begin().to_begin();
    BOOST_CHECK_EQUAL(*c, 4);

    test_traversal_from_leaf4(ai(rtc(c)), ai(rtc(bt.root())));
}

BOOST_AUTO_TEST_SUITE_END()