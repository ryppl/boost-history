/**
 * \file boost/mirror/meta_path/export_nodes.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_EXPORT_NODES_HPP
#define BOOST_MIRROR_META_PATH_EXPORT_NODES_HPP

#include <boost/mirror/meta_path/node_context.hpp>

namespace boost {
namespace mirror {

namespace meta_path {

/** Returns a mpl::vector containing the list of nodes
 *  in a node context
 */
template <class NodeContext>
struct export_nodes
{
	typedef typename node_set<NodeContext>::nodes
		type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

