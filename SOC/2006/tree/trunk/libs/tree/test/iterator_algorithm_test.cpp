//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/iterator.hpp>
#include <boost/tree/algorithm.hpp>
#include <boost/tree/ascending_cursor.hpp>
#include <boost/tree/root_tracking_cursor.hpp>

#include <list>
#include <iterator>

#define BOOST_TEST_MODULE iterator_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE( iterator_algorithms_test, test_binary_tree_with_list_fixture<int> )

template <class Order, class Cursor>
void compare_cursor_to_iterator_traversal(Order, Cursor cur) {    
    std::list<int> test_list;
    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
    typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
    
    boost::tree::copy(Order(), cur, oc_test_list);
    
    // Are the elements accessed in the correct order?
    BOOST_CHECK(std::equal( boost::tree::begin(Order(), cur),
                            boost::tree::end(Order(), cur),
                            test_list.begin()
                            ));

    // Does end() mark the right element? 
    BOOST_CHECK(std::distance(boost::tree::begin(Order(), cur),
                              boost::tree::end(Order(), cur)) == 
                std::distance(test_list.begin(), test_list.end()));

    // Reverse order.
    BOOST_CHECK(std::equal(    boost::tree::rbegin(Order(), cur),
                            boost::tree::rend(Order(), cur),
                            test_list.rbegin()
                            ));

    BOOST_CHECK(std::distance(boost::tree::rbegin(Order(), cur),
                              boost::tree::rend(Order(), cur)) == 
                std::distance(test_list.rbegin(), test_list.rend()));                    

}

template <class Cursor, class Op>
void underefed_for_each_recursive(Cursor s, Op& f)
{
    Cursor t = s.end();
    f(s);            // Caution: f(s) comes before s.to_begin(), as opposed to
    s.to_begin();    // "normal" preorder for_each
    do
        if (!s.empty())
            underefed_for_each_recursive(s, f);
    while (s++ != t);
}

template <class Cursor, class Op>
Op underefed_for_each(Cursor s, Op f)
{
    Cursor t = s.end();
    f(s);            // Caution: f(s) comes before s.to_begin(), as opposed to
    s.to_begin();    // "normal" preorder for_each
    do
        if (!s.empty())
            underefed_for_each_recursive(s, f);
    while (s++ != t);

    return f;
}

template <class Order>
void comparisons_using_ac(binary_tree<int>::cursor c) {
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(c)));
}

template <class Order>
void comparisons_using_rtc(binary_tree<int>::cursor c) {
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(c));
}

typedef boost::mpl::list<preorder,inorder,postorder> orders;

/** 
 * Check all iterator traversals by comparing them to a recursive cursor
 * algorithm output. Do that at different stages of the tree while adding
 * elements to it, so different tree shapes are checked to be catered for
 * by the iterator algorithms.
 * 
 * Afterwards, do all that using iterators wrapped around
 * "explicit stack"-based cursors also.
 */ 
//void compare_cursor_to_iterator_traversal() {
BOOST_AUTO_TEST_CASE_TEMPLATE( compare_cursor_to_iterator_traversal_test, Order, orders )
{
    binary_tree<int> test_tree2;
    //test::compare_cursor_to_iterator_traversal(Order(), test_tree2.root());

    binary_tree<int>::cursor c = test_tree2.insert(test_tree2.root(), 8);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));

    c = test_tree2.insert(c, 3);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
        
    test_tree2.insert(c, 1);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(++c, 6);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    test_tree2.insert(c, 4);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    test_tree2.insert(++c, 7);    
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(test_tree2.root().end(), 10);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(test_tree2.root().end().end(), 14);    
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(c, 13);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(c, 11);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(++c, 12);
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(test_tree2.root()));
    compare_cursor_to_iterator_traversal(Order(), make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    underefed_for_each(test_tree2.root(), comparisons_using_ac<Order>);
    underefed_for_each(test_tree2.root(), comparisons_using_rtc<Order>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_algorithms, Order, orders )
{
    typedef boost::tree::binary_tree<int>::cursor cursor;
   
//    std::list<int> test_list;
//    
//    // TODO: Put this into a better testing context.
//    boost::tree::preorder::for_each(
//        test_tree.root(), 
//        boost::lambda::bind(&std::list<int>::push_back, &test_list, boost::lambda::_1)
//    );
//    BOOST_CHECK(test_list.empty());

    //Preorder    
    test_traversal(Order(), begin(Order(), make_root_tracking_cursor(bt.root())),
                              end(Order(), make_root_tracking_cursor(bt.root())));

    test_reverse_traversal(Order(), end(Order(), make_root_tracking_cursor(bt.root())),
                                      begin(Order(), make_root_tracking_cursor(bt.root())));
                                    
    BOOST_CHECK(std::distance(begin(Order(), make_root_tracking_cursor(bt.root())), 
                              end(Order(), make_root_tracking_cursor(bt.root()))) == 11);

    // TODO: Also check with binary_tree-specialized inorder begin()!

    // Now the iterators based on stack-based cursors (that don't use cursor.to_parent())

    test_traversal(Order(), begin(Order(), make_root_tracking_cursor(make_ascending_cursor(bt.root()))), 
                                 end(Order(), make_root_tracking_cursor(make_ascending_cursor(bt.root()))));
    test_reverse_traversal(Order(), end(Order(), make_root_tracking_cursor(make_ascending_cursor(bt.root()))), 
                                         begin(Order(), make_root_tracking_cursor(make_ascending_cursor(bt.root()))));

// TODO: Move to other unit
    //Ascending iterator.
//    binary_tree<int>::cursor c = test_tree.root();
//    boost::tree::iterator<ascending, binary_tree<int>::cursor> ai_root(c);
//    c = c.begin().end().begin().begin();
//    BOOST_CHECK_EQUAL(*c, 4);
//
//    boost::tree::iterator<ascending, binary_tree<int>::cursor> ais(c);
//    test_traversal_from_leaf4(ais, ai_root);

}

BOOST_AUTO_TEST_SUITE_END()