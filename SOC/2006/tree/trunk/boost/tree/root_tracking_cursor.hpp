//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file root_tracking_cursor.hpp
 * Root tracking cursor adaptor implementation
 */

#ifndef BOOST_TREE_ROOT_TRACKING_CURSOR_HPP
#define BOOST_TREE_ROOT_TRACKING_CURSOR_HPP


#include <boost/tree/cursor_adaptor.hpp>
#include <boost/tree/cursor.hpp>

#include <boost/mpl/eval_if.hpp>

#include <utility>

namespace boost {
namespace tree {


template <class Cursor> 
class root_tracking_cursor
: public cursor_adaptor<root_tracking_cursor<Cursor>
                      , Cursor 
//                      , boost::use_default
//                      , typename cursor_horizontal_traversal<Cursor>::type
//                      , typename cursor_vertical_traversal<Cursor>::type
    > {
private:
    struct enabler {};
    typedef root_tracking_cursor<Cursor> self_type;
public:
    typedef typename Cursor::value_type value_type;

    // Container-specific:
    typedef typename Cursor::size_type size_type;

    // Cursor-specific
    typedef root_tracking_cursor<Cursor> cursor;
    typedef root_tracking_cursor<typename Cursor::const_cursor> const_cursor;
    
    // Container-specific:
    typedef cursor iterator;
    typedef const_cursor const_iterator;
    
    template <class OtherCursor>
    struct rebind {
        typedef root_tracking_cursor<OtherCursor> other;
    };
    
    root_tracking_cursor()
    : root_tracking_cursor::cursor_adaptor_(), m_depth(0) {}

    explicit root_tracking_cursor(Cursor c)
    : root_tracking_cursor::cursor_adaptor_(c), m_depth(0) {}

//    template <class OtherCursor>
//    root_tracking_cursor(
//        root_tracking_cursor<OtherCursor> const& other
//      , typename boost::enable_if<
//            boost::is_convertible<OtherCursor*, Cursor*>
//          , enabler
//        >::type = enabler()
//    )
//    : root_tracking_cursor::cursor_adaptor_(other.base())
//    , m_depth(other.m_depth) {}

private: 
    friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;
     
    std::size_t m_depth;
         
//    bool equal(cursor const& other) const
//    {
//    }

    void left()
    {
        ++m_depth;
        this->base_reference().to_begin();
    }

    void right()
    {
        ++m_depth;
        this->base_reference().to_end();
    }

    void up()
    {
        --m_depth;
        this->base_reference().to_parent();
    }

public:
    bool is_root() const
    {
        return !m_depth;
    }
};

template <class Cursor>
inline root_tracking_cursor<Cursor> track_root(Cursor c)
{
    return root_tracking_cursor<Cursor>(c);
}


} // namespace tree
} // namespace boost


#endif // BOOST_TREE_ROOT_TRACKING_CURSOR_HPP
