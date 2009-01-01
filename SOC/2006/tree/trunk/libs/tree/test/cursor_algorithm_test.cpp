//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#include <boost/lambda/bind.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_with_list_fixture<int>)

BOOST_AUTO_TEST_CASE_TEMPLATE( test_foreach_descending, Order, orders)
{
    boost::tree::for_each(
        Order(),
        fbt1.descending_root(), 
        boost::lambda::bind(&std::list<int>::push_back, &l, boost::lambda::_1)
    );
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_foreach_ascending, Order, orders)
{
    boost::tree::for_each(
        Order(),
        fbt1.ascending_root(), 
        boost::lambda::bind(&std::list<int>::push_back, &l, boost::lambda::_1)
    );
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE( test_foreach_subtree3_descending )
{
    boost::tree::for_each(
        preorder(),
        fbt1.descending_root().begin(), 
        boost::lambda::bind(&std::list<int>::push_back, &l, boost::lambda::_1)
    );
    test_subtree_traversal(preorder(), l.begin(), l.end(), 1);
    BOOST_CHECK_EQUAL(l.size(), 5);
}

BOOST_AUTO_TEST_CASE( test_foreach_subtree3_ascending )
{
    boost::tree::for_each(
        preorder(),
        fbt1.ascending_root().begin(), 
        boost::lambda::bind(&std::list<int>::push_back, &l, boost::lambda::_1)
    );
    test_subtree_traversal(preorder(), l.begin(), l.end(), 1);
    BOOST_CHECK_EQUAL(l.size(), 5);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_descending, Order, orders)
{
    boost::tree::copy(Order(), fbt1.descending_root(), o);
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_ascending, Order, orders)
{
    boost::tree::copy(Order(), fbt1.ascending_root(), o);
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_trees_descending, Order, orders)
{
    BOOST_CHECK(fbt1 != fbt2);
    boost::tree::copy(Order(), fbt1.descending_root(), fbt2.descending_root());
    BOOST_CHECK(fbt1 == fbt2);
    validate_test_dataset1_tree(fbt2.descending_root());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_trees_ascending, Order, orders)
{
    BOOST_CHECK(fbt1 != fbt2);
    boost::tree::copy(Order(), fbt1.ascending_root(), fbt2.ascending_root());
    BOOST_CHECK(fbt1 == fbt2);
    validate_test_dataset1_tree(fbt2.ascending_root());
}

//BOOST_AUTO_TEST_CASE_TEMPLATE ( test_desc_copy_using_insert_cursor, Order, orders )
//{
//    bt2.clear();
//
//    boost::tree::copy(Order(), bt.root(), tree_inserter(bt2, bt2.root())
//                    , boost::forward_traversal_tag());
//
//    validate_test_dataset1_tree(bt2.root());
//    BOOST_CHECK_EQUAL(size(bt2.root()), size(bt.root()));
//}
//
//BOOST_AUTO_TEST_CASE_TEMPLATE ( test_asc_copy_using_insert_cursor, Order, orders )
//{    
//    bt2.clear();
//        
//    boost::tree::copy(Order(), bt.root(), tree_inserter(bt2, bt2.root())
//                    , boost::bidirectional_traversal_tag());
//
//    validate_test_dataset1_tree(bt2.root());
//    BOOST_CHECK_EQUAL(size(bt2.root()), size(bt.root())); 
//}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform_descending, Order, orders)
{
    // First copy test_tree to test_tree2, by adding 1 to each element,
    // then copy test_tree2 to test_list, by subtracting 1 - so 
    // test_list should hold test_tree's original elements in ORDER.
    boost::tree::transform(Order(), fbt1.descending_root(), fbt2.descending_root(), std::bind2nd(std::plus<int>(),1));
    boost::tree::transform(Order(), fbt2.descending_root(), o, std::bind2nd(std::minus<int>(),1));
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform_ascending, Order, orders)
{
    // First copy test_tree to test_tree2, by adding 1 to each element,
    // then copy test_tree2 to test_list, by subtracting 1 - so 
    // test_list should hold test_tree's original elements in ORDER.
    boost::tree::transform(Order(), fbt1.ascending_root(), fbt2.ascending_root(), std::bind2nd(std::plus<int>(),1));
    boost::tree::transform(Order(), fbt2.ascending_root(), o, std::bind2nd(std::minus<int>(),1));
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform_trees_descending, Order, orders)
{
    BOOST_CHECK(fbt1 != fbt2);
    boost::tree::transform(Order(), fbt1.descending_root(), fbt2.descending_root()
                         , std::bind2nd(std::minus<int>(),1));
    validate_test_dataset1_minus_1_tree(fbt2.descending_root());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform_trees_ascending, Order, orders)
{
    BOOST_CHECK(fbt1 != fbt2);
    boost::tree::transform(Order(), fbt1.ascending_root(), fbt2.ascending_root()
                         , std::bind2nd(std::minus<int>(),1));
    validate_test_dataset1_minus_1_tree(fbt2.ascending_root());
}


BOOST_AUTO_TEST_SUITE_END()