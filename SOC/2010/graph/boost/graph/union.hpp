//
//=======================================================================
// Copyright 1997-2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Lie-Quan Lee, Andrew Lumsdaine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_UNION_HPP
#define BOOST_GRAPH_UNION_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/sum.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void graph_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    auto gl2  = get_property(g2, graph_label).hack->vertices;
    auto gl2e = get_property(g2, graph_label).hack->edges;
    typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
    typename graph_traits < VertexListGraph >::edge_iterator ei, ei_end;

    for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
      assert( gl2.find( g1[*vi].name ) == gl2.end() );
    }
    for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
      assert( gl2e.find( g1[*ei].name ) == gl2e.end() );
    }
    graph_sum(g1, g2, g_out);
  }

  template <class VertexListGraph, class MutableGraph>
  void old_graph_union(const VertexListGraph& G1, const VertexListGraph& G2, MutableGraph& G)
  {
    copy_graph(G1, G);
    copy_graph(G2, G);
  }

} // namespace boost

#endif // BOOST_GRAPH_UNION_HPP
