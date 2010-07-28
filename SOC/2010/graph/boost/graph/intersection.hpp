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

#ifndef BOOST_GRAPH_INTERSECTION_HPP
#define BOOST_GRAPH_INTERSECTION_HPP

#include <utility>
#include <boost/graph/global_vertex_mapping.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void intersec(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    typedef typename graph_traits<VertexListGraph>::vertex_descriptor InVertex;
    typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;
    
    detail::vertex_copier<VertexListGraph, MutableGraph> copy_vertex = detail::make_vertex_copier(g1, g_out);
    detail::edge_copier<VertexListGraph, MutableGraph> copy_edge = detail::make_edge_copier(g1, g_out);

    auto gl1 = get_property(g1, graph_label).hack->vertices; // c++ 0x
    auto gl2 = get_property(g2, graph_label).hack->vertices;
    auto gl_out = get_property(g_out, graph_label).hack->vertices;

    // copy vertices from (g1 intersection g2)
    typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
      if ( gl2.find( g1[*vi].name ) != gl2.end() ) {
        OutVertex new_v = add_vertex(g_out);
        copy_vertex(*vi, new_v);
        // g_out[new_v].name = g1[*vi].name; // copy_vertex already did it
        gl_out[ g1[*vi].name ] = new_v;
      }
    }

    // copy edges from (g1 intersection g2)
    // not using the vertex name! (but checking with an assert)
    typename graph_traits < VertexListGraph >::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
      auto g2_s  = gl2.find( g1[source(*ei, g1)].name );
      auto g2_t  = gl2.find( g1[target(*ei, g1)].name );
      auto out_s = gl_out.find( g1[source(*ei, g1)].name );
      auto out_t = gl_out.find( g1[target(*ei, g1)].name );

      if ( (g2_s != gl2.end() && g2_t != gl2.end() && edge(g2_s->second, g2_t->second, g2).second) ) {
        assert(out_s != gl_out.end() && out_t != gl_out.end());
        assert( g1[ *ei ].name == g2[ edge(g2_s->second, g2_t->second, g2).first ].name );

        typename graph_traits<MutableGraph>::edge_descriptor new_e;
        bool inserted;
        boost::tie(new_e, inserted) = add_edge(out_s->second, out_t->second, g_out);
        copy_edge(*ei, new_e);
      }
    }
  }


  template <class VertexListGraph, class MutableGraph, class globalVertexMapping> 
  void intersection(const VertexListGraph& g1, const VertexListGraph& g2, globalVertexMapping m, MutableGraph& g_out)
  {
    typedef typename graph_traits<VertexListGraph>::vertex_descriptor InVertex;
    typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

    detail::vertex_copier<VertexListGraph, MutableGraph> copy_vertex = detail::make_vertex_copier(g1, g_out);
    detail::edge_copier<VertexListGraph, MutableGraph> copy_edge = detail::make_edge_copier(g1, g_out);

    // copy vertices from (g1 intersection g2)
    typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
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
    typename graph_traits < VertexListGraph >::edge_iterator ei, ei_end;
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
