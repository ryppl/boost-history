//  Copyright (c) 2006-2009, Bernhard Reiter
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

/** 
 * @brief Descendor concept
 * Note that the existence of begin() and end() member functions follows quite
 * naturally from the existence of to_begin() and to_end(), plus
 * CopyConstructibility of X, which is quite a natural requirement for
 * cursors.
 */
template <class X>
struct Descendor
{
public:
    BOOST_CONCEPT_USAGE(Descendor)
    {
        X& rb = d.to_begin();
        rb.to_begin();  // Avoid compiler warning about unused variable

        X b = d.begin();

        X& re = d.to_end();
        re.to_end();    // Avoid compiler warning about unused variable
        
        X e = d.end();

        bool m = b.empty();
        m = false;      // Avoid compiler warning about unused variable
    }
    
private:
    X d;
};


/** 
 * @brief Ascendor concept
 * Note that the existence of a parent() member function follows quite
 * naturally from the existence of to_parent(), plus
 * CopyConstructibility of X, which is quite a natural requirement for
 * cursors.
 * 
 * Ascendor is not derived from Descendor, as there is no obviuos requirement
 * for it, so these things are best kept separate. For a use case of an
 * Ascendor-but-not-Descendor, see eg Knuth 2.3.3, (page 353)
 */
template <class X>
struct Ascendor
{
public:
    BOOST_CONCEPT_USAGE(Ascendor)
    {
        X& rp = a.to_parent();
        rp.to_parent();     // Avoid compiler warning about unused variable

        X p = a.parent();
    }
private:
    X a;
};

/** 
 * @brief RootTracker concept
 * Keeps track of a (subtree) root.
 */
template <class X>
struct RootTracker
  : Ascendor<X>
{
    BOOST_CONCEPT_USAGE(RootTracker)
    {
        bool b = r.is_root();
        b = false;          // Avoid compiler warning about unused variable
    }
private:
    X r;
};

} // namespace boost_concepts

#endif // BOOST_TREE_CURSOR_CONCEPTS_HPP
