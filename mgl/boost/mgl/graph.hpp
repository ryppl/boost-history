// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_GRAPH_HPP
#define BOOST_MGL_GRAPH_HPP

#include <vector>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/comparison.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <boost/mgl/graph_policies.hpp>
#include <boost/mgl/row.hpp>
#include <boost/mgl/edge.hpp>
#include <boost/mgl/v_prop.hpp>
#include <boost/mgl/iterator.hpp>
#include <boost/mgl/visitor_helpers.hpp>
#include <boost/mgl/colors.hpp>
#include <boost/mgl/aux_/vertex.hpp>
#include <boost/mgl/aux_/utility.hpp>

namespace boost
{

namespace mgl
{

template<
	class Derived,
	class VmapGenerator // policy class to build the map of vertices
>
class graph
{
public:
	struct get_vertices;

private:
	// get the properties of a given vertex as a vector from the derived graph
	// class
	template<class Vertex>
	struct build_vertex_properties
	{
		// get the vertex property list of the graph
		typedef typename Derived::vertex_property_list vertex_property_list;

		// transform the vertex property list into a list only containing the
		// vertices
		typedef typename ::boost::mpl::transform<
			vertex_property_list,
			v_prop_vertex< ::boost::mpl::placeholders::_1>
		>::type vertices;

		// search for 'Vertex'
		typedef typename ::boost::mpl::find<
			vertices,
			Vertex
		>::type vertex_iter;

		// get iterator to end of the property list
		typedef typename ::boost::mpl::end<vertices>::type last_prop_vertex;

		// return an empty vector if 'Vertex' isn't found (no properties exists
		// for 'Vertex') or the properties of 'Vertex'
		typedef typename ::boost::mpl::if_<
			::boost::is_same<vertex_iter, last_prop_vertex>,
			::boost::mpl::vector0<>,
			typename v_prop_property<
				typename ::boost::mpl::at<
					vertex_property_list,
					::boost::mpl::int_<vertex_iter::pos::value>
				>::type
			>::type
		>::type type;
	};

	// create a map containing all vertices of the graph stored as keys and
	// their adjacency vertices and weights as values
	struct create_vertex_map
	{
		// adjacency list of the graph
		typedef typename Derived::adjacency_list adjacency_list;

		// create a map containing the graph vertices as keys and their
		// adjacency vertices inside a vector as values
		typedef typename ::boost::mpl::fold<
			adjacency_list,
			::boost::mpl::map0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				::boost::mpl::pair<
					aux::row_start_vertex< ::boost::mpl::placeholders::_2>,
					::boost::mpl::pair<
						build_vertex_properties<aux::row_start_vertex< ::boost::mpl::placeholders::_2> >,
						aux::row_adjacency_vertices< ::boost::mpl::placeholders::_2>
					>
				>
			>
		>::type type;
	};

	// create a map containing all nodes which are connected with an edge to
	// 'VertexFrom' as key and their properties as value
	template<class VertexFrom>
	struct create_edge_nodes
	{
		typedef typename ::boost::mpl::fold<
			typename Derived::edge_list,
			::boost::mpl::map0<>,
			::boost::mpl::if_<
				::boost::is_same<
					edge_begin_vertex< ::boost::mpl::placeholders::_2>,
					VertexFrom
				>,
				::boost::mpl::insert<
					::boost::mpl::placeholders::_1,
					::boost::mpl::pair<
						edge_end_vertex< ::boost::mpl::placeholders::_2>,
						::boost::mpl::pair<
							edge_weight< ::boost::mpl::placeholders::_2>,
							edge_properties< ::boost::mpl::placeholders::_2>
						>
					>
				>,
				::boost::mpl::placeholders::_1
			>
		>::type type;
	};

	// create a map containing all edges with their properties and weights
	struct create_edge_map
	{
		typedef typename ::boost::mpl::fold<
			typename get_vertices::type,
			::boost::mpl::map0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				::boost::mpl::pair<
					::boost::mpl::placeholders::_2,
					create_edge_nodes< ::boost::mpl::placeholders::_2>
				>
			>
		>::type type;
	};

	template<class MapEntry>
	struct get_map_entry_first
	{
		typedef typename MapEntry::first type;
	};

	template<class AdjacencyList>
	struct get_start_nodes
	{
		typedef typename ::boost::mpl::transform<
			AdjacencyList,
			aux::row_start_vertex< ::boost::mpl::placeholders::_1>
		>::type type;
	};

