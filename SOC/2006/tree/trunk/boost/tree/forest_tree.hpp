//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file forest_tree.hpp
 * Binary tree based forest implementation
 */

#ifndef BOOST_TREE_FOREST_TREE_HPP
#define BOOST_TREE_FOREST_TREE_HPP

#include <boost/tree/detail/cursor/forest.hpp>

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/algorithm.hpp>
#include <boost/tree/cursor_concepts.hpp>

#include <boost/concept_check.hpp>

namespace boost {
namespace tree {

using detail::forest_cursor;
using namespace boost_concepts;

/** 
 * @brief A %forest_tree.
 * This class models the hierarchy concept. It is a (binary) tree adaptor,
 * and a (forest) tree of its own right.
 * TODO: complete this..
 *
*/
template <class T, class Hierarchy = binary_tree<T> >
class forest_tree {

BOOST_CONCEPT_ASSERT((DefaultConstructible<T>));
BOOST_CONCEPT_ASSERT((DescendingCursor< typename binary_tree<T>::cursor >));
BOOST_CONCEPT_ASSERT((DescendingCursor< typename binary_tree<T>::const_cursor >));
//BOOST_CONCEPT_ASSERT((SameType<T, typename Hierarchy::value_type>)); 
// Is there a SameType concept in BCCL?

    typedef forest_tree<T, Hierarchy> self_type;

 public:
    typedef T value_type;
    typedef Hierarchy hierarchy_type;

    typedef typename hierarchy_type::cursor base_cursor;
    typedef typename hierarchy_type::const_cursor base_const_cursor;
    
    typedef forest_cursor<base_cursor> cursor;
    typedef forest_cursor<base_const_cursor> const_cursor;
    
    typedef typename cursor_pointer<cursor>::type pointer;
    typedef typename cursor_reference<cursor>::type reference;
    typedef typename cursor_size<cursor>::type size_type;
    typedef typename cursor_difference<cursor>::type difference_type;

    /**
     *  @brief    The default constructor creates one element at the root.
     *  @param    val   The value the root will be assigned. 
     */
    forest_tree(value_type const& val = value_type()) : h(Hierarchy())
    {
        h.insert(h.root(), val);
    }

    /**
     *  @brief    The default constructor creates no elements.
     *  @param    hier    A hierarchy object that will be used as a base to
     *                    construct this forest_tree.
     */
    explicit forest_tree(Hierarchy const& hier) : h(hier)
    {
        if (h.empty())
            h.insert(h.root(), value_type());
    }

    /**
     * Returns true if the %forest_tree is empty.
     */
    bool empty()
    {
        return h.empty();
    }
    
    /**
     * Returns a read/write ("mutable") cursor to the %forest_tree's root.
     */     
    cursor root()
    {
        return cursor(h.root());
    }

    /**
     * Returns a read-only const_cursor to the %forest_tree's root.
     */     
    const_cursor root() const
    {
        return croot();
    }
    
    /**
     * Returns a read-only const_cursor to the %forest_tree's root.
     */     
    const_cursor croot() const
    {
        return const_cursor(h.croot());
    }

    /**
     * @brief       Inserts val in front of @a pos.
     * @param pos   The %forest_tree cursor in front of which to insert.
     * @param val   The value to insert.
     * @return      A cursor that points to the inserted data.
     */
    cursor insert(cursor pos, value_type const& val)
    {
        return cursor(h.insert(base_cursor(pos), val));
    }

    /**
     * @brief Clears all data from the forest tree.
     */
     void clear()
     {
        h.clear();
     }
    
protected:
    hierarchy_type h;
};


// TODO: template <class Cursor> -> template <class T, class Hierarchy>
// forest_cursor<Cursor> -> forest_tree<T, Hierarchy>::cursor
// const_cursor?
template <class Cursor>
typename forest_cursor<Cursor>::size_type
index(forest_cursor<Cursor> const& cur)
{
    return cur.index();
}

template <class Cursor, class Op>
Op for_each(preorder, forest_cursor<Cursor> s, Op f)
{
    return for_each(preorder(), Cursor(s), f);
}

template <class InCursor, class OutCursor, class Op>
OutCursor transform (preorder, forest_cursor<InCursor> s, forest_cursor<OutCursor> t, Op op)
{
    return transform(preorder(), InCursor(s), InCursor(t), op);
}

template <class InCursor, class OutCursor>
OutCursor copy (preorder, forest_cursor<InCursor> s, forest_cursor<OutCursor> t)
{
    return copy(preorder(), InCursor(s), InCursor(t));
}


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_FOREST_TREE_HPP
