//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_DIFFERENCE_HPP
#define BOOST_GRAPH_DIFFERENCE_HPP

#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/default.hpp>

namespace boost {
  namespace detail {
    template <typename Graph, typename MutableGraph,
              typename CopyVertex, typename SetVertexLabel,
              typename CopyEdge, typename SetEdgeLabel>
    void graph_difference_impl(const Graph& g1, const Graph& g2, MutableGraph& g_out,
                               CopyVertex copy_vertex,
                               SetVertexLabel set_vertex_label,
                               CopyEdge copy_edge,
                               SetEdgeLabel set_edge_label)
    {
      typedef typename graph_traits<Graph>::vertex_descriptor InVertex;
      typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

      //typename graph_bundle_type<Graph>::type const& gl1 = get_property(g1);
      typename graph_bundle_type<Graph>::type const& gl2 = get_property(g2);
      typename graph_bundle_type<MutableGraph>::type& gl_out = get_property(g_out);

      typename graph_traits < Graph >::vertex_iterator vi, vi_end;
      // copy vertices from g1
      for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
        OutVertex new_v = add_vertex(g_out);
        copy_vertex(*vi, g1, new_v, g_out);
        set_vertex_label(g1[*vi].name, new_v, g_out);
        assert( g_out[new_v].name == g1[*vi].name ); // set_vertex_label did it
        assert( gl_out.vertices[ g1[*vi].name ] == new_v ); // set_vertex_label did it
      }

      // copy edges from g1 that are not in g2
      typename graph_traits < Graph >::edge_iterator ei, ei_end;
      for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
        size_t src  = g1[source(*ei, g1)].name;
        size_t targ = g1[target(*ei, g1)].name;
        assert( gl_out.vertices.find(src)  != gl_out.vertices.end() );
        assert( gl_out.vertices.find(targ) != gl_out.vertices.end() );

        if ( gl2.edges.find( g1[*ei].name ) == gl2.edges.end() ) { // if ei is not in g2
          typename graph_traits<MutableGraph>::edge_descriptor new_e;
          bool inserted;
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          copy_edge(*ei, g1, new_e, g_out);
          set_edge_label(g1[*ei].name, new_e, g_out);
          assert( g_out[new_e].name == g1[*ei].name ); // set_edge_label did it
          assert( gl_out.edges[ g1[*ei].name ] == new_e ); // set_edge_label did it
        }
      }
    }
  } // namespace detail

  template <typename Graph, typename MutableGraph>
  void graph_difference(const Graph& g1, const Graph& g2, MutableGraph& g_out)
  {
    detail::graph_difference_impl
      (g1, g2, g_out,
       detail::default_vertex_copy<Graph, MutableGraph>(),
       detail::default_set_vertex_label<MutableGraph>(),
       detail::default_edge_copy<Graph, MutableGraph>(),
       detail::default_set_edge_label<MutableGraph>()
       );
  }

  template <typename Graph, typename MutableGraph,
            typename P, typename T, typename R>
  void graph_difference(const Graph& g1, const Graph& g2, MutableGraph& g_out,
                        const bgl_named_params<P, T, R>& params)
  {
    detail::graph_difference_impl
      (g1, g2, g_out,
       choose_param(get_param(params, vertex_copy_t()),
                    detail::default_vertex_copy<Graph, MutableGraph>()),
       choose_param(get_param(params, set_vertex_label_t()),
                    detail::default_set_vertex_label<MutableGraph>()),
       choose_param(get_param(params, edge_copy_t()),
                    detail::default_edge_copy<Graph, MutableGraph>()),
       choose_param(get_param(params, set_edge_label_t()),
                    detail::default_set_edge_label<MutableGraph>())
       );
  }
} // namespace boost

#endif // BOOST_GRAPH_DIFFERENCE_HPP
