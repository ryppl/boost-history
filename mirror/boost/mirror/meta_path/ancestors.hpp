/**
 * \file boost/mirror/meta_path/ancestors.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_ANCESTORS_HPP
#define BOOST_MIRROR_META_PATH_ANCESTORS_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/empty.hpp>

namespace boost {
namespace mirror {
namespace meta_path {

namespace detail {

	template <class NodeContext, class IncludeSelf>
	struct ancestors_base
	{
	private:
		typedef typename NodeContext::paths_and_nodes
			passed_paths_and_nodes;
		// the nodes and their paths 
		struct append_path_and_node
		{
			template <class PathsAndNodes, class Node>
			struct apply
			{
				typedef typename mpl::push_back<
					PathsAndNodes,
					mpl::pair<
						typename mpl::push_back<
							typename mpl::first<
								typename mpl::back<PathsAndNodes>::type
							>::type,
							typename mpl::second<
								typename mpl::back<PathsAndNodes>::type
							>::type
						>::type,
						Node
					>
				>::type type;
			};
		};

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
						typedef typename mpl::accumulate<
							// the given paths and nodes without the 
							// the root
							typename mpl::pop_front<
								typename mpl::first<Pair>::type
							>::type,
							// append a pair containing an empty path and 
							// the root node
							typename mpl::push_back<
								PathsAndNodes,
								mpl::pair<
									mpl::vector0<>,
									typename mpl::front<
										typename mpl::first<Pair>::type
									>::type
								>
							>::type,
							// appends a new path/node pair to the state
							append_path_and_node
						>::type type;
				};

				typedef typename mpl::eval_if<
					mpl::empty<
						typename mpl::first<Pair>::type
					>,
					if_empty,
					if_not_empty
				>::type ancestor_paths_and_nodes;

				typedef typename mpl::if_<
					IncludeSelf,
					typename mpl::push_back<
						ancestor_paths_and_nodes,
						Pair
					>::type,
					ancestor_paths_and_nodes
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

/** A nodeset containing ancestor nodes of the
 *  given node context
 */
template <class NodeContext>
struct ancestors : node_set<
	detail::ancestors_base<NodeContext, mpl::false_> 
>
{
	typedef ancestors<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

