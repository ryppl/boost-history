//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file cursor_archetypes.hpp
 * Cursor archetypes
 */
 
#ifndef BOOST_TREE_CURSOR_ARCHETYPES_HPP
#define BOOST_TREE_CURSOR_ARCHETYPES_HPP

#include <boost/iterator/iterator_archetypes.hpp>
#include <boost/tree/cursor.hpp>

namespace boost {
namespace tree {

class descendor_archetype
{
public:
    typedef descending_vertical_traversal_tag vertical_traversal;
    
    descendor_archetype& to_begin() { return *this; }
    descendor_archetype& to_end() { return *this; }

    descendor_archetype begin() const { return *this; }
    descendor_archetype end() const { return *this; }
    
    bool empty() const { return true; }
};

class ascendor_archetype
{
public:
    ascendor_archetype& to_parent() { return *this; }

    ascendor_archetype parent() const { return *this; }
};

template <
    class Value
  , class AccessCategory
  , class HorizontalTraversal
  , class VerticalTraversal
> 
class cursor_archetype;

template <
    class Value
  , class AccessCategory
  , class HorizontalTraversal
  , class VerticalTraversal
>
class cursor_archetype
: public iterator_archetype<Value, AccessCategory, HorizontalTraversal>
{

};

// Ideally derive from ascendor_archetype.
// The problem: begin() and end() return the wrong type! 
// FIXME: constructors etc
template <
    class Value
  , class AccessCategory
  , class HorizontalTraversal
>
class cursor_archetype<Value
                     , AccessCategory
                     , HorizontalTraversal
                     , descending_vertical_traversal_tag>
: public iterator_archetype<Value, AccessCategory, HorizontalTraversal>
//, public descendor_archetype
{
private:
    typedef cursor_archetype<Value
                     , AccessCategory
                     , HorizontalTraversal
                     , descending_vertical_traversal_tag> self_type;
public:
    typedef descending_vertical_traversal_tag vertical_traversal;
    
    self_type& to_begin() { return *this; }
    self_type& to_end() { return *this; }

    self_type begin() const { return *this; }
    self_type end() const { return *this; }
    
    bool empty() const { return true; }
};

template <
    class Value
  , class AccessCategory
  , class HorizontalTraversal
>
class cursor_archetype<Value
                     , AccessCategory
                     , HorizontalTraversal
                     , ascending_vertical_traversal_tag>
: public iterator_archetype<Value, AccessCategory, HorizontalTraversal>
//, public ascendor_archetype
{
private:
    typedef cursor_archetype<Value
                     , AccessCategory
                     , HorizontalTraversal
                     , ascending_vertical_traversal_tag> self_type;
public:
    typedef ascending_vertical_traversal_tag vertical_traversal;
    
    self_type& to_parent() { return *this; }

    self_type parent() const { return *this; }
};

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_CURSOR_ARCHETYPES_HPP
