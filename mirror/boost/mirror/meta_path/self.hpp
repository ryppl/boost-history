/**
 * \file boost/mirror/meta_path/self.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_SELF_HPP
#define BOOST_MIRROR_META_PATH_SELF_HPP

#include <boost/mirror/meta_path/node_context.hpp>

namespace boost {
namespace mirror {
namespace meta_path {

namespace detail {

	template <class NodeContext>
	struct self_base
	{
		// the nodes and their paths 
		typedef typename NodeContext::paths_and_nodes
			paths_and_nodes;
	};

} // namespace detail

/** A nodeset containing only the nodes of the
 *  given node context
 */
template <class NodeContext>
struct self : node_set<detail::self_base<NodeContext> >
{
	//
	typedef self<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

