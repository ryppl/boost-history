//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

// Todo:
// - Make it use orig_to_copy_t and vertex_index Named Parametersparameters

#ifndef BOOST_GRAPH_UNION_HPP
#define BOOST_GRAPH_UNION_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/default.hpp>

namespace boost {
  template <typename VertexListGraph, typename MutableGraph>
  void graph_disjoint_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph &g_out)
  {
    copy_graph(g1, g_out);
    copy_graph(g2, g_out);
  }

  template <typename Graph, typename MutableGraph, typename P, typename T, typename R>
  void graph_disjoint_union(const Graph& g1, const Graph& g2, MutableGraph &g_out,
                            const bgl_named_params<P, T, R>& params)
  {
    copy_graph
      (g1, g_out,
       vertex_copy(detail::to_two_parameters_vertex_copier
                   (g1, g_out,
                    choose_param
                    (get_param(params, vertex_copy_t()),
                     detail::default_vertex_copy<Graph, MutableGraph>())))
       .edge_copy(
                  detail::to_two_parameters_edge_copier
                  (g1, g_out,
                   choose_param
                   (get_param(params, edge_copy_t()),
                    detail::default_edge_copy<Graph, MutableGraph>())))
       );

    copy_graph
      (g2, g_out,
       vertex_copy(detail::to_two_parameters_vertex_copier
                   (g2, g_out,
                    choose_param
                    (get_param (params, vertex_copy_t()),
                     detail::default_vertex_copy<Graph, MutableGraph>())))
       .edge_copy(detail::to_two_parameters_edge_copier
                  (g2, g_out,
                   choose_param
                   (get_param (params, edge_copy_t()),
                    detail::default_edge_copy<Graph, MutableGraph>())))
       );
  }
} // namespace boost

#endif // BOOST_GRAPH_UNION_HPP
