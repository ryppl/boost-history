//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file ascending.hpp
 * Ascending traversal algorithms for cursors
 */
//TODO:
// Concept checks: MultiwayTree, parent?
// Optimise for trees such as binary_tree with their own ascending begin() members!

#ifndef BOOST_TREE_DETAIL_ALGORITHM_CURSOR_ASCENDING_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_CURSOR_ASCENDING_HPP


namespace boost {
namespace tree {

/** \addtogroup traversal */
/*\@{*/

struct ascending {};

/**
 * @brief    Ascending successor
 * @param c    MultiwayCursor to be set to its ascending successor
 * @ingroup    traversal
 */
template <class MultiwayCursor>
inline void forward(ascending, MultiwayCursor& c)
{
    c.to_parent();
    return;
}

/*\@}*/

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ALGORITHM_CURSOR_ASCENDING_HPP
