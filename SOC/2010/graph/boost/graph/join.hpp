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
// - Make it use orig_to_copy_t and vertex_index Named Parameters

#ifndef BOOST_GRAPH_JOIN_HPP
#define BOOST_GRAPH_JOIN_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/union.hpp>

namespace boost {
  namespace detail {
    template <typename Graph, typename MutableGraph,
              typename CopyVertex, typename CopyEdge,
              typename NewEdgeVisitor,
              typename In2OutVertexIndexMap>
    void graph_join_impl(const Graph& g1, const Graph& g2, MutableGraph& g_out,
                         CopyVertex copy_vertex,
                         CopyEdge copy_edge,
                         NewEdgeVisitor edge_visitor,
                         In2OutVertexIndexMap g1_to_out, In2OutVertexIndexMap g2_to_out)
    {
      bool directed = is_convertible<typename MutableGraph::directed_category, directed_tag>::value;
      // if disjoint union was receiving two In2OutVertexIndexMap, then we could just call disjoint union here

      copy_graph
        (g1, g_out,
         orig_to_copy(g1_to_out)
         .vertex_copy(detail::to_two_parameters_vertex_copier(g1, g_out, copy_vertex))
         .edge_copy(detail::to_two_parameters_edge_copier(g1, g_out, copy_edge))
         );

      copy_graph
        (g2, g_out,
         orig_to_copy(g2_to_out)
         .vertex_copy(detail::to_two_parameters_vertex_copier(g2, g_out, copy_vertex))
         .edge_copy(detail::to_two_parameters_edge_copier(g2, g_out, copy_edge))
         );

      typename graph_traits < Graph >::vertex_iterator vi, vi_end, ui, ui_end;
      for (tie(ui, ui_end) = vertices(g1); ui != ui_end; ++ui) {
        for (tie(vi, vi_end) = vertices(g2); vi != vi_end; ++vi) {
          typename graph_traits<MutableGraph>::edge_descriptor new_e;
          bool inserted;
          boost::tie(new_e, inserted) = add_edge(get(g1_to_out, *ui),
                                                 get(g2_to_out, *vi),
                                                 g_out);
          assert(inserted);
          edge_visitor(new_e, g_out);

          if (directed) {
            boost::tie(new_e, inserted) = add_edge(get(g2_to_out, *vi),
                                                   get(g1_to_out, *ui),
                                                   g_out);
            assert(inserted);
            edge_visitor(new_e, g_out);
          }
        }
      }
    }
  } // namespace detail


  template <class Graph, class MutableGraph>
  void graph_join(const Graph& g1, const Graph& g2, MutableGraph& g_out)
  {
    typedef typename graph_traits<MutableGraph>::vertex_descriptor vertex_t;
    std::vector<vertex_t> g1_to_out(num_vertices(g1));
    std::vector<vertex_t> g2_to_out(num_vertices(g2));
    detail::graph_join_impl
      (g1, g2, g_out,
       detail::default_vertex_copy<Graph, MutableGraph>(),
       detail::default_edge_copy<Graph, MutableGraph>(),
       detail::default_edge_visitor<MutableGraph>(),
       make_iterator_property_map(g1_to_out.begin(),
                                  get(vertex_index, g1), g1_to_out[0]),
       make_iterator_property_map(g2_to_out.begin(),
                                  get(vertex_index, g2), g2_to_out[0])
       );
  }

  template <typename Graph, typename MutableGraph, typename P, typename T, typename R>
  void graph_join(const Graph& g1, const Graph& g2, MutableGraph &g_out,
                            const bgl_named_params<P, T, R>& params)
  {
    typedef typename graph_traits<MutableGraph>::vertex_descriptor vertex_t;
    typename std::vector<T>::size_type n1, n2;
    if (is_default_param(get_param(params, orig_to_copy_t()))) {
      n1 = num_vertices(g1);
      n2 = num_vertices(g2);
    } else {
      n1 = n2 = 1;
    }
    std::vector<vertex_t> g1_to_out(n1);
    std::vector<vertex_t> g2_to_out(n2);

    detail::graph_join_impl
      (g1, g2, g_out,
       choose_param(get_param(params, vertex_copy_t()),
                    detail::default_vertex_copy<Graph, MutableGraph>()),
       //     detail::default_edge_copy<Graph, MutableGraph>(),
       choose_param(get_param(params, edge_copy_t()),
                    detail::default_edge_copy<Graph, MutableGraph>()),
       choose_param(get_param(params, edge_visitor_t()),
                    detail::default_edge_visitor<MutableGraph>()),
       make_iterator_property_map(g1_to_out.begin(),
                                  get(vertex_index, g1), g1_to_out[0]),
       make_iterator_property_map(g2_to_out.begin(),
                                  get(vertex_index, g2), g2_to_out[0])
       );
  }

} // namespace boost

#endif // BOOST_GRAPH_JOIN_HPP
