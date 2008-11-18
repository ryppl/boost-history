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
#include <boost/iterator/iterator_concepts.hpp>

namespace boost_concepts {

// TODO: Adapt concept requirements for algorithms according to archetypes!


// Even split up into two types: one with only to_begin() and to_end(), and
// one with also begin() and end() ?
// I think we're lacking some requirements imposed on the return types of these
// member functions, but that might overlap with iterator requirements. 
template <class X>
struct Descendor
{
public:
    BOOST_CONCEPT_USAGE(Descendor)
    {
        d.to_begin();
        d.begin();
        d.to_end();
        d.end();
    }
    
private:
    X d;
    
};

template <class X>
struct DescendingCursor
  : Descendor<X>, LvalueIterator<X>
{
};

// Derive from DescendingCursor or not?
// See eg Knuth 2.3.3, p 353.
template <class X>
struct Ascendor
{
public:
    BOOST_CONCEPT_USAGE(Ascendor)
    {
        a.to_parent();
        a.parent();
    }
private:
    X a;
};

template <class X>
struct AscendingCursor
  : Ascendor<X>, LvalueIterator<X>
{
};

template <class X>
struct RootTrackingCursor
  : AscendingCursor<X>
{
    BOOST_CONCEPT_USAGE(RootTrackingCursor)
    {
        b = r.is_root();
    }
private:
    X r;
    bool b;
};

} // namespace boost_concepts

#endif // BOOST_TREE_CURSOR_CONCEPTS_HPP
