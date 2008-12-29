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

#include "fake_binary_tree.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE( iterator_algorithms_test, fake_binary_tree_with_list_fixture<int> )

BOOST_AUTO_TEST_CASE_TEMPLATE( test_iterator_algorithms, Order, orders )
{
    fake_ascending_binary_cursor<int> afbtc1(fbt1, 0);

    test_traversal(Order(), begin(Order(), afbtc1)
                          , end(Order(), afbtc1));

    test_reverse_traversal(Order(), end(Order(), afbtc1)
                                  , begin(Order(), afbtc1));
                                    
    BOOST_CHECK_EQUAL(std::distance(begin(Order(), afbtc1) 
                                  , end(Order(), afbtc1)), 11);

    // TODO: Also check with binary_tree-specialized inorder begin()!

    // Now the iterators based on stack-based cursors (that don't use cursor.to_parent())

// FIXME
//    test_traversal(Order(), begin(Order(), make_ascending_cursor(fbt1.descending_root()))
//                          , end(Order(), make_ascending_cursor(fbt1.descending_root())));
//    test_reverse_traversal(Order(), end(Order(), make_ascending_cursor(fbt1.descending_root()))
//                                  , begin(Order(), make_ascending_cursor(fbt1.descending_root())));
//    BOOST_CHECK_EQUAL(std::distance(
//                        begin(Order(), make_ascending_cursor(fbt1.descending_root())) 
//                      , end(Order(), make_ascending_cursor(fbt1.descending_root()))), 11);
}

BOOST_AUTO_TEST_CASE( test_subtree3_iterator_algorithms )
{
    fake_ascending_binary_cursor<int> afbtc1(fbt1, 0);

    test_subtree_traversal(preorder(), begin(preorder(), afbtc1.begin())
                                     , end(preorder(), afbtc1.begin()), 1);
    BOOST_CHECK_EQUAL(std::distance(begin(preorder(), afbtc1.begin())
                                  , end(preorder(), afbtc1.begin())), 5);

    test_subtree_traversal(inorder(), begin(inorder(), afbtc1.begin())
                                    , end(inorder(), afbtc1.begin()), 0);
    BOOST_CHECK_EQUAL(std::distance(begin(inorder(), afbtc1.begin())
                                  , end(inorder(), afbtc1.begin())), 5);

    test_subtree_traversal(postorder(), begin(postorder(), afbtc1.begin())
                                      , end(postorder(), afbtc1.begin()), 0);
    BOOST_CHECK_EQUAL(std::distance(begin(postorder(), afbtc1.begin())
                                  , end(postorder(), afbtc1.begin())), 5);
}

BOOST_AUTO_TEST_CASE( test_subtree6_iterator_algorithms )
{
    fake_ascending_binary_cursor<int> afbtc1(fbt1, 0);

    test_subtree_traversal(preorder(), begin(preorder(), afbtc1.begin().end())
                                     , end(preorder(), afbtc1.begin().end()), 3);
    BOOST_CHECK_EQUAL(std::distance(begin(preorder(), afbtc1.begin().end())
                                  , end(preorder(), afbtc1.begin().end())), 3);

    test_subtree_traversal(inorder(), begin(inorder(), afbtc1.begin().end())
                                    , end(inorder(), afbtc1.begin().end()), 2);
    BOOST_CHECK_EQUAL(std::distance(begin(inorder(), afbtc1.begin().end())
                                  , end(inorder(), afbtc1.begin().end())), 3);

    test_subtree_traversal(postorder(), begin(postorder(), afbtc1.begin().end())
                                      , end(postorder(), afbtc1.begin().end()), 1);
    BOOST_CHECK_EQUAL(std::distance(begin(postorder(), afbtc1.begin().end())
                                  , end(postorder(), afbtc1.begin().end())), 3);
}

BOOST_AUTO_TEST_CASE( test_subtree10_iterator_algorithms )
{
    fake_ascending_binary_cursor<int> afbtc1(fbt1, 0);

    test_subtree_traversal(preorder(), begin(preorder(), afbtc1.end())
                                     , end(preorder(), afbtc1.end()), 6);
    BOOST_CHECK_EQUAL(std::distance(begin(preorder(), afbtc1.end())
                                  , end(preorder(), afbtc1.end())), 5);

    test_subtree_traversal(inorder(), begin(inorder(), afbtc1.end())
                                    , end(inorder(), afbtc1.end()), 6);
    BOOST_CHECK_EQUAL(std::distance(begin(inorder(), afbtc1.end())
                                  , end(inorder(), afbtc1.end())), 5);

    test_subtree_traversal(postorder(), begin(postorder(), afbtc1.end())
                                      , end(postorder(), afbtc1.end()), 5);
    BOOST_CHECK_EQUAL(std::distance(begin(postorder(), afbtc1.end())
                                  , end(postorder(), afbtc1.end())), 5);
}

BOOST_AUTO_TEST_CASE( test_ascending_iterator_algorithms )
{
    typedef fake_ascending_binary_cursor<int> cursor;
    cursor c(fbt1, 0);
    typedef boost::tree::root_tracking_cursor<cursor> rtc;
    typedef boost::tree::iterator<ascending, rtc> ai;
    c.to_begin().to_end().to_begin().to_begin();
    BOOST_CHECK_EQUAL(*c, 4);

    test_traversal_from_leaf4(ai(rtc(c)), ai(rtc(cursor(fbt1,0))));
}

BOOST_AUTO_TEST_SUITE_END()