//  Copyright (c) 2006-2009, Bernhard Reiter
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

BOOST_AUTO_TEST_SUITE( basic_forest_test )

BOOST_AUTO_TEST_CASE( constructors_test )
{
    forest_tree<int> ft0;
    //BOOST_CHECK_EQUAL(*ft0.root(), 0);
    BOOST_CHECK(ft0.empty());
}

//BOOST_AUTO_TEST_CASE( insert_value_test )
//{
//    using namespace boost::tree;
//
////    forest_tree<int> mytree;
////    
////    forest_tree<int>::cursor c = mytree.root();
////    c = mytree.insert(c, 6);
////    BOOST_CHECK_EQUAL(*c, 6);
////
////    c = mytree.insert(c, 5);    
////    BOOST_CHECK_EQUAL(*c, 5);
////
////    c = mytree.insert(c, 4);    
////    BOOST_CHECK_EQUAL(*c, 4);
////    BOOST_CHECK(c == mytree.root().begin());
////
////    ++c;
////    BOOST_CHECK_EQUAL(*c, 5);
////    ++c;
////    BOOST_CHECK_EQUAL(*c, 6);
//    
//    forest_tree<int> ft0;
//
//    forest_tree<int>::cursor c = ft0.insert(ft0.root().end(), 8);
//    
//    BOOST_CHECK_EQUAL(*c, 8);
//    BOOST_CHECK(c == ft0.root().begin());
//    BOOST_CHECK(++c == ft0.root().end());
//    BOOST_CHECK(ft0.root().begin().parent() == ft0.root());
//    BOOST_CHECK(!ft0.root().empty());
//    BOOST_CHECK(ft0.root().begin().empty());
//    
//    c = ft0.insert(ft0.root().end(), 6);
//    BOOST_CHECK_EQUAL(*c, 6);
//    BOOST_CHECK(ft0.root().begin() != ft0.root().end());
//    BOOST_CHECK(c != ft0.root().end());
//    BOOST_CHECK(c.base() == ft0.root().base().begin().end());
//    BOOST_CHECK(c.parent() == ft0.root());
//    BOOST_CHECK(!ft0.root().empty());
//    BOOST_CHECK(++c == ft0.root().end()); 
//    ----c;
//    BOOST_CHECK(c == ft0.root().begin());
//    BOOST_CHECK_EQUAL(*c, 8);
//
//    c = ft0.insert(ft0.root().end(), 7);  
//    BOOST_CHECK_EQUAL(*c, 7);
//    BOOST_CHECK(c.parent() == ft0.root());
//    BOOST_CHECK(!ft0.root().empty());
//    BOOST_CHECK(++c == ft0.root().end());
//    ----c;
//    BOOST_CHECK_EQUAL(*c, 6);
//    BOOST_CHECK(c.parent() == ft0.root());
//    --c;
//    BOOST_CHECK(c == ft0.root().begin());
//    BOOST_CHECK(c.parent() == ft0.root());
//    BOOST_CHECK_EQUAL(*c, 8);
//    c = ft0.root().begin().begin();
//    BOOST_CHECK(c.parent() == ft0.root().begin());
//
//    c = ft0.insert(ft0.root().begin().begin(), 3);
//    BOOST_CHECK_EQUAL(*c, 3);
//    BOOST_CHECK(c == ft0.root().begin().begin());
//    BOOST_CHECK(ft0.root().begin().begin().parent() == ft0.root().begin());
//
//    // Need more checks after this line...
//    c = ft0.insert(ft0.root().begin().begin().begin(), 1);
//    c = ft0.root().begin();
//    (++c).to_end();
//
//    c = ft0.insert(c, 4);
//    BOOST_CHECK_EQUAL(*c, 4);
//    BOOST_CHECK(--(c.to_parent()) == ft0.root().begin());
//
//    c = ft0.root().begin();
//    BOOST_CHECK_EQUAL(*c, 8);
//    BOOST_CHECK_EQUAL(*c.to_begin(), 3);
//    BOOST_CHECK_EQUAL(*c.to_begin(), 1);
//    BOOST_CHECK(c.empty());
//    
//    //validate_corresponding_forest_tree(ft0);
//}

BOOST_AUTO_TEST_SUITE_END()

//BOOST_FIXTURE_TEST_SUITE(forest_algorithms_test, test_binary_tree_with_list_fixture<int>)
//
//// Test *order correspondence:
//// forest   binary
//// pre      pre
//// post     in
//typedef boost::mpl::list< boost::mpl::pair<preorder, preorder>
//                        , boost::mpl::pair<postorder, inorder> > corresponding_orders;
//
//BOOST_AUTO_TEST_CASE_TEMPLATE( test_natural_correspondence_for_each, Order
//                             , corresponding_orders )
//{
//    using namespace boost::tree;
//
//    forest_tree<int> ft(bt);
//    
//    //validate_corresponding_forest_tree(ft);
//    
//    std::list<int> test_list;
//    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
//    typedef output_iterator_cursor<back_insert_iter_list_int> oc_bi_lst_type;
//    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
//    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
//    
//    boost::tree::for_each(
//        typename Order::first()
//      , ft
//      , boost::lambda::bind(&std::list<int>::push_back, &test_list, boost::lambda::_1)
//    );
//    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
//    BOOST_CHECK_EQUAL(test_list.size(), 11);
//    
//}

//BOOST_AUTO_TEST_CASE_TEMPLATE( test_natural_correspondence_copy, Order
//                             , corresponding_orders )
//{
//    using namespace boost::tree;
//
//    forest_tree<int> ft(bt);
//    
//    //validate_corresponding_forest_tree(ft);
//    
//    std::list<int> test_list;
//    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
//    typedef output_iterator_cursor<back_insert_iter_list_int> oc_bi_lst_type;
//    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
//    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
//    
//    boost::tree::copy(typename Order::first(), ft.root(), oc_test_list);
//    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
//    BOOST_CHECK_EQUAL(test_list.size(), 11);
//    test_list.clear();
//}

//BOOST_AUTO_TEST_CASE_TEMPLATE( test_natural_correspondence_transform, Order
//                             , corresponding_orders )
//{
//    using namespace boost::tree;
//
//    forest_tree<int> ft(bt);
//    
//    //validate_corresponding_forest_tree(ft);
//    
//    std::list<int> test_list;
//    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
//    typedef output_iterator_cursor<back_insert_iter_list_int> oc_bi_lst_type;
//    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
//    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
//    boost::tree::transform(typename Order::first(), ft.root(), oc_test_list
//                         , std::bind2nd(std::plus<int>(),0));
//    test_traversal(typename Order::second(), test_list.begin(), test_list.end());
//    BOOST_CHECK_EQUAL(test_list.size(), 11);
//}
//
//BOOST_AUTO_TEST_SUITE_END()