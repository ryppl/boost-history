/**
 * \file boost/mirror/meta_path/prior.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_PRIOR_HPP
#define BOOST_MIRROR_META_PATH_PRIOR_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mirror/algorithm/prior.hpp>
#include <boost/mirror/algorithm/point_to.hpp>
#include <boost/mirror/algorithm/is_valid.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/back.hpp>

namespace boost {
namespace mirror {
namespace meta_path {

namespace detail {

	template <
		class PathsAndNodes, 
		class Pair, 
		class Node,
		template <class> class PriorOrNext
	>
	struct neighbour_base_process_single_path_and_node_apply
	{
		typedef PathsAndNodes type;
	};

	template <
		class PathsAndNodes, 
		class Path,
		class Iterator,
		class IsValid
	>
	struct neighbour_base_append_if_valid;

	template <
		class PathsAndNodes, 
		class Path,
		class Iterator
	>
	struct neighbour_base_append_if_valid<
		PathsAndNodes, 
		Path,
		Iterator,
		mpl::false_
	>
	{
		typedef PathsAndNodes type;
	};

	template <
		class PathsAndNodes, 
		class Path,
		class Iterator
	>
	struct neighbour_base_append_if_valid<
		PathsAndNodes, 
		Path,
		Iterator,
		mpl::true_
	>
	{
		typedef typename mpl::push_back<
			PathsAndNodes,
			typename mpl::pair<
				Path,
				typename mirror::deref<
					Iterator
				>::type
			>::type
		>::type type;
	};


	template <
		class PathsAndNodes, 
		class Pair, 
		template <class> class PriorOrNext
	>
	struct neighbour_base_process_single_path_and_seq_elem_apply
	{
		typedef typename mpl::first<Pair>::type
			path;

		typedef typename mpl::back<
			path
		>::type meta_object_sequence;

		typedef typename mpl::second<Pair>::type
			meta_object;

		typedef typename PriorOrNext<
				typename mirror::point_to<
					meta_object_sequence,
					typename meta_object::position
				>::type
			>::type
		new_iterator;

		typedef typename neighbour_base_append_if_valid<
			PathsAndNodes,
			path,
			new_iterator,
			typename mirror::is_valid<new_iterator>::type
		>::type type;
	};


	template <
		class PathsAndNodes, 
		class Pair, 
		class ReflectedType, 
		class VariantTag,
		class MetaAttributes,
		class AttribPos,
		template <class> class PriorOrNext
	>
	struct neighbour_base_process_single_path_and_node_apply<
		PathsAndNodes, 
		Pair, 
		meta_class_attribute<
			ReflectedType, 
			VariantTag,
			MetaAttributes,
			AttribPos
		>,
		PriorOrNext
	> : neighbour_base_process_single_path_and_seq_elem_apply<
		PathsAndNodes, 
		Pair, 
		PriorOrNext
	>{ };

	template <
		class PathsAndNodes, 
		class Pair, 
		class Position,
		class BaseClass,
		typename AccessSpec, 
		typename InheritanceSpec,
		template <class> class PriorOrNext
	>
	struct neighbour_base_process_single_path_and_node_apply<
		PathsAndNodes, 
		Pair, 
		meta_inheritance<
			Position,
			BaseClass,
			AccessSpec, 
			InheritanceSpec
		>,
		PriorOrNext
	> : neighbour_base_process_single_path_and_seq_elem_apply<
		PathsAndNodes, 
		Pair, 
		PriorOrNext
	>{ };


	template <class NodeContext, template <class> class PriorOrNext >
	struct neighbour_base
	{
	private:
		typedef typename NodeContext::paths_and_nodes
			passed_paths_and_nodes;

		struct process_single_path_and_node
		{
			template <class PathsAndNodes, class Pair>
			struct apply
			: neighbour_base_process_single_path_and_node_apply<
				PathsAndNodes, 
				Pair, 
				typename mpl::second<Pair>::type,
				PriorOrNext
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

/** A nodeset containing the prior node of the
 *  given node context
 */
template <class NodeContext>
struct prior : node_set<
	detail::neighbour_base<NodeContext, mirror::prior> 
>
{
	typedef prior<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