	template<class AdjacencyList>
	struct get_adjacency_vertices
	{
		typedef typename ::boost::mpl::transform<
			AdjacencyList,
			aux::row_adjacency_vertices< ::boost::mpl::placeholders::_1>
		>::type type;
	};

	template<class StartVertex, class FindVertex, class TraversalStack>
	struct find_vertex_impl
	{
		// get the adjacency list from the graph
		typedef typename Derived::adjacency_list adjacency_list;

		// create a separate list only containing the start nodes
		typedef typename ::boost::mpl::transform<
			adjacency_list,
			aux::row_start_vertex< ::boost::mpl::placeholders::_1>
		>::type start_nodes;

		BOOST_MPL_ASSERT_RELATION(::boost::mpl::size<start_nodes>::value, ==, ::boost::mpl::size<adjacency_list>::value);

		// get an iterator to the row of 'StartNode' from the graph's adjacency list
		typedef typename ::boost::mpl::find<
			start_nodes,
			StartVertex
		>::type row_iter;

		//! @todo Add check for iterator!

		// get the adjacency nodes of 'StartNode'
		typedef typename aux::row_adjacency_vertices<
			typename ::boost::mpl::at<
				adjacency_list,
				::boost::mpl::int_<row_iter::pos::value>
			>::type
		>::type adjacency_vertices;

		// dereference iterator to get the row itself
		typedef typename ::boost::mpl::deref<row_iter>::type row;

		// add node to the traversal stack
		typedef typename ::boost::mpl::if_<
			::boost::mpl::contains<
				TraversalStack,
				StartVertex
			>,
			TraversalStack,
			typename ::boost::mpl::push_back<
				TraversalStack,
				StartVertex
			>::type
		>::type ext_traversal_stack;

		// call 'find_node_impl' recursively to traverse through the adjacency nodes of 'StartNode'
		//! @todo Optimize this O(N^2) operation!!!
		typedef typename ::boost::mpl::if_<
			::boost::mpl::contains<
				ext_traversal_stack,
				FindVertex
			>,
			ext_traversal_stack,
			typename ::boost::mpl::fold<
				adjacency_vertices,
				ext_traversal_stack,
				::boost::mpl::if_<
					::boost::mpl::contains<
						::boost::mpl::placeholders::_1,
						::boost::mpl::placeholders::_2
					>,
					::boost::mpl::placeholders::_1,
					::boost::mpl::if_<
						::boost::mpl::contains<
							::boost::mpl::placeholders::_1,
							FindVertex
						>,
						::boost::mpl::placeholders::_1,
						find_vertex_impl<
							::boost::mpl::placeholders::_2,
							FindVertex,
							::boost::mpl::placeholders::_1
						>
					>
				>
			>::type
		>::type type;
	};

	// build a representation (map) for all vertices and their adjacency vertices
	//struct build_vertices_map
	//{
	//	typedef typename ::boost::mpl::fold<
	//		typename Derived::adjacency_list,
	//		::boost::mpl::map0<>,
	//		::boost::mpl::insert<
	//			::boost::mpl::placeholders::_1,
	//			::boost::mpl::pair<
	//				aux::row_start_vertex< ::boost::mpl::placeholders::_2>,
	//				aux::row_adjacency_vertices< ::boost::mpl::placeholders::_2>
	//			>
	//		>
	//	>::type type;
	//};

	// build a representation (set) of all vertices of the graph
	struct build_vertices_set
	{
		typedef typename ::boost::mpl::fold<
			typename Derived::adjacency_list,
			::boost::mpl::set0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				aux::row_start_vertex< ::boost::mpl::placeholders::_2>
			>
		>::type type;
	};

	template<class ColorMap>
	struct get_next_unvisited
	{
		typedef typename ::boost::mpl::fold<
			typename build_vertices_set::type,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					get_color< ::boost::mpl::placeholders::_2, ColorMap>,
					white
				>,
				::boost::mpl::push_back< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>,
				::boost::mpl::placeholders::_1
			>
		>::type unvisited_vertices;

		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			typename Derived::end::type,
			typename ::boost::mpl::front<unvisited_vertices>::type
		>::type type;
	};

	template<class Vertex, class ColorMap, class TraversalStack, class EndAtStrategy, class Enable = void>
	struct get_next_vertex_dfs
	{
		// get the adjacency vertices of 'Vertex'
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			Vertex
		>::type adjacency_vertices;

