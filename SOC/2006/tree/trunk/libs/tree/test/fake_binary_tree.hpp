//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_TREE_TEST_FAKE_BINARY_TREE_HPP
#define LIBS_TREE_TEST_FAKE_BINARY_TREE_HPP

#include <boost/tree/cursor_adaptor.hpp>

#include <vector>

#include "helpers.hpp"

template <class T>
struct fake_binary_tree_cursor;

/// See http://en.wikipedia.org/wiki/Binary_Tree#Methods_for_storing_binary_trees
template <class T>
struct fake_binary_tree {
    typedef fake_binary_tree_cursor<T> cursor;
    typedef fake_binary_tree_cursor<T const> const_cursor;
        
    fake_binary_tree(typename std::vector<T>::size_type s) : m_data(s)
    { }
    
    cursor root()
    {
        return cursor(*this, 0);
    }

    typedef std::vector<T> children_type;
    typedef typename children_type::size_type size_type;
    typedef typename children_type::value_type value_type;
    typedef typename children_type::difference_type difference_type;
    typedef typename children_type::pointer pointer;
    typedef typename children_type::reference reference;
    
    children_type m_data;  
};

template <class T>
bool operator==(fake_binary_tree<T> const& x, fake_binary_tree<T> const& y)
{
    return (x.m_data == y.m_data);
}

template <class T>
bool operator!=(fake_binary_tree<T> const& x, fake_binary_tree<T> const& y)
{
    return !(x == y);
}

// Make this use cursor_adaptor. Yes, even if that means relying on it.
template <class T>
struct fake_binary_tree_cursor {
    typedef boost::forward_traversal_tag vertical_traversal;
    typedef boost::bidirectional_traversal_tag horizontal_traversal;
    typedef horizontal_traversal iterator_category;
    typedef typename fake_binary_tree<T>::value_type value_type;
    typedef typename fake_binary_tree<T>::difference_type difference_type;
    typedef typename fake_binary_tree<T>::pointer pointer;
    typedef typename fake_binary_tree<T>::reference reference;

    typedef typename fake_binary_tree<T>::size_type size_type;
    
    typedef fake_binary_tree_cursor<T> cursor;
    typedef fake_binary_tree_cursor<T const> const_cursor;
    
    
    //typedef typename fake_binary_tree<T>::children_type::value_type cur; 
    //cur c;
    fake_binary_tree<T>& m_tree;
    typename fake_binary_tree<T>::size_type m_pos;
    
    explicit fake_binary_tree_cursor(fake_binary_tree<T>& t, size_type p = 0) : m_tree(t), m_pos(p) {}
    //fake_binary_tree_cursor(fake_binary_tree_cursor const& mtc) : c(mtc.c), bgn(false) {}
    
    T const& operator*() const
    {
        return m_tree.m_data[m_pos];
    }

    T& operator*()
    {
        return m_tree.m_data[(m_pos-1)/2];
    }
    
    fake_binary_tree_cursor& to_begin()
    {
        m_pos <<= 1;
        ++m_pos;
        return *this;
    }
    
    fake_binary_tree_cursor begin()
    {
        fake_binary_tree_cursor tmp(*this);
        tmp.to_begin();
        return tmp;
    }

    fake_binary_tree_cursor& to_end()
    {
        ++m_pos;
        m_pos <<= 1;
        return *this;
    }

    fake_binary_tree_cursor end()
    {
        fake_binary_tree_cursor tmp(*this);
        tmp.to_end();
        return tmp;
    }
    
    fake_binary_tree_cursor& operator++()
    {
        ++m_pos;
        return *this;
    }
    
    fake_binary_tree_cursor& operator--()
    {
        --m_pos;
        return *this;
    }
    
    bool empty() const
    {
        if (m_pos >= m_tree.m_data.size())
            return true;
        if (m_pos == 0)
            return false;
        return (m_tree.m_data[m_pos] == 0);
    }
    
    size_type index() const
    {
        return (m_pos + 1) % 2;
    }
};

template <class T>
bool operator==(fake_binary_tree_cursor<T> const& x, fake_binary_tree_cursor<T> const& y)
{
    return (x.m_tree == y.m_tree) && (x.m_pos == y.m_pos);
}

