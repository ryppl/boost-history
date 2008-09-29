//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//TODO: Make this a test suite.
//        Add iterator traversal tests - check if proper overloads (if present)
//        are used.

// TODO: get timings. that makes that no testcase anymore, right?
//does boost have timers? what does the austern et al one look like?

#include <boost/tree/binary_tree.hpp>

#include <boost/tree/iterator.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/tree/ascending_cursor.hpp>
#include <boost/tree/root_tracking_cursor.hpp>

#include <boost/test/minimal.hpp>

#include <list>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

// Some macro magic, to save us from all too tedious redundant calls
// to each of the three types of order algorithms and checks.

#include "subtree_algorithms_checks.hpp"

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

template <class Cursor>
void comparisons(Cursor c) {
    test::compare_cursor_to_iterator_traversal(preorder(), c);
    test::compare_cursor_to_iterator_traversal(inorder(), c);
    test::compare_cursor_to_iterator_traversal(postorder(), c);
    return;
}

void comparisons_using_ac(binary_tree<int>::cursor c) {
    comparisons(make_root_tracking_cursor(make_ascending_cursor(c)));
    return;
}

void comparisons_using_rtc(binary_tree<int>::cursor c) {
    comparisons(make_root_tracking_cursor(c));
    return;
}

/** 
 * Check all iterator traversals by comparing them to a recursive cursor
 * algorithm output. Do that at different stages of the tree while adding
 * elements to it, so different tree shapes are checked to be catered for
 * by the iterator algorithms.
 * 
 * Afterwards, do all that using iterators wrapped around
 * "explicit stack"-based cursors also.
 */ 
void compare_cursor_to_iterator_traversal() {
    binary_tree<int> test_tree2;
    //comparisons(test_tree2.root());

    binary_tree<int>::cursor c = test_tree2.insert(test_tree2.root(), 8);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));

    c = test_tree2.insert(c, 3);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
        
    test_tree2.insert(c, 1);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(++c, 6);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    test_tree2.insert(c, 4);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    test_tree2.insert(++c, 7);    
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(test_tree2.root().end(), 10);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(test_tree2.root().end().end(), 14);    
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(c, 13);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(c, 11);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    c = test_tree2.insert(++c, 12);
    comparisons(make_root_tracking_cursor(test_tree2.root()));
    comparisons(make_root_tracking_cursor(make_ascending_cursor(test_tree2.root())));
    
    underefed_for_each(test_tree2.root(), comparisons_using_ac);
    underefed_for_each(test_tree2.root(), comparisons_using_rtc);
}

int test_main(int, char* [])
{
    typedef boost::tree::binary_tree<int>::cursor cursor;
    
    binary_tree<int> test_tree;    
//    std::list<int> test_list;
//    
//    // TODO: Put this into a better testing context.
//    boost::tree::preorder::for_each(
//        test_tree.root(), 
//        boost::lambda::bind(&std::list<int>::push_back, &test_list, boost::lambda::_1)
//    );
//    BOOST_CHECK(test_list.empty());

    create_test_data_tree(test_tree);

    //Preorder    
    test_traversal(preorder(), begin(preorder(), make_root_tracking_cursor(test_tree.root())),
                              end(preorder(), make_root_tracking_cursor(test_tree.root())));

    test_reverse_traversal(preorder(), end(preorder(), make_root_tracking_cursor(test_tree.root())),
                                      begin(preorder(), make_root_tracking_cursor(test_tree.root())));
                                    
    BOOST_CHECK(std::distance(begin(preorder(), make_root_tracking_cursor(test_tree.root())), 
                              end(preorder(), make_root_tracking_cursor(test_tree.root()))) == 11);

    // Inorder
    test_traversal(inorder(), begin(inorder(), make_root_tracking_cursor(test_tree.root())), 
                           end(inorder(), make_root_tracking_cursor(test_tree.root())));

    test_reverse_traversal(inorder(), end(inorder(), make_root_tracking_cursor(test_tree.root())), 
                                   begin(inorder(), make_root_tracking_cursor(test_tree.root())));
    
    // TODO: Also check with binary_tree-specialized inorder begin()!

    BOOST_CHECK(std::distance(begin(inorder(), make_root_tracking_cursor(test_tree.root())), 
                              end(inorder(), make_root_tracking_cursor(test_tree.root()))) == 11);

    //Postorder
    test_traversal(postorder(), begin(postorder(), make_root_tracking_cursor(test_tree.root())), 
                             end(postorder(), make_root_tracking_cursor(test_tree.root())));
    test_reverse_traversal(postorder(), end(postorder(), make_root_tracking_cursor(test_tree.root())), 
                                     begin(postorder(), make_root_tracking_cursor(test_tree.root())));

    BOOST_CHECK(std::distance(begin(postorder(), make_root_tracking_cursor(test_tree.root())), 
                              end(postorder(), make_root_tracking_cursor(test_tree.root()))) == 11);

    // Now the iterators based on stack-based cursors (that don't use cursor.to_parent())

    test_traversal(preorder(), begin(preorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))), 
                                 end(preorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))));
    test_reverse_traversal(preorder(), end(preorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))), 
                                         begin(preorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))));    
    
    test_traversal(inorder(), begin(inorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))), 
                             end(inorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))));
    test_reverse_traversal(inorder(), end(inorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))), 
                                     begin(inorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))));

    test_traversal(postorder(), begin(postorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))), 
                                  end(postorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))));
    test_reverse_traversal(postorder(), end(postorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))), 
                                          begin(postorder(), make_root_tracking_cursor(make_ascending_cursor(test_tree.root()))));

    //Ascending iterator.
    binary_tree<int>::cursor c = test_tree.root();
    boost::tree::iterator<ascending, binary_tree<int>::cursor> ai_root(c);
    c = c.begin().end().begin().begin();
    BOOST_CHECK(*c == 4);

    boost::tree::iterator<ascending, binary_tree<int>::cursor> ais(c);
    test_traversal_from_leaf4(ais, ai_root);

    //Now the advanced stuff
    compare_cursor_to_iterator_traversal();
        
    return 0;
}


