//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_tree_traversal_data.hpp"
#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_fixture<int>)

BOOST_AUTO_TEST_CASE_TEMPLATE( test_successor, Order, orders )
{
    fake_binary_tree<int>::root_tracking_cursor c = fbt1.root_tracking_root(); //.begin();
    to_first(Order(), c);
    // Replace by a fake_to_first function for dependency minimization's sake?
    // preorder: fbt1.root_tracking_root().begin();
    // in- and postorder: fbt1.root_tracking_root().begin().begin().begin(); 

    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();

    for (; ci!=cie; ++ci) {
        BOOST_CHECK_EQUAL(*c, ci->second);
        boost::tree::successor(Order(), c);
    }
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