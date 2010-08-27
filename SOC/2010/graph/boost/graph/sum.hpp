//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_SUM_HPP
#define BOOST_GRAPH_SUM_HPP

#include <utility>
#include <boost/graph/default.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {
  namespace detail {
    template <typename VertexListGraph, typename MutableGraph,
              typename CopyVertex, typename MergeVertices, typename SetVertexLabel,
              typename CopyEdge, typename MergeEdges, typename SetEdgeLabel>
    void graph_sum_impl(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                        CopyVertex copy_vertex,
                        MergeVertices merge_vertices,
                        SetVertexLabel set_vertex_label,
                        CopyEdge copy_edge,
                        MergeEdges merge_edges,
                        SetEdgeLabel set_edge_label)
    {
      typedef typename graph_traits<VertexListGraph>::vertex_descriptor InVertex;
      typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

      //typename graph_bundle_type<VertexListGraph>::type const& gl1 = get_property(g1);
      typename graph_bundle_type<VertexListGraph>::type const& gl2 = get_property(g2);
      typename graph_bundle_type<MutableGraph>::type& gl_out = get_property(g_out);

      typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
      // copy vertices from g1
      for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
        if ( gl2.vertices.find ( g1[*vi].name ) == gl2.vertices.end() ) { // if vi is not in g2
          OutVertex new_v = add_vertex(g_out);
          copy_vertex(*vi, g1, new_v, g_out);
          set_vertex_label(g1[*vi].name, new_v, g_out);
          assert( g_out[new_v].name == g1[*vi].name ); // set_vertex_label did it
          assert( gl_out.vertices[ g1[*vi].name ] == new_v ); // set_vertex_label did it
        } else { // vi is also in g2, we should merge them
          OutVertex new_v = add_vertex(g_out);
          merge_vertices(*vi, g1, gl2.vertices.find ( g1[*vi].name )->second, g2, new_v, g_out);
          set_vertex_label(g1[*vi].name, new_v, g_out);
          assert( g_out[new_v].name == g1[*vi].name ); // set_vertex_label did it
          assert( gl_out.vertices[ g1[*vi].name ] == new_v ); // set_vertex_label did it
        }
      }
      // copy vertices from (g2 - g1)
      for (tie(vi, vi_end) = vertices(g2); vi != vi_end; ++vi) {
        if ( gl_out.vertices.find ( g2[*vi].name ) == gl_out.vertices.end() ) { // if vi is not in g1
          OutVertex new_v = add_vertex(g_out);
          copy_vertex(*vi, g2, new_v, g_out);
          set_vertex_label(g2[*vi].name, new_v, g_out);
          assert( g_out[new_v].name == g2[*vi].name ); // set_vertex_label did it
          assert( gl_out.vertices[ g2[*vi].name ] == new_v );  // set_vertex_label did it
        }
      }

      typename graph_traits < VertexListGraph >::edge_iterator ei, ei_end;
      typename graph_traits < MutableGraph >::edge_descriptor new_e;
      bool inserted;

      // copy edges from g1
      for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
        size_t src  = g1[source(*ei, g1)].name;
        size_t targ = g1[target(*ei, g1)].name;
        assert( gl_out.vertices.find(src)  != gl_out.vertices.end() );
        assert( gl_out.vertices.find(targ) != gl_out.vertices.end() );

        if ( gl2.edges.find( g1[*ei].name ) == gl2.edges.end() ) { // if ei is not in g2
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          copy_edge(*ei, g1, new_e, g_out);
          set_edge_label(g1[*ei].name, new_e, g_out);
          assert( g_out[new_e].name == g1[*ei].name ); // set_edge_label did it
          assert( gl_out.edges[ g1[*ei].name ] == new_e ); // set_edge_label did it
        } else {
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          merge_edges(*ei, g1, gl2.edges.find( g1[*ei].name )->second, g2, new_e, g_out);
          set_edge_label(g1[*ei].name, new_e, g_out);
          assert( g_out[new_e].name == g1[*ei].name ); // set_edge_label did it
          assert( gl_out.edges[ g1[*ei].name ] == new_e ); // set_edge_label did it
        }
      }
      // copy edges from g2 if it is *not* already there
      for (tie(ei, ei_end) = edges(g2); ei != ei_end; ++ei) {
        size_t src  = g2[source(*ei, g2)].name;
        size_t targ = g2[target(*ei, g2)].name;
        assert( gl_out.vertices.find(src)  != gl_out.vertices.end() );
        assert( gl_out.vertices.find(targ) != gl_out.vertices.end() );

        if ( gl_out.edges.find( g2[*ei].name ) == gl_out.edges.end() ) { // ei is not yet in g_out
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          copy_edge(*ei, g2, new_e, g_out);
          set_edge_label(g2[*ei].name, new_e, g_out);
          assert( g_out[new_e].name == g2[*ei].name ); // set_edge_label did it
          assert( gl_out.edges[ g2[*ei].name ] == new_e ); // set_edge_label did it
        }
      }
    }
  } // namespace detail


  template <typename VertexListGraph, typename MutableGraph>
  void graph_sum(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    detail::graph_sum_impl
      (g1, g2, g_out,
       detail::default_vertex_copy<VertexListGraph, MutableGraph>(),
       detail::default_vertices_merge<VertexListGraph, MutableGraph>(),
       detail::default_set_vertex_label<MutableGraph>(),
       detail::default_edge_copy<VertexListGraph, MutableGraph>(),
       detail::default_edges_merge<VertexListGraph, MutableGraph>(),
       detail::default_set_edge_label<MutableGraph>()
       );
  }

  template <typename VertexListGraph, typename MutableGraph,
            typename P, typename T, typename R>
  void graph_sum(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                 const bgl_named_params<P, T, R>& params)
  {
    detail::graph_sum_impl
      (g1, g2, g_out,
       choose_param(get_param(params, vertex_copy_t()),
                    detail::default_vertex_copy<VertexListGraph, MutableGraph>()),
       choose_param(get_param(params, vertices_merge_t()),
                    detail::default_vertices_merge<VertexListGraph, MutableGraph>()),
       choose_param(get_param(params, set_vertex_label_t()),
                    detail::default_set_vertex_label<MutableGraph>()),
       choose_param(get_param(params, edge_copy_t()),
                    detail::default_edge_copy<VertexListGraph, MutableGraph>()),
       choose_param(get_param(params, edges_merge_t()),
                    detail::default_edges_merge<VertexListGraph, MutableGraph>()),
       choose_param(get_param(params, set_edge_label_t()),
                    detail::default_set_edge_label<MutableGraph>())
       );
  }

  template <typename VertexListGraph>
  VertexListGraph graph_sum(const VertexListGraph& g1, const VertexListGraph& g2)
  {
    VertexListGraph g_out;
    graph_sum(g1, g2, g_out);
    return g_out;
  }

  template <typename VertexListGraph, typename P, typename T, typename R>
  VertexListGraph graph_sum(const VertexListGraph& g1, const VertexListGraph& g2,
                 const bgl_named_params<P, T, R>& params)
  {
    VertexListGraph g_out;
    graph_sum(g1, g2, g_out, params);
    return g_out;
  }

  // graph_union is an alias to graph_sum
  template <typename VertexListGraph, typename MutableGraph>
  inline void graph_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    graph_sum(g1, g2, g_out);
  }

  template <typename VertexListGraph, typename MutableGraph,
            typename P, typename T, typename R>
  inline void graph_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                          const bgl_named_params<P, T, R>& params)
  {
    graph_sum(g1, g2, g_out, params);
  }

  template <typename VertexListGraph>
  inline VertexListGraph graph_union(const VertexListGraph& g1, const VertexListGraph& g2)
  {
    return graph_sum(g1, g2);
  }

  template <typename VertexListGraph, typename P, typename T, typename R>
  inline VertexListGraph graph_union(const VertexListGraph& g1, const VertexListGraph& g2,
                                     const bgl_named_params<P, T, R>& params)
  {
    return graph_sum(g1, g2, params);
  }
} // namespace boost

#endif // BOOST_GRAPH_SUM_HPP
