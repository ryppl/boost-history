//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_COMPLEMENT_HPP
#define BOOST_GRAPH_COMPLEMENT_HPP

#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/default.hpp>

namespace boost {
  namespace detail {

    template <typename Graph, typename MutableGraph,
              typename CopyVertex,
              typename NewEdgeVisitor,
              typename Orig2OutVertexIndexMap>
    void graph_complement_impl(const Graph& g_in, MutableGraph& g_out,
                               CopyVertex copy_vertex, NewEdgeVisitor edge_visitor,
                               Orig2OutVertexIndexMap orig2out,
                               bool reflexive)
    {
      typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

      typename graph_traits < Graph >::vertex_iterator vi, vi_end;
      typename graph_traits < Graph >::vertex_iterator ui, ui_end;

      // copy vertices from g_in
      for (tie(vi, vi_end) = vertices(g_in); vi != vi_end; ++vi) {
        OutVertex new_v = add_vertex(g_out);
        put(orig2out, *vi, new_v);
        copy_vertex(*vi, g_in, new_v, g_out);
      }

      // create edges
      for (tie(ui, ui_end) = vertices(g_in); ui != ui_end; ++ui) {
        for (tie(vi, vi_end) = vertices(g_in); vi != vi_end; ++vi) {
          if ( (reflexive || ui != vi) && edge(*ui, *vi, g_in).second == false ) {
            typename graph_traits<MutableGraph>::edge_descriptor new_e;
            bool inserted;
            boost::tie(new_e, inserted) = add_edge(get(orig2out, *ui),
                                                   get(orig2out, *vi),
                                                   g_out);
            assert(inserted);
            edge_visitor(new_e, g_out);
          }
        }
      }
    }
  } // namespace detail

  template <typename Graph, typename MutableGraph>
  void graph_complement(const Graph& g_in, MutableGraph& g_out, bool reflexive)
  {
    typedef typename graph_traits<MutableGraph>::vertex_descriptor vertex_t;
    std::vector<vertex_t> orig2out(num_vertices(g_in));

    detail::graph_complement_impl
      (g_in, g_out,
       detail::default_vertex_copy<Graph, MutableGraph>(),
       detail::default_edge_visitor<MutableGraph>(),
       make_iterator_property_map(orig2out.begin(),
                                  get(vertex_index, g_in), orig2out[0]),
       reflexive
       );
  }

  template <typename Graph, typename MutableGraph,
            typename P, typename T, typename R>
  void graph_complement(const Graph& g_in, MutableGraph& g_out,
                        const bgl_named_params<P, T, R>& params, bool reflexive)
  {
    typename std::vector<T>::size_type n;
    n = is_default_param(get_param(params, orig_to_copy_t()))
      ? num_vertices(g_in) : 1;
    std::vector<BOOST_DEDUCED_TYPENAME graph_traits<MutableGraph>::vertex_descriptor>
      orig2out(n);

    detail::graph_complement_impl
      (g_in, g_out,
       choose_param(get_param(params, vertex_copy_t()),
                    detail::default_vertex_copy<Graph, MutableGraph>()),
       choose_param(get_param(params, edge_visitor_t()),
                    detail::default_edge_visitor<MutableGraph>()),
       choose_param(get_param(params, orig_to_copy_t()),
                    make_iterator_property_map
                    (orig2out.begin(),
                     choose_const_pmap(get_param(params, vertex_index),
                                       g_in, vertex_index), orig2out[0])),
       reflexive
       );
  }

//  template <typename Graph, typename MutableGraph>
//  void graph_reflexive_complement(const Graph& g_in, MutableGraph& g_out)
//  {
//    typedef typename graph_traits<MutableGraph>::vertex_descriptor vertex_t;
//    std::vector<vertex_t> orig2out(num_vertices(g_in));
//
//    detail::graph_complement_impl
//      (g_in, g_out,
//       detail::make_vertex_copier(g_in, g_out),
//       make_iterator_property_map(orig2out.begin(),
//                                  get(vertex_index, g_in), orig2out[0]),
//       true
//       );
//  }

} // namespace boost

#endif // BOOST_GRAPH_COMPLEMENT_HPP