		// build a vector with all unvisited adjacency vertices
		//! @todo Try to optimize this!
		typedef typename ::boost::mpl::fold<
			adjacency_vertices,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					get_color< ::boost::mpl::placeholders::_2, ColorMap>,
					white
				>,
				::boost::mpl::push_back< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>,
				::boost::mpl::placeholders::_1
			>
		>::type unvisited_vertices;

		// check if current vertex should be marked as 'black'
		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			typename set_color<Vertex, black, ColorMap>::type,
			ColorMap
		>::type color_map;

		typedef typename ::boost::mpl::eval_if<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			// search recursively
			get_next_vertex_dfs<
				typename ::boost::mpl::back<TraversalStack>::type,
				color_map,
				typename ::boost::mpl::pop_back<TraversalStack>::type,
				EndAtStrategy
			>,
			::boost::mpl::front<unvisited_vertices>
		>::type type;
	};

	// specialication when traversal stack is empty
	template<class Vertex, class ColorMap, class TraversalStack, class EndAtStrategy>
	struct get_next_vertex_dfs<Vertex, ColorMap, TraversalStack, EndAtStrategy, typename ::boost::enable_if< ::boost::mpl::empty<TraversalStack> >::type>
	{
		// get the adjacency vertices of 'Vertex'
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			Vertex
		>::type adjacency_vertices;

		// build a vector with all unvisited adjacency vertices
		//! @todo Try to optimize this!
		typedef typename ::boost::mpl::fold<
			adjacency_vertices,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					get_color< ::boost::mpl::placeholders::_2, ColorMap>,
					white
				>,
				::boost::mpl::push_back< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>,
				::boost::mpl::placeholders::_1
			>
		>::type unvisited_vertices;

		// check if current vertex should be marked as 'black'
		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			typename set_color<Vertex, black, ColorMap>::type,
			ColorMap
		>::type color_map;

		typedef typename ::boost::mpl::eval_if<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			// no vertices are reachable -> find unreachables
			::boost::mpl::eval_if<
				typename ::boost::is_same<EndAtStrategy, ::boost::mgl::EndAtEndOfGraph>::type,
				get_next_unvisited<ColorMap>,
				typename Derived::end::type
			>,
			::boost::mpl::front<unvisited_vertices>
		>::type type;
	};

	// get the next unvisited adjacency vertex from a given vertex
	template<class ColorMap, class TraversalStack, class EndAtStrategy>
	struct get_next_vertex_bfs
	{
		BOOST_MPL_ASSERT_NOT(( ::boost::mpl::empty<TraversalStack> ));

		// get the adjacency vertices of 'Vertex'
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			typename ::boost::mpl::front<TraversalStack>::type
		>::type adjacency_vertices;

		// build a vector with all unvisited adjacency vertices
		//! @todo Try to optimize this!
		typedef typename ::boost::mpl::fold<
			adjacency_vertices,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					get_color< ::boost::mpl::placeholders::_2, ColorMap>,
					white
				>,
				::boost::mpl::push_back< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>,
				::boost::mpl::placeholders::_1
			>
		>::type unvisited_vertices;

		// check if current vertex should be marked as 'black'
		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			typename set_color<
				typename ::boost::mpl::front<TraversalStack>::type,
				black,
				ColorMap>::type,
			ColorMap
		>::type color_map;

		// push back next vertex to traversal stack
		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			// dequeue first entry or ...
			typename ::boost::mpl::pop_front<TraversalStack>::type,
			// ... enqueue next unvisited adjacency vertex
			//typename ::boost::mpl::push_back<
			//	TraversalStack,
			//	typename ::boost::mpl::front<unvisited_vertices>::type
			//>::type
			TraversalStack
		>::type traversal_stack;

		typedef typename ::boost::mpl::eval_if<
			typename ::boost::mpl::empty<unvisited_vertices>::type,
			// search recursively
			::boost::mpl::eval_if<
				typename ::boost::mpl::empty<traversal_stack>::type,
				// no vertices are reachable -> find unreachables
				::boost::mpl::eval_if<
					typename ::boost::is_same<EndAtStrategy, ::boost::mgl::EndAtEndOfGraph>::type,
					get_next_unvisited<ColorMap>,
					typename Derived::end::type
				>,
				get_next_vertex_bfs<
					color_map,
					traversal_stack,
					EndAtStrategy
				>
			>,
			::boost::mpl::front<unvisited_vertices>
		>::type type;
	};

public:
	// get all vertices of the graph
	struct get_vertices
	{
		// get the start vertices of the adjacency list
		typedef typename get_start_nodes<typename Derived::adjacency_list>::type start_nodes;

