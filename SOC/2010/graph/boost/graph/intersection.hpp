//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_INTERSECTION_HPP
#define BOOST_GRAPH_INTERSECTION_HPP

#include <utility>
#include <boost/graph/global_vertex_mapping.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/default.hpp>

namespace boost {
  namespace detail {
    template <typename Graph, typename MutableGraph,
              typename MergeVertices, typename SetVertexLabel,
              typename MergeEdges, typename SetEdgeLabel>
    void graph_intersection_impl(const Graph& g1, const Graph& g2, MutableGraph& g_out,
                                 MergeVertices merge_vertices,
                                 SetVertexLabel set_vertex_label,
                                 MergeEdges merge_edges,
                                 SetEdgeLabel set_edge_label)
    {
      typedef typename graph_traits<Graph>::vertex_descriptor InVertex;
      typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

      auto & gl1 = get_property(g1);
      auto & gl2 = get_property(g2);
      auto & gl_out = get_property(g_out);

      // copy vertices from (g1 intersection g2)
      typename graph_traits < Graph >::vertex_iterator vi, vi_end;
      for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
        if ( gl2.vertices.find( g1[*vi].name ) != gl2.vertices.end() ) { // if vi is in g2
          OutVertex new_v = add_vertex(g_out);
          merge_vertices(*vi, g1, gl2.vertices.find ( g1[*vi].name )->second, g2, new_v, g_out);
          set_vertex_label(g1[*vi].name, new_v, g_out);
          assert( g_out[new_v].name == g1[*vi].name ); // set_vertex_label did it
          assert( gl_out.vertices[ g1[*vi].name ] == new_v ); // set_vertex_label did it
        }
      }

      typename graph_traits < Graph >::edge_iterator ei, ei_end;
      typename graph_traits < MutableGraph >::edge_descriptor new_e;
      bool inserted;
      // copy edges from (g1 intersection g2)
      for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
        auto src  = g1[source(*ei, g1)].name;
        auto targ = g1[target(*ei, g1)].name;
        assert( gl_out.vertices.find(src)  != gl_out.vertices.end() );
        assert( gl_out.vertices.find(targ) != gl_out.vertices.end() );

        if ( gl2.edges.find( g1[*ei].name ) != gl2.edges.end() ) { // if ei is in g2
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          //          merge_edges(*ei, g1, gl2.edges[ g1[*ei].name ], g2, new_e, g_out); // Does not compile! Why?
          merge_edges(*ei, g1, gl2.edges.find( g1[*ei].name )->second, g2, new_e, g_out);
          set_edge_label(g1[*ei].name, new_e, g_out);
          assert( g_out[new_e].name == g1[*ei].name ); // set_edge_label did it
          assert( gl_out.edges[ g1[*ei].name ] == new_e ); // set_edge_label did it
        }
      }
    }
  } // namespace detail

  template <typename Graph, typename MutableGraph>
  void graph_intersection(const Graph& g1, const Graph& g2, MutableGraph& g_out)
  {
    detail::graph_intersection_impl
      (g1, g2, g_out,
       detail::default_vertices_merge<Graph, MutableGraph>(),
       detail::default_set_vertex_label<MutableGraph>(),
       detail::default_edges_merge<Graph, MutableGraph>(),
       detail::default_set_edge_label<MutableGraph>()
       );
  }

  template <typename Graph, typename MutableGraph,
            typename P, typename T, typename R>
  void graph_intersection(const Graph& g1, const Graph& g2, MutableGraph& g_out,
                          const bgl_named_params<P, T, R>& params)
  {
    detail::graph_intersection_impl
      (g1, g2, g_out,
       choose_param(get_param(params, vertices_merge_t()),
                    detail::default_vertices_merge<Graph, MutableGraph>()),
       choose_param(get_param(params, set_vertex_label_t()),
                    detail::default_set_vertex_label<MutableGraph>()),
       choose_param(get_param(params, edges_merge_t()),
                    detail::default_edges_merge<Graph, MutableGraph>()),
       choose_param(get_param(params, set_edge_label_t()),
                    detail::default_set_edge_label<MutableGraph>())
       );
  }

  // Version with globalVertexMapping
  template <class Graph, class MutableGraph, class globalVertexMapping> 
  void gvm_graph_intersection(const Graph& g1, const Graph& g2, globalVertexMapping m, MutableGraph& g_out)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor InVertex;
    typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

    detail::vertex_copier<Graph, MutableGraph> copy_vertex = detail::make_vertex_copier(g1, g_out);
    detail::edge_copier<Graph, MutableGraph> copy_edge = detail::make_edge_copier(g1, g_out);

    // copy vertices from (g1 intersection g2)
    typename graph_traits < Graph >::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
      std::pair < InVertex, bool > v = m.find_vertex( g1, *vi, g2 ); // search for vi in g2
      if (v.second == true) { // vi is also in g2
        OutVertex new_v = add_vertex(g_out);
        copy_vertex(*vi, new_v);
        std::pair < typename globalVertexMapping::global_id_type, bool > id = m.get_id(g1, *vi);
        assert (id.second == true);
        m.associate(g_out, new_v, id.first);
      }
    }

    // copy edges from (g1 intersection g2)
    typename graph_traits < Graph >::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
      std::pair < InVertex,  bool > g2_s, g2_t;
      std::pair < OutVertex, bool > out_s, out_t;
      g2_s  = m.find_vertex( g1, source(*ei, g1), g2 );
      g2_t  = m.find_vertex( g1, target(*ei, g1), g2 );
      out_s = m.find_vertex( g1, source(*ei, g1), g_out );
      out_t = m.find_vertex( g1, target(*ei, g1), g_out );

      if ( (g2_s.second && g2_t.second && edge(g2_s.first, g2_t.first, g2).second) ) {
        assert(out_s.second == true && out_t.second == true);
        typename graph_traits<MutableGraph>::edge_descriptor new_e;
        bool inserted;
        boost::tie(new_e, inserted) = add_edge(out_s.first, out_t.first, g_out);
        copy_edge(*ei, new_e);
      }
    }
  }

} // namespace boost

#endif // BOOST_GRAPH_INTERSECTION_HPP
