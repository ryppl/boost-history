//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_TREE_TEST_TEST_TREE_TRAVERSAL_HPP
#define LIBS_TREE_TEST_TEST_TREE_TRAVERSAL_HPP

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/mpl/list.hpp>

typedef boost::mpl::list<boost::tree::preorder
                        ,boost::tree::inorder
                        ,boost::tree::postorder> orders;

template <class T = int>
struct test_binary_tree_fixture {
    test_binary_tree_fixture()
    {
        create_test_dataset1_tree(bt);
        create_test_dataset1_tree(bt2);
        
        typename boost::tree::binary_tree<T>::cursor d = bt2.root();

        // Just to make sure we won't be getting any false positives when 
        // copying test_tree1 to test_tree2, we'll change one of test_tree2's
        // values.
        d = d.begin().end().begin().begin();
        *d = T(29);
    }
    
    // Test data from http://en.wikipedia.org/wiki/Image:Binary_search_tree.svg
    // (With two additional nodes: 11 inserted left of 13; 12 right of 11)
    // and in combination with http://en.wikipedia.org/wiki/Tree_traversal#Examples
    // (as tree shapes are equal [apart from the extra nodes])
    static void create_test_dataset1_tree(boost::tree::binary_tree<T>& ret)
    {
        // For augmented trees. (Why is this necessary? Nothing here is explicit!)
        typedef typename boost::tree::binary_tree<T>::value_type value_type; 
        
        typename boost::tree::binary_tree<T>::cursor cur = ret.insert(ret.root(), value_type(8));
        cur = ret.insert(cur.to_begin(), value_type(3));
        ret.insert(cur.to_begin(), value_type(1));
        cur = ret.insert(++cur, value_type(6));
        ret.insert(cur.to_begin(), value_type(4));
        ret.insert(++cur, value_type(7));
        cur = ret.insert(ret.root().end(), value_type(10));
        cur = ret.insert(ret.root().end().end(), value_type(14));
        cur = ret.insert(cur.to_begin(), value_type(13));
        cur = ret.insert(cur.to_begin(), value_type(11));
        cur = ret.insert(++cur.to_begin(), value_type(12));
    }
    
    template <class Cursor>
    static void validate_test_dataset1_tree(Cursor cur)
    {
        BOOST_CHECK_EQUAL(*cur.begin(), 8);
        BOOST_CHECK_EQUAL(*cur.begin().begin(), 3);
        BOOST_CHECK_EQUAL(*cur.begin().begin().begin(), 1);
        BOOST_CHECK(cur.begin().begin().end().is_leaf());
        BOOST_CHECK(cur.begin().begin().begin().is_leaf()); //Leaf
        
        BOOST_CHECK_EQUAL(*cur.begin().end().begin(), 6);
        BOOST_CHECK_EQUAL(*cur.begin().end().begin().begin(), 4);
        BOOST_CHECK(cur.begin().end().begin().begin().is_leaf()); //Leaf

        BOOST_CHECK_EQUAL(*cur.begin().end().end().begin(), 7);
        BOOST_CHECK(cur.begin().end().end().begin().is_leaf()); //Leaf

        BOOST_CHECK_EQUAL(*cur.end().begin(), 10);
        BOOST_CHECK(cur.end().begin().is_leaf());
        BOOST_CHECK_EQUAL(*cur.end().end().begin(), 14);
        BOOST_CHECK(cur.end().end().end().is_leaf());
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin(), 13);
        BOOST_CHECK(cur.end().end().begin().end().is_leaf());
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin().begin(), 11);
        BOOST_CHECK(cur.end().end().begin().begin().begin().is_leaf()); 
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin().end().begin(), 12);
        BOOST_CHECK(cur.end().end().begin().begin().end().begin().is_leaf()); //Leaf
    }

    static void validate_test_dataset1_minus_1_tree(typename boost::tree::binary_tree<T>::const_cursor cur)
    {
        BOOST_CHECK_EQUAL(*cur.begin(), 7);
        BOOST_CHECK_EQUAL(*cur.begin().begin(), 2);    
        BOOST_CHECK_EQUAL(*cur.begin().begin().begin(), 0);  //Leaf
        BOOST_CHECK_EQUAL(*cur.begin().end().begin(), 5);        
        BOOST_CHECK_EQUAL(*cur.begin().end().begin().begin(), 3); //Leaf
        BOOST_CHECK_EQUAL(*cur.begin().end().end().begin(), 6); //Leaf
    
        BOOST_CHECK_EQUAL(*cur.end().begin(), 9);
        BOOST_CHECK_EQUAL(*cur.end().end().begin(), 13);
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin(), 12);
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin().begin(), 10); 
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin().end().begin(), 11); //Leaf
    }
    
    boost::tree::binary_tree<T> bt, bt2;
};

template <class Container>
void generate_mock_cursor_data(boost::tree::preorder, Container& data)
{
    using std::make_pair;
    data[0] = make_pair(0, 8);
    data[1] = make_pair(1, 3);
    data[2] = make_pair(3, 1);
    data[3] = make_pair(4, 6);
    data[4] = make_pair(9, 4);
    data[5] = make_pair(10, 7);
    data[6] = make_pair(2, 10);
    data[7] = make_pair(6, 14);
    data[8] = make_pair(13, 13);
    data[9] = make_pair(27, 11);
    data[10] = make_pair(56, 12);
}

template <class Container>
void generate_mock_cursor_data(boost::tree::inorder, Container& data)
{
    using std::make_pair;
    data[0] = make_pair(3, 1);
    data[1] = make_pair(1, 3);
    data[2] = make_pair(9, 4); 
    data[3] = make_pair(4, 6);
    data[4] = make_pair(10, 7);
    data[5] = make_pair(0, 8);
    data[6] = make_pair(2, 10);
    data[7] = make_pair(27, 11);
    data[8] = make_pair(56, 12);
    data[9] = make_pair(13, 13);
    data[10] = make_pair(6, 14);
}

template <class Container>
void generate_mock_cursor_data(boost::tree::postorder, Container& data)
{
    using std::make_pair;
    data[0] = make_pair(3, 1);
    data[1] = make_pair(9, 4);
    data[2] = make_pair(10, 7);
    data[3] = make_pair(4, 6);
    data[4] = make_pair(1, 3);
    data[5] = make_pair(56, 12);
    data[6] = make_pair(27, 11);
    data[7] = make_pair(13, 13);
    data[8] = make_pair(6, 14);
    data[9] = make_pair(2, 10);
    data[10] = make_pair(0, 8);
}

template <class Iterator>
void test_traversal_from_leaf4(Iterator a, Iterator b)
{    
    BOOST_CHECK_EQUAL(*a, 4);
    BOOST_CHECK_EQUAL(*++a, 6);
    BOOST_CHECK_EQUAL(*++a, 3);
    BOOST_CHECK_EQUAL(*++a, 8);
    BOOST_CHECK(++a == b);

} // namespace ascending


#endif // LIBS_TREE_TEST_TEST_TREE_TRAVERSAL_HPP