		// add those vertices to an empty set
		typedef typename ::boost::mpl::fold<
			start_nodes,
			::boost::mpl::set0<>,
			::boost::mpl::if_<
				::boost::mpl::has_key< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>,
				::boost::mpl::placeholders::_1,
				::boost::mpl::insert< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>
			>
		>::type start_set;

		// get the adjacency vertices of the start nodes
		typedef typename get_adjacency_vertices<typename Derived::adjacency_list>::type adjacency_nodes;

		// add those vertices to the former created set
		typedef typename ::boost::mpl::fold<
			adjacency_nodes,
			start_set,
			aux::vec_to_set< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>
		>::type type;
	};

	// get the properties of a given vertex as a vector
	template<class Vertex>
	struct get_vertex_properties
	{
		// transform the vertex property list into a list only containing the
		// vertices
		typedef typename get_map_entry_first<
			typename ::boost::mpl::at<
				typename create_vertex_map::type,
				Vertex
			>::type
		>::type type;
	};

	/// \brief Get the properties of a given egde as a vector
	/// \param VertexFrom Source vertex of the edge
	/// \param VertexTo Target vertex of the edge
	template<class VertexFrom, class VertexTo>
	struct get_edge_properties
	{
		// get the edge map
		typedef typename create_edge_map::type edge_map;

		// get element 'VertexFrom' at edge map
		typedef typename ::boost::mpl::at<
			edge_map,
			VertexFrom
		>::type type_;

		// get element 'VertexTo' at nested map
		typedef typename ::boost::mpl::at<
			type_,
			VertexTo
		>::type type__;

		// check if the edge really exists
		typedef typename ::boost::mpl::if_<
			::boost::is_same<type__, ::boost::mpl::void_>,
			::boost::mpl::void_,
			typename type__::second
		>::type type;
	};

	/// \brief Get the weight of a given egde
	/// \param VertexFrom Source vertex of the edge
	/// \param VertexTo Target vertex of the edge
	template<class VertexFrom, class VertexTo>
	struct get_edge_weight
	{
		// get the edge map
		typedef typename create_edge_map::type edge_map;

		// get element 'VertexFrom' at edge map
		typedef typename ::boost::mpl::at<
			edge_map,
			VertexFrom
		>::type type_;

		// get element 'VertexTo' at nested map
		typedef typename ::boost::mpl::at<
			type_,
			VertexTo
		>::type type__;

		// weight is stored at first entry of pair
		typedef typename type__::first weight;

		// get integer value of the weight
		enum { value = weight::type::value };
	};

	template<class StartVertex, class FindVertex>
	struct find_vertex
	{
		typedef typename find_vertex_impl<StartVertex, FindVertex, ::boost::mpl::vector0<> >::type type;
	};

	// needed to make detection of 'dfs_begin' possible for this graph type
	typedef ::boost::mpl::void_ dfs_begin_supported;

	/// \brief Get a DFS iterator to the begin of the graph
	/// \param EndAtStrategy The strategy used to abort the iteration
	/// \param TracePolicy The policy used to trace the iteration
	/// \param Visitor The visitor class used during the iteration
	template<class EndAtStrategy, class TracePolicy, class Visitor>
	struct dfs_begin
	{
		// initialize the visitor
		typedef typename Visitor::on_init::type visitor_init;

		// get the first vertex
		typedef typename ::boost::mpl::front<
			//typename build_vertices_map::type
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type
		>::type::first first_vertex;

		// mark first vertex as 'gray'
		typedef typename set_color<first_vertex, gray, ::boost::mpl::map0<> >::type color_map;

		// create traversal stack
		typedef ::boost::mpl::vector1<first_vertex> traversal_stack;

		// get the adjacency vertices of the first vertex
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			first_vertex
		>::type adjacency_vertices;

		// call the visitor (if it has defined the right functions)
		typedef typename ::boost::mpl::eval_if<
			typename has_discover_vertex<Visitor>::type,
			// call visitor to discover the first vertex
			do_on_discover_vertex<Visitor, first_vertex, visitor_init, traversal_stack, color_map>,
			::boost::mpl::eval_if<
				typename has_examine_edge<Visitor>::type,
				::boost::mpl::fold<
					adjacency_vertices,
					visitor_init,
					// call visitor to examine all outgoing edges of the vertex
					do_on_examine_edge<Visitor, first_vertex, ::boost::mpl::placeholders::_2, ::boost::mpl::placeholders::_1, traversal_stack, color_map>
				>,
				::boost::mpl::void_
			>
		>::type visitor_result;

