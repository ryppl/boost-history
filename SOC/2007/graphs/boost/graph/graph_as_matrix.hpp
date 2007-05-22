// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRPAPH_GRAPH_AS_MATRIX_HPP
#define BOOST_GRPAPH_GRAPH_AS_MATRIX_HPP

#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>

namespace boost
{
    // NOTE:
    // The adjacency list must be a model of an incidence graph since it needs
    // out_edge_iterator and out_edges().
    //
    // NOTE:
    // This are _not_ constant time operations, but it is optimized (sort of) for
    // the type of graph. If directed, there isn't much choice in search algorithm.
    // Specifically, this will run in O(out_degree(u)). For bidirectional graphs,
    // we can optimize somewhat and search either the out edges of u or the in
    // edges of v for the requested edge. This will run in O(min{out_degree(u), in_degree(v)}).
    // A clever trick... Likewise, for undirected graphs, we can search either
    // the out edges of u or v so it runs in O(min{out_degree(u), out_degree(v)}).

    template <typename OEL, typename VL, typename VP,
	      typename EP, typename GP, typename EL>
    std::pair<
	typename adjacency_list<OEL,VL,directedS,VP,EP,GP,EL>::edge_descriptor,
	bool
	>
    edges(typename adjacency_list<OEL,VL,directedS,VP,EP,GP,EL>::vertex_descriptor u,
	  typename adjacency_list<OEL,VL,directedS,VP,EP,GP,EL>::vertex_descriptor v,
	  const adjacency_list<OEL,VL,directedS,VP,EP,GP,EL>& g)
    {
	typedef adjacency_list<OEL,VL,directedS,VP,EP,GP,EL> Graph;

	bool found = false;
	typename Graph::edge_descriptor edge;

	// iterate over the out_edges of u, looking for v
	typename Graph::out_edge_iterator i, j;
	for(tie(i, j) = out_edges(u, g); i != j; ++i) {
	    if(target(*i, g) == v) {
		edge = *i;
		found = true;
		break;
	    }
	}
	return std::make_pair(edge, found);
    }

    template <typename OEL, typename VL, typename VP,
	      typename EP, typename GP, typename EL>
    std::pair<
	typename adjacency_list<OEL,VL,bidirectionalS,VP,EP,GP,EL>::edge_descriptor,
	bool
	>
    edges(typename adjacency_list<OEL,VL,bidirectionalS,VP,EP,GP,EL>::vertex_descriptor u,
	  typename adjacency_list<OEL,VL,bidirectionalS,VP,EP,GP,EL>::vertex_descriptor v,
	  const adjacency_list<OEL,VL,bidirectionalS,VP,EP,GP,EL>& g)
    {
	typedef adjacency_list<OEL,VL,bidirectionalS,VP,EP,GP,EL> Graph;

	bool found = false;
	typename Graph::edge_descriptor edge;

	// iterate over either the out_edges or in_edges depending on the minimum
	// degreer (this should save some time if done repeatedly).
	if(out_degree(u) < in_degree(v)) {
	    typename Graph::out_edge_iterator i, j;
	    for(tie(i, j) = out_edges(u, g); i != j; ++i) {
		if(target(*i, g) == v) {
		    edge = *i;
		    found = true;
		    break;
		}
	    }
	}
	else {
	    typename Graph::in_edge_iterator i, j;
	    for(tie(i, j) = in_edges(v, g); i != j; ++i) {
		if(target(*i, g) == v) {
		    edge = *i;
		    found = true;
		    break;
		}
	    }
	}
	return std::make_pair(edge, found);
    }

    template <typename OEL, typename VL, typename VP,
	      typename EP, typename GP, typename EL>
    std::pair<
	typename adjacency_list<OEL,VL,undirectedS,VP,EP,GP,EL>::edge_descriptor,
	bool
	>
    edges(typename adjacency_list<OEL,VL,undirectedS,VP,EP,GP,EL>::vertex_descriptor u,
	  typename adjacency_list<OEL,VL,undirectedS,VP,EP,GP,EL>::vertex_descriptor v,
	  const adjacency_list<OEL,VL,undirectedS,VP,EP,GP,EL>& g)
    {
	typedef adjacency_list<OEL,VL,undirectedS,VP,EP,GP,EL> Graph;

	bool found = false;
	typename Graph::edge_descriptor edge;

	typename Graph::vertex_descriptor p = out_degree(u) < out_degree(v) ? u : v;

	// iterate over the out_edges of u, looking for v
	typename Graph::out_edge_iterator i, j;
	for(tie(i, j) = out_edges(p, g); i != j; ++i) {
	    if(target(*i, g) == v) {
		edge = *i;
		found = true;
		break;
	    }
	}
	return std::make_pair(edge, found);
    }

    template <typename VP, typename EP, typename GP>
    std::pair<undirected_graph<VP,EP,GP>, bool>
    edges(typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
	  typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	  const undirected_graph<VP,EP,GP>& g)
    {
	edges(g.impl());
    }

    template <typename VP, typename EP, typename GP>
    std::pair<directed_graph<VP,EP,GP>, bool>
    edges(typename directed_graph<VP,EP,GP>::vertex_descriptor u,
	  typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	  const directed_graph<VP,EP,GP>& g)
    {
	edges(g.impl());
    }
}

#endif
