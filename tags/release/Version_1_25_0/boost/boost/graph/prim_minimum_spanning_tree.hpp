//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
//
#ifndef BOOST_GRAPH_MST_PRIM_HPP
#define BOOST_GRAPH_MST_PRIM_HPP

#include <functional>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace boost {
  
  namespace detail {
    // this should be somewhere else in boost...
    template <class U, class V> struct _project2nd {
      V operator()(U, V v) const { return v; }
    };
  }

  namespace detail {

    // This is Prim's algorithm to calculate the Minimum Spanning Tree
    // for an undirected graph with weighted edges.

    template <class Graph, class P, class T, class R, class Weight>
    inline void
    prim_mst_impl(const Graph& G,
		  typename graph_traits<Graph>::vertex_descriptor s,
		  const bgl_named_params<P,T,R>& params,
		  Weight)
    {
      typedef typename property_traits<Weight>::value_type W;
      std::less<W> compare;
      detail::_project2nd<W,W> combine;
      dijkstra_shortest_paths(G, s, params.distance_compare(compare).
			      distance_combine(combine));
    }
  } // namespace detail

  template <class VertexListGraph, class PredecessorMap,
            class P, class T, class R>
  inline void prim_minimum_spanning_tree
    (const VertexListGraph& g,
     PredecessorMap p_map,
     const bgl_named_params<P,T,R>& params)
  {
    detail::prim_mst_impl
      (g, 
       choose_param(get_param(params, root_vertex_t()), *vertices(g).first), 
       params.predecessor_map(p_map),
       choose_const_pmap(get_param(params, edge_weight), g, edge_weight));
  }

  template <class VertexListGraph, class PredecessorMap>
  inline void prim_minimum_spanning_tree
    (const VertexListGraph& g, PredecessorMap p_map)
  {
    detail::prim_mst_impl
      (g, *vertices(g).first, predecessor_map(p_map).
       weight_map(get(edge_weight, g)),
       get(edge_weight, g));
  }

} // namespace boost

#endif // BOOST_GRAPH_MST_PRIM_HPP