		// create a new iterator to first vertex
		typedef dfs_iterator<
			graph,
			first_vertex,
			color_map,
			traversal_stack,
			EndAtStrategy,
			TracePolicy,
			typename TracePolicy::template apply<first_vertex, typename TracePolicy::container_type>::type,
			Visitor,
			visitor_result
		> type;
	};

	// needed to make detection of 'bfs_begin' possible for this graph type
	typedef ::boost::mpl::void_ bfs_begin_supported;

	/// \brief Get a BFS iterator to the begin of the graph
	/// \param EndAtStrategy The strategy used to abort the iteration
	/// \param TracePolicy The policy used to trace the iteration
	/// \param Visitor The visitor class used during the iteration
	template<class EndAtStrategy, class TracePolicy, class Visitor>
	struct bfs_begin
	{
		// initialize the visitor
		typedef typename Visitor::on_init::type visitor_init;

		// get the first vertex
		typedef typename ::boost::mpl::front<
			//typename build_vertices_map::type
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type
		>::type::first first_vertex;

		// mark first vertex as 'gray'
		typedef typename set_color<first_vertex, gray, ::boost::mpl::map0<> >::type color_map;

		// create traversal stack
		typedef ::boost::mpl::vector1<first_vertex> traversal_stack;

		// get the adjacency vertices of the first vertex
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			first_vertex
		>::type adjacency_vertices;

		// call the visitor (if it has defined the right functions)
		typedef typename ::boost::mpl::eval_if<
			typename has_discover_vertex<Visitor>::type,
			// call visitor to discover the first vertex
			do_on_discover_vertex<Visitor, first_vertex, visitor_init, traversal_stack, color_map>,
			::boost::mpl::eval_if<
				typename has_examine_edge<Visitor>::type,
				::boost::mpl::fold<
					adjacency_vertices,
					visitor_init,
					// call visitor to examine all outgoing edges of the vertex
					do_on_examine_edge<Visitor, first_vertex, ::boost::mpl::placeholders::_2, ::boost::mpl::placeholders::_1, traversal_stack, color_map>
				>,
				::boost::mpl::void_
			>
		>::type visitor_result;

		// create a new iterator to first vertex
		typedef bfs_iterator<
			graph,
			first_vertex,
			color_map,
			traversal_stack,
			EndAtStrategy,
			TracePolicy,
			typename TracePolicy::template apply<first_vertex, typename TracePolicy::container_type>::type,
			Visitor,
			visitor_result
		> type;
	};

	template<class VertexIterator>
 	struct edge_begin
	{
		// get the adjacency vertices of vertex where the iterator points to
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			typename VertexIterator::vertex
		>::type adjacency_vertices;

		// get first adjacency vertex or the end indicator of no adjacency
		// vertices exists
		typedef typename ::boost::mpl::if_<
			typename ::boost::is_same<adjacency_vertices, ::boost::mpl::void_>::type,
			typename VertexIterator::graph::end,
			typename ::boost::mpl::front<adjacency_vertices>::type
		>::type first_vertex;

		// create the edge iterator
		typedef edge_iterator<
			typename VertexIterator::graph,
			typename VertexIterator::vertex,
			first_vertex
		> type;
	};

	//! @todo Move this to the derived class to let the user decide which/what should be the end marker.
	struct end
	{
		//typedef ::boost::mpl::void_ type;
		typedef ::boost::mpl::int_<0> type;
	};

	template<class VertexIterator>
	struct dfs_next
	{
		// get the next unvisited adjacency vertex of the current vertex
		typedef get_next_vertex_dfs<
			typename VertexIterator::vertex,
			typename VertexIterator::color_map,
			typename VertexIterator::traversal_stack,
			typename VertexIterator::end_at_strategy
		> next;

		typedef typename next::type next_vertex;
		typedef typename next::color_map color_map_;

		// mark 'next_vertex' as 'gray'
		typedef typename ::boost::mpl::eval_if<
			::boost::is_same<next_vertex, typename Derived::end::type>,
			color_map_,
			set_color<next_vertex, gray, color_map_>
		>::type color_map;

