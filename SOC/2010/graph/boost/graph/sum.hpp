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

#ifndef BOOST_GRAPH_SUM_HPP
#define BOOST_GRAPH_SUM_HPP

#include <utility>
#include <boost/graph/global_vertex_mapping.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {
  template <class VertexListGraph, class MutableGraph, class globalVertexMapping> 
  void graph_sum(const VertexListGraph& g1, const VertexListGraph& g2, globalVertexMapping m, MutableGraph& g_out)
  {
    typedef typename graph_traits<VertexListGraph>::vertex_descriptor InVertex;
    typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;
    typedef typename globalVertexMapping::global_id_type id_type;


    typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
    // copy vertices from g1
    for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
      OutVertex new_v = add_vertex(g_out);
      //      copy_vertex(*vi, new_v); // -> should copy vertex properties here
      std::pair < typename globalVertexMapping::global_id_type, bool > id = m.get_id(g1, *vi);
      assert (id.second == true);
      m.associate(g_out, new_v, id.first);
    }
    // copy vertices from (g2 - g1)
    for (tie(vi, vi_end) = vertices(g2); vi != vi_end; ++vi) {
      std::pair < InVertex, bool > v = m.find_vertex( g2, *vi, g1 ); // search for vi in g1
      if (v.second == false) { // vi is not in g1
        OutVertex new_v = add_vertex(g_out);
        //      copy_vertex(*vi, new_v); // -> should copy vertex properties here
        std::pair < id_type, bool > id = m.get_id(g2, *vi);
        assert (id.second == true);
        m.associate(g_out, new_v, id.first);
      }
    }

    typename graph_traits < VertexListGraph >::edge_iterator ei, ei_end;
    // copy edges from g1
    for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
      std::pair < OutVertex, bool > out_s, out_t;
      out_s = m.find_vertex( g1, source(*ei, g1), g_out );
      out_t = m.find_vertex( g1, target(*ei, g1), g_out );
      assert(out_s.second == true && out_t.second == true);

      typename graph_traits<MutableGraph>::edge_descriptor new_e;
      bool inserted;
      boost::tie(new_e, inserted) = add_edge(out_s.first, out_t.first, g_out);
      //        copy_edge(*ei, new_e); // -> should copy vertex properties here
    }
    // copy edges from g2
    for (tie(ei, ei_end) = edges(g2); ei != ei_end; ++ei) {
      std::pair < OutVertex, bool > out_s, out_t;
      out_s = m.find_vertex( g2, source(*ei, g2), g_out );
      out_t = m.find_vertex( g2, target(*ei, g2), g_out );
      assert(out_s.second == true && out_t.second == true);

      typename graph_traits<MutableGraph>::edge_descriptor new_e;
      bool inserted;
      boost::tie(new_e, inserted) = add_edge(out_s.first, out_t.first, g_out);
      //        copy_edge(*ei, new_e); // -> should copy vertex properties here
    }
  }

} // namespace boost

#endif // BOOST_GRAPH_SUM_HPP
