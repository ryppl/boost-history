//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file insert_cursor.hpp
 * Insert cursor implementation
 */

#ifndef BOOST_TREE_INSERT_CURSOR_HPP
#define BOOST_TREE_INSERT_CURSOR_HPP

#include <boost/tree/cursor_adaptor.hpp>

namespace boost {
namespace tree {

using boost::iterator_core_access;

template <class Tree>
class insert_cursor;

/**
 * @brief Output cursor wrapper around an output iterator.
 * 
 * This can be very useful e.g. to have cursor algorithms actually work on
 * iterators, thus permitting some kind of linearization of a given subtree.
 * (Modelled after std::insert_iterator and the like.)
 * 
 * For construction, the outputter_cursor_iterator_wrapper might come in useful
 * in saving keystrokes.
 */
// TODO: Complete this.
// Shouldn't we be using cursor_facade?
template <class Tree>
class insert_cursor
: public cursor_adaptor<insert_cursor<Tree>
                      , typename Tree::cursor 
//                      , boost::use_default
//                      , boost::use_default
//                      , boost::use_default
    >
 {
protected:
    mutable Tree& tree;
public:        
    /**
     * For construction, we obviously need a tree and a cursor to work on (i.e., write to).
     */
    explicit insert_cursor(Tree& x, typename Tree::cursor cur)
    : insert_cursor::cursor_adaptor_(cur), tree(x) {}

    // Cursor-specific
    typedef insert_cursor<typename Tree::cursor> cursor;
    typedef insert_cursor<typename Tree::const_cursor> const_cursor;

    operator typename Tree::cursor()
    {
        return this->base_reference();
    }
    
private:
    friend class boost::iterator_core_access;
    friend class boost::tree::cursor_core_access;

    typename insert_cursor::cursor_adaptor_::reference dereference() const
    {
        if (this->base_reference().parity()) {
            const_cast<typename Tree::cursor&>(this->base_reference())
            = tree.insert(this->base_reference(), typename Tree::value_type());
        }
        return *this->base_reference();
    }

    void left()
    {
        this->base_reference() = 
            tree.insert(this->base_reference(), typename Tree::value_type());
    }
};

/** 
 * @param o    An output iterator.
 * @result    An instance of insert_cursor working on o.
 * 
 * Use as shortcut for cumbersome typenames, just as with std::inserter and the like.
 */
template<class Tree>
inline insert_cursor<Tree>
tree_inserter(Tree& t, typename Tree::cursor c)
{
    return insert_cursor<Tree>(t, c);
}

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_INSERT_CURSOR_HPP
