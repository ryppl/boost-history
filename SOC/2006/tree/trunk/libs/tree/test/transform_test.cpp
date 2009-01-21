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

// TODO: Actually transform back and forth; eg, add 1 using STL transform, then subtract it
// again using the subtree algorithm.

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_fixture<int>)

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform_descending, Order, orders)
{
    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    //std::transform(po.begin(), po.end(), po.begin(), std::bind2nd(std::plus<int>(/*second member of pair*/),0))
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();
    mock_binary_cursor< container_type::const_iterator > mc(ci, cie);
    
    boost::tree::transform(Order(), fbt1.descending_root(), mc, std::bind2nd(std::plus<int>(),0));
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_transform_ascending, Order, orders)
{
    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    //std::transform(po.begin(), po.end(), po.begin(), std::bind2nd(std::plus<int>(/*second member of pair*/),0))
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();
    mock_binary_cursor< container_type::const_iterator > mc(ci, cie);
    
    boost::tree::transform(Order(), fbt1.ascending_root(), mc, std::bind2nd(std::plus<int>(),0));
}

BOOST_AUTO_TEST_SUITE_END()