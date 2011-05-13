// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_GRAPH_POLICIES_HPP
#define BOOST_MGL_GRAPH_POLICIES_HPP

#include <boost/mpl/fold.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/pair.hpp>

#include <boost/mgl/aux_/utility.hpp>

namespace boost
{

namespace mgl
{

struct VmapDirectedGraph
{
	template<class AdjacencyList>
	struct apply
	{
		typedef typename ::boost::mpl::fold<
			AdjacencyList,
			::boost::mpl::map0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				::boost::mpl::pair<
					aux::row_start_vertex< ::boost::mpl::placeholders::_2>,
					aux::row_adjacency_vertices< ::boost::mpl::placeholders::_2>
				>
			>
		>::type type;
	};
};

struct VmapUndirectedGraph
{
private:
	template<class AdjacencyList>
	struct build_vertex_set
	{
		// Add the start vertices to a set
		typedef typename ::boost::mpl::fold<
			AdjacencyList,
			::boost::mpl::set0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				aux::row_start_vertex< ::boost::mpl::placeholders::_2>
			>
		>::type s;

		// Add adjacency vertices to the former created set
		typedef typename ::boost::mpl::fold<
			AdjacencyList,
			s,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				aux::row_adjacency_vertices< ::boost::mpl::placeholders::_2>
			>
		>::type type;
	};

	template<class AdjacencyList, class Vertex>
	struct find_ingoing_vertices
	{
		typedef typename ::boost::mpl::fold<
			AdjacencyList,
			::boost::mpl::vector0<>,
			::boost::mpl::if_<
				::boost::mpl::contains<
					aux::row_adjacency_vertices< ::boost::mpl::placeholders::_2>,
					Vertex
				>,
				::boost::mpl::push_back<
					::boost::mpl::placeholders::_1,
					aux::row_start_vertex< ::boost::mpl::placeholders::_2>
				>,
				::boost::mpl::placeholders::_1
			>
		>::type type;
	};

	template<class V1, class V2>
	struct join_vectors
	{
		//! @todo: Optimize this!
		typedef typename ::boost::mpl::fold<
			V2,
			V1,
			::boost::mpl::if_<
				::boost::mpl::contains<
					V1,
					::boost::mpl::placeholders::_2
				>,
				::boost::mpl::placeholders::_1,
				::boost::mpl::push_back<
					::boost::mpl::placeholders::_1,
					::boost::mpl::placeholders::_2
				>
			>
		>::type type;
	};

public:
	template<class AdjacencyList>
	struct apply
	{
		// determine the outgoing vertices
		typedef typename ::boost::mpl::fold<
			AdjacencyList,
			::boost::mpl::map0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				::boost::mpl::pair<
					aux::row_start_vertex< ::boost::mpl::placeholders::_2>,
					aux::row_adjacency_vertices< ::boost::mpl::placeholders::_2>
				>
			>
		>::type m1;

		// determine the ingoing vertices
		typedef typename ::boost::mpl::fold<
			typename build_vertex_set<AdjacencyList>::type,
			::boost::mpl::map0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				::boost::mpl::pair<
					::boost::mpl::placeholders::_2,
					find_ingoing_vertices<
						AdjacencyList,
						::boost::mpl::placeholders::_2
					>
				>
			>
		>::type m2;

		// merge both maps
		//! @todo: Try to optimize this!
		typedef typename ::boost::mpl::fold<
			m1,
			::boost::mpl::map0<>,
			::boost::mpl::insert<
				::boost::mpl::placeholders::_1,
				::boost::mpl::pair<
					::boost::mpl::first< ::boost::mpl::placeholders::_2 >,
					join_vectors<
						::boost::mpl::second< ::boost::mpl::placeholders::_2 >,
						::boost::mpl::at<
							m2,
							::boost::mpl::first< ::boost::mpl::placeholders::_2 >
						>
					>
				>
			>
		>::type type;
	};
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_GRAPH_POLICIES_HPP
