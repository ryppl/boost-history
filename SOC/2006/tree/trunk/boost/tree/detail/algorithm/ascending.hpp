//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file ascending.hpp
 * Ascending traversal algorithms for cursors
 */

#ifndef BOOST_TREE_DETAIL_ALGORITHM_ASCENDING_HPP
#define BOOST_TREE_DETAIL_ALGORITHM_ASCENDING_HPP

#include <boost/iterator/iterator_categories.hpp>

namespace boost {
namespace tree {

/** \addtogroup traversal */
/*\@{*/

struct ascending {
    typedef forward_traversal_tag iterator_category; 
};

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

#endif // BOOST_TREE_DETAIL_ALGORITHM_ASCENDING_HPP
