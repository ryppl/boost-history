/**
 * \file boost/mirror/meta_path/node_context.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_NODE_CONTEXT_HPP
#define BOOST_MIRROR_META_PATH_NODE_CONTEXT_HPP

#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>

namespace boost {
namespace mirror {

namespace meta_path {

/** Context for a 'node' (meta_class/meta_attributes/meta_attribute/
 *  meta_inheritance/... in a meta_class traversal
 * 
 *  This template is instantiated by the class traversals 
 *  and passed to the visitors
 */
template <class FullPath, class MetaObject>
struct node_context
{
	typedef mpl::vector1<
		mpl::pair<
			FullPath,
			MetaObject
		>
	> paths_and_nodes;
};

template <class FullPath, class MetaObject>
node_context<FullPath, MetaObject> 
make_node_context(FullPath, MetaObject)
{
	return node_context<FullPath, MetaObject>();
}

template <class NodeContext>
struct node_set
{
	typedef typename NodeContext::paths_and_nodes
		paths_and_nodes;

	typedef typename mpl::transform_view<
		typename NodeContext::paths_and_nodes,
		mpl::second< mpl::_1 >
	>::type nodes;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