template <class T>
bool operator!=(fake_binary_tree_cursor<T> const& x, fake_binary_tree_cursor<T> const& y)
{
    return !(x==y);
}

template <class T>
struct fake_ascending_binary_tree_cursor;

template <class T>
struct fake_ascending_binary_tree_cursor
: public boost::tree::cursor_adaptor<fake_ascending_binary_tree_cursor<T>
                                   , fake_binary_tree_cursor<T> >
{
    typedef fake_ascending_binary_tree_cursor<T> cursor;
    typedef fake_ascending_binary_tree_cursor<T const> const_cursor;

    fake_ascending_binary_tree_cursor(fake_binary_tree<T>& t
                                    , typename fake_binary_tree<T>::size_type p)
    : fake_ascending_binary_tree_cursor::cursor_adaptor_(fake_binary_tree_cursor<T>(t, p)) {}

private: 
    friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;
    
    void up()
    {
        --this->base_reference().m_pos;
        this->base_reference().m_pos >>= 1;
    }
};

//template <class T>
//struct fake_ascending_binary_tree_cursor
//: public fake_binary_tree_cursor<T> {
//    fake_ascending_binary_tree_cursor(fake_binary_tree<T>& t
//                                    , typename fake_binary_tree<T>::size_type p)
//    : fake_binary_tree_cursor<T>(t, p) {}
//    
//    fake_ascending_binary_tree_cursor& to_parent()
//    {
//        --fake_binary_tree_cursor<T>::m_pos;
//        fake_binary_tree_cursor<T>::m_pos >>= 1;
//        return *this;
//    }
//    
//    fake_ascending_binary_tree_cursor parent()
//    {
//        fake_ascending_binary_tree_cursor tmp(*this);
//        tmp.to_parent();
//        return tmp;
//    }
//};

fake_binary_tree<int> generate_fake_binary_tree()
{
    fake_binary_tree<int> mt(57);

    mt.m_data[0] = 8;
    mt.m_data[1] = 3;
    mt.m_data[2] = 10;
    mt.m_data[3] = 1;
    mt.m_data[4] = 6;
    mt.m_data[6] = 14;
    mt.m_data[9] = 4;
    mt.m_data[10] = 7;
    mt.m_data[13] = 13;
    mt.m_data[27] = 11;
    mt.m_data[56] = 12;

    return mt;
}

template <class T = int>
struct fake_binary_tree_fixture {
    fake_binary_tree_fixture() : mbt1(generate_fake_binary_tree()), mbt2(57) {}
    
    template <class Cursor>
    static void validate_test_dataset1_tree(Cursor cur)
    {
        BOOST_CHECK_EQUAL(*cur.begin(), 8);
        BOOST_CHECK_EQUAL(*cur.begin().begin(), 3);
        BOOST_CHECK_EQUAL(*cur.begin().begin().begin(), 1);  //Leaf
        BOOST_CHECK_EQUAL(*cur.begin().end().begin(), 6);
        BOOST_CHECK_EQUAL(*cur.begin().end().begin().begin(), 4); //Leaf
        BOOST_CHECK_EQUAL(*cur.begin().end().end().begin(), 7); //Leaf
        BOOST_CHECK_EQUAL(*cur.end().begin(), 10);
        BOOST_CHECK_EQUAL(*cur.end().end().begin(), 14);
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin(), 13);
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin().begin(), 11); 
        BOOST_CHECK_EQUAL(*cur.end().end().begin().begin().end().begin(), 12); //Leaf
    }

    template <class Cursor>
    static void validate_test_dataset1_minus_1_tree(Cursor cur)
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
    
    fake_binary_tree<T> mbt1, mbt2;
};

template <class T = int>
struct fake_binary_tree_with_list_fixture
: public fake_binary_tree_fixture<T>
, public test_with_list_fixture {
     fake_binary_tree_with_list_fixture()
     : fake_binary_tree_fixture<T>()
     , test_with_list_fixture() {}
};

#endif // LIBS_TREE_TEST_FAKE_BINARY_TREE_HPP
