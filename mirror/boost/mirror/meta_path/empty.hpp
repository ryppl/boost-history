/**
 * \file boost/mirror/meta_path/empty.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_EMPTY_HPP
#define BOOST_MIRROR_META_PATH_EMPTY_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mpl/empty.hpp>

namespace boost {
namespace mirror {

namespace meta_path {

/** Size - count of nodes in a node set 
 */
template <class NodeSet>
struct empty : public mpl::empty<typename NodeSet::nodes> { };

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

