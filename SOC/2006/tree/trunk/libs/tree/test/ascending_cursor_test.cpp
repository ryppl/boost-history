//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>

#include <boost/tree/ascending_cursor.hpp>

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE( ascending_cursor_test, fake_binary_tree_fixture<int> )

BOOST_AUTO_TEST_CASE( test_ascending_cursor )
{
    typedef fake_binary_tree<int>::descending_cursor dc_t;
    ascending_cursor<dc_t> ac = make_ascending_cursor(fbt1.descending_root());

    ac.to_begin().to_end().to_begin().to_begin();

// FIXME
//    BOOST_CHECK_EQUAL(*ac, 4);
    ac.to_parent();
//    BOOST_CHECK_EQUAL(*ac, 6);
//    ac.to_parent();
//    BOOST_CHECK_EQUAL(*ac, 3);
//    ac.to_parent();
//    BOOST_CHECK_EQUAL(*ac, 8);

}

BOOST_AUTO_TEST_SUITE_END()