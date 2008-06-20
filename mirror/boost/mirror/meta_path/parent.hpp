/**
 * \file boost/mirror/meta_path/parent.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_PARENT_HPP
#define BOOST_MIRROR_META_PATH_PARENT_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/empty.hpp>

namespace boost {
namespace mirror {
namespace meta_path {

namespace detail {

	template <class NodeContext>
	struct parent_base
	{
	private:
		typedef typename NodeContext::paths_and_nodes
			passed_paths_and_nodes;
		struct process_single_path_and_node
		{
			template <class PathsAndNodes, class Pair>
			struct apply
			{
				struct if_empty
				{
					typedef PathsAndNodes type;
				};

				struct if_not_empty
				{
					typedef typename mpl::push_back<
						PathsAndNodes,
						mpl::pair<
							typename mpl::pop_back<
								typename mpl::first<Pair>::type
							>::type,
							typename mpl::back<
								typename mpl::first<Pair>::type
							>::type
						>
					>::type type;
				};

				typedef typename mpl::eval_if<
					mpl::empty<
						typename mpl::first<Pair>::type
					>,
					if_empty,
					if_not_empty
				>::type type;
			};
		};

	public:
		typedef typename mpl::accumulate<
			passed_paths_and_nodes,
			mpl::vector0<>,
			process_single_path_and_node
		>::type paths_and_nodes;
	};

} // namespace detail

/** A nodeset containing parent node of the
 *  given node context
 */
template <class NodeContext>
struct parent : node_set<detail::parent_base<NodeContext> >
{
	typedef parent<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