		// add next vertex to traversal stack
		typedef typename ::boost::mpl::eval_if<
			::boost::is_same<next_vertex, typename Derived::end::type>,
			typename VertexIterator::traversal_stack,
			::boost::mpl::push_back<typename VertexIterator::traversal_stack, next_vertex>
		>::type traversal_stack;

		BOOST_MPL_ASSERT_NOT(( ::boost::mpl::empty<traversal_stack> ));

		// get the adjacency vertices of the next vertex
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			next_vertex
		>::type adjacency_vertices;

		// get the iterators visitor class
		typedef typename VertexIterator::visitor_type visitor;

		// call the visitor (if it has defined the right functions)
		typedef typename ::boost::mpl::eval_if<
			typename has_discover_vertex<visitor>::type,
			// call visitor to discover the next vertex
			do_on_discover_vertex<visitor, next_vertex, typename VertexIterator::visitor_result, traversal_stack, typename VertexIterator::color_map>,
			::boost::mpl::eval_if<
				typename has_examine_edge<visitor>::type,
				::boost::mpl::fold<
					adjacency_vertices,
					typename VertexIterator::visitor_result,
					// call visitor to examine all outgoing edges of the vertex
					do_on_examine_edge<visitor, next_vertex, ::boost::mpl::placeholders::_2, ::boost::mpl::placeholders::_1, traversal_stack, typename VertexIterator::color_map>
				>,
				::boost::mpl::void_
			>
		>::type visitor_result;

		// get the trace policy
		typedef typename VertexIterator::trace_policy trace_policy;

		// create a new iterator for the next vertex
		typedef dfs_iterator<
			graph,
			next_vertex,
			color_map,
			traversal_stack,
			typename VertexIterator::end_at_strategy,
			trace_policy,
			typename ::boost::mpl::eval_if<
				::boost::is_same<next_vertex, typename Derived::end::type>,
				typename VertexIterator::vertex_trace,
				typename trace_policy::template apply<next_vertex, typename VertexIterator::vertex_trace>::type
			>::type,
			visitor,
			typename ::boost::mpl::if_<
				typename has_examine_edge<visitor>::type,
				visitor_result,
				typename ::boost::mpl::if_<
					::boost::is_same<next_vertex, typename Derived::end::type>,
					typename VertexIterator::visitor_result,
					visitor_result
				>::type
			>::type
		> type;
	};

	template<class VertexIterator>
	struct bfs_next
	{
		// get the next unvisited adjacency vertex of the current vertex
		typedef get_next_vertex_bfs<
			typename VertexIterator::color_map,
			typename VertexIterator::traversal_stack,
			typename VertexIterator::end_at_strategy
		> next;

		typedef typename next::type next_vertex;
		typedef typename next::color_map color_map_;
		typedef typename next::traversal_stack traversal_stack_;

		// mark 'next_vertex' as 'gray'
		typedef typename ::boost::mpl::eval_if<
			::boost::is_same<next_vertex, typename Derived::end::type>,
			color_map_,
			set_color<next_vertex, gray, color_map_>
		>::type color_map;

		// add next vertex to traversal stack
		typedef typename ::boost::mpl::eval_if<
			::boost::is_same<next_vertex, typename Derived::end::type>,
			traversal_stack_,
			::boost::mpl::push_back<traversal_stack_, next_vertex>
		>::type traversal_stack;

		// get the adjacency vertices of the next vertex
		typedef typename ::boost::mpl::eval_if<
			::boost::is_same<next_vertex, typename Derived::end::type>,
			::boost::mpl::vector0<>,
			::boost::mpl::at<
				//typename build_vertices_map::type,
				typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
				next_vertex
			>
		>::type adjacency_vertices;

		// get the iterators visitor class
		typedef typename VertexIterator::visitor_type visitor;

		// call the visitor (if it has defined the right functions)
		typedef typename ::boost::mpl::eval_if<
			typename has_discover_vertex<visitor>::type,
			// call visitor to discover the next vertex
			do_on_discover_vertex<visitor, next_vertex, typename VertexIterator::visitor_result, traversal_stack, typename VertexIterator::color_map>,
			::boost::mpl::eval_if<
				typename has_examine_edge<visitor>::type,
				::boost::mpl::fold<
					adjacency_vertices,
					typename VertexIterator::visitor_result,
					// call visitor to examine all outgoing edges of the vertex
					do_on_examine_edge<visitor, next_vertex, ::boost::mpl::placeholders::_2, ::boost::mpl::placeholders::_1, traversal_stack, typename VertexIterator::color_map>
				>,
				::boost::mpl::void_
			>
		>::type visitor_result;

