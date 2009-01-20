//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_TREE_TEST_FAKE_BINARY_TREE_HPP
#define LIBS_TREE_TEST_FAKE_BINARY_TREE_HPP

#include <boost/tree/cursor_facade.hpp>
#include <boost/tree/cursor_adaptor.hpp>

#include <vector>

#include "helpers.hpp"

template <class T>
struct fake_descending_binary_cursor;

template <class T>
struct fake_ascending_binary_cursor;

template <class T>
struct fake_root_tracking_binary_cursor;

/// See http://en.wikipedia.org/wiki/Binary_Tree#Methods_for_storing_binary_trees
template <class T>
struct fake_binary_tree {
    typedef std::vector<T> children_type;
    typedef typename children_type::size_type size_type;
    typedef typename children_type::value_type value_type;
    typedef typename children_type::difference_type difference_type;
    typedef typename children_type::pointer pointer;
    typedef typename children_type::reference reference;

    typedef fake_descending_binary_cursor<T> descending_cursor;
    typedef fake_descending_binary_cursor<T/* const*/> const_descending_cursor; //FIXME

    typedef descending_cursor cursor;
    typedef const_descending_cursor const_cursor;

    typedef fake_ascending_binary_cursor<T> ascending_cursor;
    typedef fake_ascending_binary_cursor<T const> const_ascending_cursor;

    typedef fake_root_tracking_binary_cursor<T> root_tracking_cursor;
    typedef fake_root_tracking_binary_cursor<T const> const_root_tracking_cursor;
        
    fake_binary_tree(typename std::vector<T>::size_type s = 0) : m_data(s)
    { }
    
    descending_cursor descending_root()
    {
        return descending_cursor(*this, 0);
    }

    descending_cursor root()
    {
        return descending_cursor(*this, 0);
    }

    const_descending_cursor root() const
    {
        return const_descending_cursor(*this, 0);
    }

    ascending_cursor ascending_root()
    {
        return ascending_cursor(*this, 0);
    }

    root_tracking_cursor root_tracking_root()
    {
        return root_tracking_cursor(*this, 0);
    }

    descending_cursor insert(descending_cursor c, value_type const& v)
    {
        if (c.m_pos >= m_data.size())
            m_data.resize(c.m_pos + 1);
        m_data[c.m_pos] = v;
        return c;
    }

//    ascending_cursor insert(ascending_cursor c, value_type const& v)
//    {
//        m_data[c.m_pos] = v;
//        return c;
//    }
//
//    root_tracking_cursor insert(root_tracking_cursor c, value_type const& v)
//    {
//        m_data[c.m_pos] = v;
//        return c;
//    }
    
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

// This should be easily extensible to nary by replacing the
// factor 2 by n in dereference, left, right and idx. 
template <class T> 
class fake_descending_binary_cursor
: public boost::tree::cursor_facade<
        fake_descending_binary_cursor<T>
      , T
      , boost::bidirectional_traversal_tag
      , boost::tree::descending_vertical_traversal_tag
    >
{
public:
    typedef fake_descending_binary_cursor<T> cursor;
    typedef fake_descending_binary_cursor<T/* const*/> const_cursor;

    typedef typename fake_descending_binary_cursor<T>::cursor_facade_::size_type size_type;

    explicit fake_descending_binary_cursor(fake_binary_tree<T>& t, size_type p = 0)
    : m_tree(t), m_pos(p) {}
    
    fake_descending_binary_cursor(fake_descending_binary_cursor<T> const& other)
    : m_tree(other.m_tree), m_pos(other.m_pos) {}

    fake_descending_binary_cursor<T>& operator=(fake_descending_binary_cursor<T> const& other)
    {
        m_pos = other.m_pos;
        return *this;
    }

    fake_binary_tree<T>& m_tree;
    typename fake_binary_tree<T>::size_type m_pos;

private:
    friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;

    static const 
    typename fake_descending_binary_cursor<T>::cursor_facade_::value_type def_val
    = typename fake_descending_binary_cursor<T>::cursor_facade_::value_type();

    typename fake_descending_binary_cursor<T>::cursor_facade_::reference
    dereference() const
    {
        return m_tree.m_data[(m_pos-1)/2];
    }

    bool equal(fake_descending_binary_cursor<T> const& other) const
    {
        return (this->m_tree == other.m_tree) 
            && (this->m_pos == other.m_pos);
    }

    void increment()
    {
        ++m_pos;
    }
    
    void decrement()
    {
        --m_pos;
    }  

    void left()
    {
        m_pos *= 2;
        ++m_pos;
    }

    void right()
    {
        ++m_pos;
        m_pos *= 2;
    }
    
    bool const empty_() const
    {
        if (m_pos >= m_tree.m_data.size())
            return true;
        if (m_pos == 0)
            return false;
        return (m_tree.m_data[m_pos] == def_val);
    }

    size_type const idx() const
    {
        return (m_pos + 1) % 2;
    }
};        

template <class T>
typename fake_descending_binary_cursor<T>::size_type
index(fake_descending_binary_cursor<T> const& cur)
{
    return cur.index();
}

template <class T>
struct fake_ascending_binary_cursor
: public boost::tree::cursor_adaptor<fake_ascending_binary_cursor<T>
                                   , fake_descending_binary_cursor<T>
                                   , boost::use_default
                                   , boost::use_default
                                   , boost::tree::ascending_vertical_traversal_tag >
{
    typedef fake_ascending_binary_cursor<T> cursor;
    typedef fake_ascending_binary_cursor<T const> const_cursor;

    fake_ascending_binary_cursor(fake_binary_tree<T>& t
                                    , typename fake_binary_tree<T>::size_type p)
    : fake_ascending_binary_cursor::cursor_adaptor_(fake_descending_binary_cursor<T>(t, p)) {}

private: 
    friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;
    
    void up()
    {
        --this->base_reference().m_pos;
        this->base_reference().m_pos >>= 1;
    }

public:
    bool is_root() const
    {
        return this->base_reference().m_pos == 0;
    }
};

template <class T>
struct fake_root_tracking_binary_cursor
: public boost::tree::cursor_adaptor<fake_root_tracking_binary_cursor<T>
                                   , fake_ascending_binary_cursor<T>
                                    >
{
    typedef fake_root_tracking_binary_cursor<T> cursor;
    typedef fake_root_tracking_binary_cursor<T const> const_cursor;

    fake_root_tracking_binary_cursor(fake_binary_tree<T>& t
                                    , typename fake_binary_tree<T>::size_type p)
    : fake_root_tracking_binary_cursor::cursor_adaptor_(fake_ascending_binary_cursor<T>(t, p)) {}

private: 
    friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;

public:
    bool is_root() const
    {
        return this->base_reference().is_root();
    }
};

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
    fake_binary_tree_fixture() : fbt1(generate_fake_binary_tree()), fbt2(57) {}
    
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
    
    fake_binary_tree<T> fbt1, fbt2;
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
