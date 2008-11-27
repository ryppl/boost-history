//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_TREE_TEST_TEST_TREE_TRAVERSAL_HPP
#define LIBS_TREE_TEST_TEST_TREE_TRAVERSAL_HPP

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/algorithm.hpp>
#include <boost/tree/cursor.hpp>

#include <list>

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
    
    static void validate_test_dataset1_tree(boost::tree::binary_tree<T>& ret)
    {
        BOOST_CHECK_EQUAL(*ret.root().begin(), 8);
        BOOST_CHECK_EQUAL(*ret.root().begin().begin(), 3);    
        BOOST_CHECK_EQUAL(*ret.root().begin().begin().begin(), 1);  //Leaf
        BOOST_CHECK_EQUAL(*ret.root().begin().end().begin(), 6);        
        BOOST_CHECK_EQUAL(*ret.root().begin().end().begin().begin(), 4); //Leaf
        BOOST_CHECK_EQUAL(*ret.root().begin().end().end().begin(), 7); //Leaf
    
        BOOST_CHECK_EQUAL(*ret.root().end().begin(), 10);
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin(), 14);
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin().begin(), 13);
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin().begin().begin(), 11); 
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin().begin().end().begin(), 12); //Leaf
    }

    static void validate_test_dataset1_minus_1_tree(boost::tree::binary_tree<T>& ret)
    {
        BOOST_CHECK_EQUAL(*ret.root().begin(), 7);
        BOOST_CHECK_EQUAL(*ret.root().begin().begin(), 2);    
        BOOST_CHECK_EQUAL(*ret.root().begin().begin().begin(), 0);  //Leaf
        BOOST_CHECK_EQUAL(*ret.root().begin().end().begin(), 5);        
        BOOST_CHECK_EQUAL(*ret.root().begin().end().begin().begin(), 3); //Leaf
        BOOST_CHECK_EQUAL(*ret.root().begin().end().end().begin(), 6); //Leaf
    
        BOOST_CHECK_EQUAL(*ret.root().end().begin(), 9);
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin(), 13);
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin().begin(), 12);
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin().begin().begin(), 10); 
        BOOST_CHECK_EQUAL(*ret.root().end().end().begin().begin().end().begin(), 11); //Leaf
    }
    
    boost::tree::binary_tree<T> bt, bt2;
};

template <class T = int>
struct test_binary_tree_with_list_fixture
: public test_binary_tree_fixture<T> {
    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
    typedef boost::tree::output_iterator_cursor<back_insert_iter_list_int> oc_bi_lst_type;
    
    test_binary_tree_with_list_fixture()
    : test_binary_tree_fixture<T>(), l(), i(l), o(i) { }
    
    std::list<int> l;
    back_insert_iter_list_int i;
    oc_bi_lst_type o;
};

template <class Tree>
void validate_corresponding_forest_tree(Tree const& t)
{
    typename Tree::const_cursor c = t.root().begin();
    BOOST_CHECK_EQUAL(*c, 8);
    BOOST_CHECK_EQUAL(*c.to_begin(), 3);
    BOOST_CHECK_EQUAL(*c.to_begin(), 1);
    BOOST_CHECK(c.empty());
    BOOST_CHECK(++c == t.root().begin().begin().end());
    --c;
    c.to_parent();
    BOOST_CHECK_EQUAL(*++c, 6);
    BOOST_CHECK_EQUAL(*c.to_begin(), 4);
    c.to_parent();
    BOOST_CHECK_EQUAL(*++c, 7);
    BOOST_CHECK(++c == t.root().begin().end());
    
    c = t.root().begin();
    BOOST_CHECK_EQUAL(*++c, 10);
    BOOST_CHECK_EQUAL(*++c, 14);
    BOOST_CHECK(++c == t.root().end());
    --c;
    BOOST_CHECK_EQUAL(*c.to_begin(), 13);
    BOOST_CHECK_EQUAL(*c.to_begin(), 11);
    BOOST_CHECK_EQUAL(*++c, 12);
}

template <class Iterator>
void test_traversal(boost::tree::preorder, Iterator a, Iterator b) 
{
    BOOST_CHECK_EQUAL(*a++, 8);
    BOOST_CHECK_EQUAL(*a++, 3);
    BOOST_CHECK_EQUAL(*a++, 1);
    BOOST_CHECK_EQUAL(*a++, 6);
    BOOST_CHECK_EQUAL(*a++, 4);
    BOOST_CHECK_EQUAL(*a++, 7);
    BOOST_CHECK_EQUAL(*a++, 10);
    BOOST_CHECK_EQUAL(*a++, 14);
    BOOST_CHECK_EQUAL(*a++, 13);
    BOOST_CHECK_EQUAL(*a++, 11);
    BOOST_CHECK_EQUAL(*a++, 12);
    BOOST_CHECK(a == b);
}

