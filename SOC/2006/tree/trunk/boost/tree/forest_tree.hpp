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

namespace boost {
namespace tree {

using detail::forest_cursor;


/** 
 * @brief A %forest_tree.
 * This class models the hierarchy concept. It is a (binary) tree adaptor,
 * and a (forest) tree of its own right.
 * TODO: complete this..
 *
*/
template <class T, class Hierarchy = binary_tree<T> >
class forest_tree {
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

//    forest_tree()
//    {
//        h.insert(h.root(), );
//    }
    
    explicit forest_tree(Hierarchy const& hier = Hierarchy()) : h(hier)
    { }
    
    bool empty()
    {
        return h.empty();
    }
    
    size_type size() const
    {
        return h.size();
    }
    
    /**
     * Returns a read/write ("mutable") cursor to the %binary_tree's root.
     */     
    cursor root()
    {
        return cursor(h.root());
    }

    /**
     * Returns a read-only const_cursor to the %binary_tree's root.
     */     
    const_cursor root() const
    {
        return croot();
    }
    
    /**
     * Returns a read-only const_cursor to the %binary_tree's root.
     */     
    const_cursor croot() const
    {
        return const_cursor(h.croot());
    }

    cursor insert(cursor pos, value_type const& val)
    {
        // TODO: Could we remove the root-checking part if root.parent() 
        // returned root? Or don't we even want root?
        base_cursor bc = base_cursor(pos);
        if (bc != h.root())
            bc = bc.parent();
        //if (bc.index())
        return cursor(h.insert(bc, val));
    }
    
 protected:
    hierarchy_type h;
     
};


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_FOREST_TREE_HPP
