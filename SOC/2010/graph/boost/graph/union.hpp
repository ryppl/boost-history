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

  // This is also disjoint! But it uses graph_sum and vertice/edge names
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
  void graph_disjoint_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph &g_out)
  {
    copy_graph(g1, g_out);
    copy_graph(g2, g_out);
  }

  template <class VertexListGraph, class MutableGraph, class P, class T, class R>
  void graph_disjoint_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph &g_out,
                            const bgl_named_params<P, T, R>& params)
  {
    // still need to do the same with orig_to_copy and vertex_index_map
    typedef typename detail::vertex_copier<VertexListGraph, MutableGraph> vertex_copier_t;
    vertex_copier_t vc1 = detail::make_vertex_copier<VertexListGraph, MutableGraph>(g1, g_out);
    vertex_copier_t vc2 = detail::make_vertex_copier<VertexListGraph, MutableGraph>(g2, g_out);
    typedef typename detail::edge_copier<VertexListGraph, MutableGraph> edge_copier_t;
    edge_copier_t ec1 = detail::make_edge_copier<VertexListGraph, MutableGraph>(g1, g_out);
    edge_copier_t ec2 = detail::make_edge_copier<VertexListGraph, MutableGraph>(g2, g_out);

    auto p = params
      .vertex_copy (choose_param
                    (get_param(params, vertex_copy_t()),
                     std::pair<vertex_copier_t, vertex_copier_t>(vc1, vc2))
                    )
      .edge_copy (choose_param
                  (get_param(params, edge_copy_t()),
                   std::pair<edge_copier_t, edge_copier_t>(ec1, ec2))
                  );

    copy_graph
      (g1, g_out,
       p.vertex_copy(get_param(p, vertex_copy_t()).first).edge_copy(get_param(p,edge_copy_t()).first)
       );

    copy_graph
      (g2, g_out,
       p.vertex_copy(get_param(p, vertex_copy_t()).second).edge_copy(get_param(p,edge_copy_t()).second)
       );
  }


} // namespace boost

#endif // BOOST_GRAPH_UNION_HPP
