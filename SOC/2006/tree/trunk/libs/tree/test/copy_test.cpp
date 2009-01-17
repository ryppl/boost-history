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

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test_with_fixture
                       , fake_binary_tree_with_list_fixture<int>)

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_descending, Order, orders)
{
    boost::tree::copy(Order(), fbt1.descending_root(), o);
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE( alternate_test_copy_descending )
{
    std::vector<int> po(11);
    po[0] = 8;
    po[1] = 3;
    po[2] = 1;
    po[3] = 6;
    po[4] = 4;
    po[5] = 7;
    po[6] = 10;
    po[7] = 14;
    po[8] = 13;
    po[9] = 11;
    po[10] = 12;
    std::vector<int>::const_iterator ci = po.begin();
    mock_cursor< std::vector<int>::const_iterator > mc(ci);
    
    boost::tree::copy(preorder(), fbt1.descending_root(), mc);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_ascending, Order, orders)
{
    boost::tree::copy(Order(), fbt1.ascending_root(), o);
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_fixture<int>)

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

BOOST_AUTO_TEST_SUITE_END()