//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file postorder_iterator.hpp
 * Postorder iterator wrapper around cursor.
 */

// TODO: concept checks.

#ifndef BOOST_TREE_DETAIL_ITERATOR_POSTORDER_HPP
#define BOOST_TREE_DETAIL_ITERATOR_POSTORDER_HPP

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

//#include <boost/test/minimal.hpp>

namespace boost {
namespace tree {
	
namespace postorder {

#include <boost/tree/detail/iterator/_order.hpp>

} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_DETAIL_ITERATOR_POSTORDER_HPP
