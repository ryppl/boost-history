//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/iterator.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/lambda/bind.hpp>

#include <list>
#include <algorithm>
#include <iterator>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, test_binary_tree_with_list_fixture<int>)

typedef boost::mpl::list<preorder,inorder,postorder> orders;

BOOST_AUTO_TEST_CASE_TEMPLATE( test_foreach, Order, orders)
{
    boost::tree::for_each(
        Order(),
        bt.root(), 
        boost::lambda::bind(&std::list<int>::push_back, &l, boost::lambda::_1)
    );
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy, Order, orders)
{
    boost::tree::copy(Order(), bt.root(), o);
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform, Order, orders)
{
    // First copy test_tree to test_tree2, by adding 1 to each element,
    // then copy test_tree2 to test_list, by subtracting 1 - so 
    // test_list should hold test_tree's original elements in ORDER.
    boost::tree::transform(Order(), bt.root(), bt2.root(), std::bind2nd(std::plus<int>(),1));
    boost::tree::transform(Order(), bt2.root(), o, std::bind2nd(std::minus<int>(),1));
    test_traversal(Order(), l.begin(), l.end());
}

BOOST_AUTO_TEST_SUITE_END()