		// get the trace policy
		typedef typename VertexIterator::trace_policy trace_policy;

		// create a new iterator for the next vertex
		typedef bfs_iterator<
			graph,
			next_vertex,
			color_map,
			traversal_stack,
			typename VertexIterator::end_at_strategy,
			trace_policy,
			typename ::boost::mpl::eval_if<
				::boost::is_same<next_vertex, typename Derived::end::type>,
				typename VertexIterator::vertex_trace,
				typename trace_policy::template apply<next_vertex, typename VertexIterator::vertex_trace>::type
			>::type,
			visitor,
			typename ::boost::mpl::if_<
				typename has_examine_edge<visitor>::type,
				visitor_result,
				typename ::boost::mpl::if_<
					::boost::is_same<next_vertex, typename Derived::end::type>,
					typename VertexIterator::visitor_result,
					visitor_result
				>::type
			>::type
		> type;
	};

	template<class EdgeIterator>
	struct edge_next
	{
		// get the adjacency vertices of vertex where the iterator points to
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			typename EdgeIterator::source_vertex
		>::type adjacency_vertices;

		typedef typename ::boost::mpl::plus<typename EdgeIterator::edge_index, ::boost::mpl::int_<1> >::type new_index;

		// create a new iterator for the next edge
		typedef edge_iterator<
			typename EdgeIterator::graph,
			typename EdgeIterator::source_vertex,
			typename ::boost::mpl::if_<
				typename ::boost::mpl::greater_equal<new_index, typename ::boost::mpl::size<adjacency_vertices>::type>::type,
				typename EdgeIterator::graph::end::type,
				typename ::boost::mpl::at<adjacency_vertices, new_index>::type
			>::type,
			new_index
		> type;
	};

	// needed to make detection of 'dfs_find' possible for this graph type
	typedef ::boost::mpl::void_ dfs_find_supported;

	/// \brief Get a DFS iterator to a specific vertex of the graph
	/// \param Vertex The vertex the iteration should be started from
	/// \param EndAtStrategy The strategy used to abort the iteration
	/// \param TracePolicy The policy used to trace the iteration
	/// \param Visitor The visitor class used during the iteration
	template<class Vertex, class EndAtStrategy, class TracePolicy, class Visitor>
	struct dfs_find
	{
		typedef typename build_vertices_set::type vertices;

		// initialize the visitor
		typedef typename ::boost::mpl::if_<
			typename ::boost::is_same<typename ::boost::mpl::at<vertices, Vertex>::type, ::boost::mpl::void_>::type,
			::boost::mpl::void_,
			typename Visitor::on_init::type
		>::type visitor_init;

		// get the adjacency vertices of the first vertex
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			Vertex
		>::type adjacency_vertices;

		// mark first vertex as 'gray'
		typedef typename set_color<Vertex, gray, ::boost::mpl::map0<> >::type color_map;

		// create traversal stack
		typedef ::boost::mpl::vector1<Vertex> traversal_stack;

		typedef typename ::boost::mpl::if_<
			typename ::boost::is_same<typename ::boost::mpl::at<vertices, Vertex>::type, ::boost::mpl::void_>::type,
			::boost::mpl::void_,
			dfs_iterator<
				graph,
				Vertex,
				color_map,
				traversal_stack,
				EndAtStrategy,
				TracePolicy,
				typename TracePolicy::template apply<Vertex, typename TracePolicy::container_type>::type,
				Visitor,
				typename ::boost::mpl::eval_if<
					typename has_discover_vertex<Visitor>::type,
					do_on_discover_vertex<
						Visitor,
						Vertex,
						visitor_init,
						traversal_stack,
						color_map
					>,
					::boost::mpl::eval_if<
						typename has_examine_edge<Visitor>::type,
						::boost::mpl::fold<
							adjacency_vertices,
							visitor_init,
							// call visitor to examine all outgoing edges of the vertex
							do_on_examine_edge<
								Visitor,
								Vertex,
								::boost::mpl::placeholders::_2,
								::boost::mpl::placeholders::_1,
								traversal_stack,
								color_map
							>
						>,
						::boost::mpl::void_
					>
				>::type
			>
		>::type type;
	};

