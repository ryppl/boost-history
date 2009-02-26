//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/forest.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/lambda/bind.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/pair.hpp>

#include <list>

#define BOOST_TEST_MODULE forest test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_tree_traversal_data.hpp"
#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_AUTO_TEST_SUITE( basic_forest_test )

BOOST_AUTO_TEST_CASE( constructors_test )
{
    forest<int> ft0;
    //BOOST_CHECK_EQUAL(*ft0.root(), 0);
    BOOST_CHECK(ft0.empty());
    BOOST_CHECK(ft0.begin() == ft0.end());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(forest_fixture_test, fake_binary_tree_fixture<int>)

BOOST_AUTO_TEST_CASE( binary_tree_constructor_test )
{
    forest<int, fake_binary_tree<int> > ft0(fbt1);
    validate_test_dataset1_forest(ft0.begin(), ft0.end());
}

BOOST_AUTO_TEST_CASE( insert_test )
{
    forest<int, fake_binary_tree<int> > ft0;
    create_test_dataset1_forest(ft0);
    validate_test_dataset1_forest(ft0.begin(), ft0.end());
}

BOOST_AUTO_TEST_SUITE_END()
