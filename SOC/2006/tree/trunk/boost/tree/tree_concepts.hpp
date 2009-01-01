//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file tree_concepts.hpp
 * Tree concepts
 */
 
#ifndef BOOST_TREE_TREE_CONCEPTS_HPP
#define BOOST_TREE_TREE_CONCEPTS_HPP

#include <boost/concept_check.hpp>

namespace boost_concepts {

template <class X>
struct Tree
{
public:
    typedef typename X::cursor cursor;
    typedef typename X::const_cursor const_cursor;

    BOOST_CONCEPT_USAGE(Tree)
    {
        c = t.root();
        cc = t.root();
    }
    
private:
    X t;
    cursor c;
    const_cursor cc;
    
};

} // namespace boost_concepts

#endif // BOOST_TREE_TREE_CONCEPTS_HPP
