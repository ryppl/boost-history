/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_NODE_BASE_HPP
#define BOOST_INDEXED_SET_INDEX_NODE_BASE_HPP

namespace boost{

namespace indexed_sets{

namespace detail{

/* index_node_base tops the node hierarchy of indexed_set. It holds
 * the value of the element contained.
 */

template<typename Value>
struct index_node_base
{
  typedef Value value_type;
  value_type    value;

private:
  index_node_base(); /* this class is not intended to be cted, merely allocated */
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
