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

#ifndef BOOST_GRAPH_JOIN_HPP
#define BOOST_GRAPH_JOIN_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/union.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void graph_join(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

    auto & gl1 = get_property(g1, graph_label).hack->vertices; // c++ 0x
    auto & gl2 = get_property(g2, graph_label).hack->vertices;
    auto & gl_out  = get_property(g_out, graph_label).hack->vertices;

    typename graph_traits < MutableGraph >::vertex_iterator vi, vi_end;
    typename graph_traits < MutableGraph >::vertex_iterator ui, ui_end;

    typedef typename VertexListGraph::directed_category Dr;
    bool directed = is_convertible<Dr, directed_tag>::value;

    graph_union(g1, g2, g_out);

    for (tie(ui, ui_end) = vertices(g1); ui != ui_end; ++ui) {
      for (tie(vi, vi_end) = vertices(g2); vi != vi_end; ++vi) {
        OutVertex u = gl_out [ g1[*ui].name ];
        OutVertex v = gl_out [ g2[*vi].name ];
        typename graph_traits<MutableGraph>::edge_descriptor new_e;
        bool inserted;
        boost::tie(new_e, inserted) = add_edge(u, v, g_out);
        if (directed)
          boost::tie(new_e, inserted) = add_edge(v, u, g_out);
      }
    }
  }

} // namespace boost

#endif // BOOST_GRAPH_JOIN_HPP
