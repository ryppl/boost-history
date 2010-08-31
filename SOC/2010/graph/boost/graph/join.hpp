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
// - Make it use in_to_out_t and vertex_index Named Parameters

#ifndef BOOST_GRAPH_JOIN_HPP
#define BOOST_GRAPH_JOIN_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/default.hpp>

namespace boost {
  namespace detail {
    template <typename VertexListGraph, typename MutableGraph,
              typename CopyVertex, typename CopyEdge,
              typename NewEdgeVisitor,
              typename In2OutVertexIndexMap,
              typename VertexIndexMap>
    void graph_join_impl(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                         CopyVertex copy_vertex,
                         CopyEdge copy_edge,
                         NewEdgeVisitor edge_visitor,
                         In2OutVertexIndexMap g1_to_out, In2OutVertexIndexMap g2_to_out,
                         VertexIndexMap vertex_index1, VertexIndexMap vertex_index2)
    {
      bool directed = is_convertible<typename MutableGraph::directed_category, directed_tag>::value;
      // if disjoint union was receiving two In2OutVertexIndexMap, then we could just call disjoint union here

      copy_graph
        (g1, g_out,
         orig_to_copy(g1_to_out)
         .vertex_index_map(vertex_index1)
         .vertex_copy(detail::make_vertex_copier_wrapper(g1, g_out, copy_vertex))
         .edge_copy(detail::make_edge_copier_wrapper(g1, g_out, copy_edge))
         );

      copy_graph
        (g2, g_out,
         orig_to_copy(g2_to_out)
         .vertex_index_map(vertex_index2)
         .vertex_copy(detail::make_vertex_copier_wrapper(g2, g_out, copy_vertex))
         .edge_copy(detail::make_edge_copier_wrapper(g2, g_out, copy_edge))
         );

      typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end, ui, ui_end;
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


  template <class VertexListGraph, class MutableGraph>
  void graph_join(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    typedef typename graph_traits<MutableGraph>::vertex_descriptor vertex_t;
    std::vector<vertex_t> g1_to_out(num_vertices(g1));
    std::vector<vertex_t> g2_to_out(num_vertices(g2));
    detail::graph_join_impl
      (g1, g2, g_out,
       detail::default_vertex_copy<VertexListGraph, MutableGraph>(),
       detail::default_edge_copy<VertexListGraph, MutableGraph>(),
       detail::default_edge_visitor<MutableGraph>(),
       make_iterator_property_map(g1_to_out.begin(),
                                  get(vertex_index, g1), g1_to_out[0]),
       make_iterator_property_map(g2_to_out.begin(),
                                  get(vertex_index, g2), g2_to_out[0]),
       get(vertex_index, g1),
       get(vertex_index, g2)
       );
  }

  template <typename VertexListGraph, typename MutableGraph, typename P, typename T, typename R>
  void graph_join(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                            const bgl_named_params<P, T, R>& params)
  {
    typedef typename graph_traits<MutableGraph>::vertex_descriptor vertex_t;
    typename std::vector<T>::size_type n1, n2;
    n1 = is_default_param(get_param(params, in1_to_out_t())) ? num_vertices(g1) : 1;
    n2 = is_default_param(get_param(params, in2_to_out_t())) ? num_vertices(g2) : 1;
    std::vector<vertex_t> g1_to_out(n1);
    std::vector<vertex_t> g2_to_out(n2);

    detail::graph_join_impl
      (g1, g2, g_out,
       choose_param(get_param(params, vertex_copy_t()),
                    detail::default_vertex_copy<VertexListGraph, MutableGraph>()),
       choose_param(get_param(params, edge_copy_t()),
                    detail::default_edge_copy<VertexListGraph, MutableGraph>()),
       choose_param(get_param(params, edge_visitor_t()),
                    detail::default_edge_visitor<MutableGraph>()),
       choose_param(get_param(params, in1_to_out_t()),
                    make_iterator_property_map(g1_to_out.begin(),
                                               get(vertex_index, g1), g1_to_out[0])),
       choose_param(get_param(params, in2_to_out_t()),
                    make_iterator_property_map(g2_to_out.begin(),
                                               get(vertex_index, g2), g2_to_out[0])),
       choose_const_pmap(get_param(params, vertex_index1), g1, vertex_index),
       choose_const_pmap(get_param(params, vertex_index1), g2, vertex_index)
       );
  }

  template <class VertexListGraph>
  VertexListGraph graph_join(const VertexListGraph& g1, const VertexListGraph& g2)
  {
    VertexListGraph g_out;
    graph_join(g1, g2, g_out);
    return g_out;
  }

  template <typename VertexListGraph, typename P, typename T, typename R>
  VertexListGraph graph_join(const VertexListGraph& g1, const VertexListGraph& g2,
                             const bgl_named_params<P, T, R>& params)
  {
    VertexListGraph g_out;
    graph_join(g1, g2, g_out, params);
    return g_out;
  }
} // namespace boost

#endif // BOOST_GRAPH_JOIN_HPP