	// needed to make detection of 'bfs_find' possible for this graph type
	typedef ::boost::mpl::void_ bfs_find_supported;

	/// \brief Get a BFS iterator to a specific vertex of the graph
	/// \param Vertex The vertex the iteration should be started from
	/// \param EndAtStrategy The strategy used to abort the iteration
	/// \param TracePolicy The policy used to trace the iteration
	/// \param Visitor The visitor class used during the iteration
	template<class Vertex, class EndAtStrategy, class TracePolicy, class Visitor>
	struct bfs_find
	{
		typedef typename build_vertices_set::type vertices;

		// initialize the visitor
		typedef typename ::boost::mpl::if_<
			typename ::boost::is_same<typename ::boost::mpl::at<vertices, Vertex>::type, ::boost::mpl::void_>::type,
			::boost::mpl::void_,
			typename Visitor::on_init::type
		>::type visitor_init;

		// get the adjacency vertices of the first vertex
		typedef typename ::boost::mpl::at<
			//typename build_vertices_map::type,
			typename VmapGenerator::template apply<typename Derived::adjacency_list>::type,
			Vertex
		>::type adjacency_vertices;

		// mark first vertex as 'gray'
		typedef typename set_color<Vertex, gray, ::boost::mpl::map0<> >::type color_map;

		// create traversal stack
		typedef ::boost::mpl::vector1<Vertex> traversal_stack;

		typedef typename ::boost::mpl::if_<
			typename ::boost::is_same<typename ::boost::mpl::at<vertices, Vertex>::type, ::boost::mpl::void_>::type,
			::boost::mpl::void_,
			bfs_iterator<
				graph,
				Vertex,
				color_map,
				traversal_stack,
				EndAtStrategy,
				TracePolicy,
				typename TracePolicy::template apply<Vertex, typename TracePolicy::container_type>::type,
				Visitor,
				typename ::boost::mpl::eval_if<
					typename has_discover_vertex<Visitor>::type,
					do_on_discover_vertex<
						Visitor,
						Vertex,
						visitor_init,
						traversal_stack,
						color_map
					>,
					::boost::mpl::eval_if<
						typename has_examine_edge<Visitor>::type,
						::boost::mpl::fold<
							adjacency_vertices,
							visitor_init,
							do_on_examine_edge<
								Visitor,
								Vertex,
								::boost::mpl::placeholders::_2,
								::boost::mpl::placeholders::_1,
								traversal_stack,
								color_map
							>
						>,
						::boost::mpl::void_
					>
				>::type
			>
		>::type type;
	};

	template<class Vertex>
	struct out_vertices
	{
		typedef typename ::boost::mpl::fold<
			typename Derived::edge_list,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					edge_begin_vertex< ::boost::mpl::placeholders::_2>,
					Vertex
				>,
				::boost::mpl::push_back<
					::boost::mpl::placeholders::_1,
					edge_end_vertex< ::boost::mpl::placeholders::_2>
				>,
				::boost::mpl::placeholders::_1
			>
		>::type type;
	};

	template<class Vertex>
	struct out_edges
	{
		typedef typename ::boost::mpl::fold<
			typename Derived::edge_list,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					edge_begin_vertex< ::boost::mpl::placeholders::_2>,
					Vertex
				>,
				::boost::mpl::push_back<
					::boost::mpl::placeholders::_1,
					::boost::mpl::placeholders::_2
				>,
				::boost::mpl::placeholders::_1
			>
		>::type type;
	};

	template<class Vertex>
	struct in_vertices
	{
		typedef typename ::boost::mpl::fold<
			typename Derived::edge_list,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					edge_end_vertex< ::boost::mpl::placeholders::_2>,
					Vertex
				>,
				::boost::mpl::push_back<
					::boost::mpl::placeholders::_1,
					edge_begin_vertex< ::boost::mpl::placeholders::_2>
				>,
				::boost::mpl::placeholders::_1
			>
		>::type type;
	};

	template<class Vertex>
	struct in_edges
	{
		typedef typename ::boost::mpl::fold<
			typename Derived::edge_list,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::is_same<
					edge_end_vertex< ::boost::mpl::placeholders::_2>,
					Vertex
				>,
				::boost::mpl::push_back<
					::boost::mpl::placeholders::_1,
					::boost::mpl::placeholders::_2
				>,
				::boost::mpl::placeholders::_1
			>
		>::type type;
	};
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_GRAPH_HPP
