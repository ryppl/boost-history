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
#include "mock_binary_cursor.hpp"

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test_with_fixture
                       , fake_binary_tree_fixture<int>)

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_descending, Order, orders )
{
    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();
    mock_binary_cursor< container_type::const_iterator > mc(ci, cie);
    
    boost::tree::copy(Order(), fbt1.descending_root(), mc);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy_ascending, Order, orders )
{
    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();
    mock_binary_cursor< container_type::const_iterator > mc(ci, cie);
    
    boost::tree::copy(Order(), fbt1.ascending_root(), mc);
}

BOOST_AUTO_TEST_SUITE_END()