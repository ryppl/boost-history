//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file cursor_concepts.hpp
 * Cursor concepts
 */
 
#ifndef BOOST_TREE_CURSOR_CONCEPTS_HPP
#define BOOST_TREE_CURSOR_CONCEPTS_HPP

#include <boost/concept_check.hpp>

namespace boost {
namespace tree {

template <class X>
struct DescendingCursor
{
public:
    BOOST_CONCEPT_USAGE(DescendingCursor)
    {
        d.to_begin();
        d.to_end();
    }
    
private:
    X d;
    
};

template <class T>
class descending_cursor_archetype
{
public:
    void to_begin() {}
    void to_end() {}
};

// Derive from DescendingCursor or not?
template <class X>
struct AscendingCursor
{
public:
    BOOST_CONCEPT_USAGE(AscendingCursor)
    {
        a.to_parent();
    }
    
private:
    X a;
};

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_CURSOR_CONCEPTS_HPP
