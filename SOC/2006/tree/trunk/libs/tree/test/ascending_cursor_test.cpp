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

BOOST_FIXTURE_TEST_SUITE( ascending_cursor_test, fake_binary_tree_with_list_fixture<int> )

BOOST_AUTO_TEST_CASE( test_ascending_cursor )
{
// FIXME: Test differently, ie not relying on cursor-iterator adaptation.
//    test_traversal(Order(), begin(Order(), make_ascending_cursor(fbt1.descending_root()))
//                          , end(Order(), make_ascending_cursor(fbt1.descending_root())));
//    test_reverse_traversal(Order(), end(Order(), make_ascending_cursor(fbt1.descending_root()))
//                                  , begin(Order(), make_ascending_cursor(fbt1.descending_root())));
//    BOOST_CHECK_EQUAL(std::distance(
//                        begin(Order(), make_ascending_cursor(fbt1.descending_root())) 
//                      , end(Order(), make_ascending_cursor(fbt1.descending_root()))), 11);
}

BOOST_AUTO_TEST_SUITE_END()