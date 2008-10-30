//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/forest_tree.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/lambda/bind.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/pair.hpp>

#include <list>

#define BOOST_TEST_MODULE forest_tree test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

typedef boost::mpl::list< boost::mpl::pair<preorder, preorder>
                        /*, boost::mpl::pair<postorder, inorder>*/ > order_pairs;

BOOST_FIXTURE_TEST_SUITE(forest_algorithms_test, test_binary_tree_with_list_fixture<int>)

BOOST_AUTO_TEST_CASE( forest_tree_test )
{
    using namespace boost::tree;
    
    typedef forest_tree<int> tree_type;
    tree_type mytree;
    
    tree_type::cursor c = mytree.root();
    c = mytree.insert(c, 6);    
    BOOST_CHECK_EQUAL(*c, 6);

    c = mytree.insert(c, 5);    
    BOOST_CHECK_EQUAL(*c, 5);

    c = mytree.insert(c, 4);    
    BOOST_CHECK_EQUAL(*c, 4);
    BOOST_CHECK(c == mytree.root().begin());

    ++c;
    BOOST_CHECK_EQUAL(*c, 5);
    ++c;
    BOOST_CHECK_EQUAL(*c, 6);
    
    tree_type forest;
    //create_test_data_tree(forest);
    c = forest.insert(forest.root(), 8);
    BOOST_CHECK(c == forest.root().begin());
    BOOST_CHECK_EQUAL(*c, 8);
    c = forest.insert(c, 3);
    BOOST_CHECK_EQUAL(*c, 3);
    BOOST_CHECK_EQUAL(*++c, 8);
//    BOOST_CHECK_EQUAL(*forest.root().begin().begin(), 3);

}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_natural_correspondence, Order, order_pairs )
{
    using namespace boost::tree;

    typedef forest_tree<int> forest_tree_type;
    forest_tree_type ft(bt);
    
    validate_corresponding_forest_tree(ft);
    
    // Now test *order correspondence:
    // forest    binary
    // pre        pre
    // post        in
    std::list<int> test_list;
    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
    typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
    
    boost::tree::for_each(
        typename Order::first(),
        ft.root(), 
        boost::lambda::bind(&std::list<int>::push_back, &test_list, boost::lambda::_1)
    );
    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
    BOOST_CHECK_EQUAL(test_list.size(), 11);
    test_list.clear();
    
    boost::tree::copy(typename Order::first(), ft.root(), oc_test_list);
    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
    BOOST_CHECK_EQUAL(test_list.size(), 11);
    test_list.clear();
    
    boost::tree::transform(typename Order::first(), ft.root(), oc_test_list, std::bind2nd(std::plus<int>(),0));
    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
    BOOST_CHECK_EQUAL(test_list.size(), 11);
    test_list.clear();

    //test::preorder::algorithms(ft.root(), ft.root()); // FIXME: Fix algorithms for use in here.
    
//    boost::tree::copy(typename Order::first(), ft.root(), oc_test_list);
//    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
//    BOOST_CHECK_EQUAL(test_list.size(), 11);
}


BOOST_AUTO_TEST_SUITE_END()