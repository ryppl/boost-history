/**
 * \file boost/mirror/meta_path/siblings.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_SIBLINGS_HPP
#define BOOST_MIRROR_META_PATH_SIBLINGS_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mirror/algorithm/accumulate.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace mirror {
namespace meta_path {

namespace detail {

	template <
		class PathsAndNodes, 
		class Pair, 
		class Node, 
		class IncludeSelf
	>
	struct siblings_base_process_single_path_and_node_apply
	{
		typedef typename mpl::if_<
			IncludeSelf,
			typename mpl::push_back<
				PathsAndNodes,
				Pair
			>::type,
			PathsAndNodes 
		>::type type;
	};

	template <
		class PathsAndNodes, 
		class Pair, 
		class IncludeSelf
	>
	struct siblings_base_process_single_path_and_seq_elem_apply
	{
		typedef typename mpl::first<Pair>::type
			path;

		typedef typename mpl::back<
			path
		>::type meta_object_sequence;

		struct append_sibling
		{
			template <
				class InnerPathsAndNodes,
				class SiblingOrSelf
			>
			struct apply
			{
				typedef typename mpl::second<Pair>::type
					Self;

				typedef typename mpl::if_<
					typename mpl::and_<
						typename boost::is_same<
							SiblingOrSelf,
							Self
						>::type,
						typename mpl::not_<IncludeSelf>::type
					>::type,
					InnerPathsAndNodes,
					typename mpl::push_back<
						InnerPathsAndNodes,
						mpl::pair<
							path,
							SiblingOrSelf
						>
					>::type
				>::type type;

			};
		};


		typedef typename mirror::accumulate<
			meta_object_sequence,
			PathsAndNodes,
			append_sibling 
		>::type type;
	};


	template <
		class PathsAndNodes, 
		class Pair, 
		class ReflectedType, 
		class VariantTag,
		class MetaAttributes,
		class AttribPos,
		class IncludeSelf
	>
	struct siblings_base_process_single_path_and_node_apply<
		PathsAndNodes, 
		Pair, 
		meta_class_attribute<
			ReflectedType, 
			VariantTag,
			MetaAttributes,
			AttribPos
		>,
		IncludeSelf
	> : siblings_base_process_single_path_and_seq_elem_apply<
		PathsAndNodes, 
		Pair, 
		IncludeSelf
	>{ };

	template <
		class PathsAndNodes, 
		class Pair, 
		class Position,
		class BaseClass,
		typename AccessSpec, 
		typename InheritanceSpec,
		class IncludeSelf
	>
	struct siblings_base_process_single_path_and_node_apply<
		PathsAndNodes, 
		Pair, 
		meta_inheritance<
			Position,
			BaseClass,
			AccessSpec, 
			InheritanceSpec
		>,
		IncludeSelf
	> : siblings_base_process_single_path_and_seq_elem_apply<
		PathsAndNodes, 
		Pair, 
		IncludeSelf
	>{ };


	template <class NodeContext, class IncludeSelf>
	struct siblings_base
	{
	private:
		typedef typename NodeContext::paths_and_nodes
			passed_paths_and_nodes;

		struct process_single_path_and_node
		{
			template <class PathsAndNodes, class Pair>
			struct apply
			: siblings_base_process_single_path_and_node_apply<
				PathsAndNodes, 
				Pair, 
				typename mpl::second<Pair>::type,
				IncludeSelf
			>{ };
		};

	public:
		typedef typename mpl::accumulate<
			passed_paths_and_nodes,
			mpl::vector0<>,
			process_single_path_and_node
		>::type paths_and_nodes;
	};

} // namespace detail

/** A nodeset containing sibling nodes of the
 *  given node context
 */
template <class NodeContext>
struct siblings : node_set<
	detail::siblings_base<NodeContext, mpl::false_> 
>
{
	typedef siblings<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