template <class Iterator>
void test_reverse_traversal(boost::tree::preorder, Iterator a, Iterator b)
{     
    BOOST_CHECK_EQUAL(*--a, 12);
    BOOST_CHECK_EQUAL(*--a, 11);
    BOOST_CHECK_EQUAL(*--a, 13);
    BOOST_CHECK_EQUAL(*--a, 14);
    BOOST_CHECK_EQUAL(*--a, 10);
    BOOST_CHECK_EQUAL(*--a, 7);
    BOOST_CHECK_EQUAL(*--a, 4);
    BOOST_CHECK_EQUAL(*--a, 6);
    BOOST_CHECK_EQUAL(*--a, 1);
    BOOST_CHECK_EQUAL(*--a, 3);
    BOOST_CHECK_EQUAL(*--a, 8);
    BOOST_CHECK(a == b);
}

template <class Iterator>
void test_subtree_traversal(boost::tree::preorder, Iterator a, Iterator b) 
{
    BOOST_CHECK_EQUAL(*a++, 3);
    BOOST_CHECK_EQUAL(*a++, 1);
    BOOST_CHECK_EQUAL(*a++, 6);
    BOOST_CHECK_EQUAL(*a++, 4);
    BOOST_CHECK_EQUAL(*a++, 7);
    BOOST_CHECK(a == b);
}

template <class Iterator>
void test_traversal(boost::tree::inorder, Iterator a, Iterator b)
{        
    BOOST_CHECK_EQUAL(*a++, 1);
    BOOST_CHECK_EQUAL(*a++, 3);
    BOOST_CHECK_EQUAL(*a++, 4);
    BOOST_CHECK_EQUAL(*a++, 6);
    BOOST_CHECK_EQUAL(*a++, 7);
    BOOST_CHECK_EQUAL(*a++, 8);
    BOOST_CHECK_EQUAL(*a++, 10);
    BOOST_CHECK_EQUAL(*a++, 11);
    BOOST_CHECK_EQUAL(*a++, 12);
    BOOST_CHECK_EQUAL(*a++, 13);
    BOOST_CHECK_EQUAL(*a++, 14);
    BOOST_CHECK(a == b);
}

template <class Iterator>
void test_reverse_traversal(boost::tree::inorder, Iterator a, Iterator b)
{
    BOOST_CHECK_EQUAL(*--a, 14);
    BOOST_CHECK_EQUAL(*--a, 13);
    BOOST_CHECK_EQUAL(*--a, 12);
    BOOST_CHECK_EQUAL(*--a, 11);
    BOOST_CHECK_EQUAL(*--a, 10);
    BOOST_CHECK_EQUAL(*--a, 8);
    BOOST_CHECK_EQUAL(*--a, 7);
    BOOST_CHECK_EQUAL(*--a, 6);
    BOOST_CHECK_EQUAL(*--a, 4);
    BOOST_CHECK_EQUAL(*--a, 3);
    BOOST_CHECK_EQUAL(*--a, 1);
    BOOST_CHECK(a == b);
}


template <class Iterator>
void test_traversal(boost::tree::postorder, Iterator a, Iterator b)
{    
    BOOST_CHECK_EQUAL(*a++, 1);    
    BOOST_CHECK_EQUAL(*a++, 4);
    BOOST_CHECK_EQUAL(*a++, 7);
    BOOST_CHECK_EQUAL(*a++, 6);
    BOOST_CHECK_EQUAL(*a++, 3);
    BOOST_CHECK_EQUAL(*a++, 12);
    BOOST_CHECK_EQUAL(*a++, 11);
    BOOST_CHECK_EQUAL(*a++, 13);
    BOOST_CHECK_EQUAL(*a++, 14);
    BOOST_CHECK_EQUAL(*a++, 10);
    BOOST_CHECK_EQUAL(*a++, 8);
    BOOST_CHECK(a == b);
}

template <class Iterator>
void test_reverse_traversal(boost::tree::postorder, Iterator a, Iterator b)
{    
    BOOST_CHECK_EQUAL(*--a, 8);
    BOOST_CHECK_EQUAL(*--a, 10);
    BOOST_CHECK_EQUAL(*--a, 14);
    BOOST_CHECK_EQUAL(*--a, 13);
    BOOST_CHECK_EQUAL(*--a, 11);
    BOOST_CHECK_EQUAL(*--a, 12);
    BOOST_CHECK_EQUAL(*--a, 3);
    BOOST_CHECK_EQUAL(*--a, 6);
    BOOST_CHECK_EQUAL(*--a, 7);
    BOOST_CHECK_EQUAL(*--a, 4);
    BOOST_CHECK_EQUAL(*--a, 1);
    BOOST_CHECK(a == b);
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
