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

#ifndef BOOST_GRAPH_DIFFERENCE_HPP
#define BOOST_GRAPH_DIFFERENCE_HPP

#include <utility>
#include <boost/graph/global_vertex_mapping.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph, class globalVertexMapping> 
  void difference(const VertexListGraph& g1, const VertexListGraph& g2, globalVertexMapping m, MutableGraph& g_out)
  {
//    detail::make_vertex_copier(g_in, g_out);
//    detail::make_edge_copier(g_in, g_out); 

    typedef typename graph_traits<VertexListGraph>::vertex_descriptor InVertex;
    typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

    // copy vertices from g1
    typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = vertices(g1); vi != vi_end; ++vi) {
      OutVertex new_v = add_vertex(g_out);
      //      copy_vertex(*vi, new_v); // -> should copy vertex properties here
      std::pair < typename globalVertexMapping::global_id_type, bool > id = m.get_id(g1, *vi);
      assert (id.second == true);
      m.associate(g_out, new_v, id.first);
    }


    // copy edges from g1 that are not in g2
    typename graph_traits < VertexListGraph >::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei) {
      std::pair < InVertex,  bool > g2_s, g2_t;
      std::pair < OutVertex, bool > out_s, out_t;
      g2_s  = m.find_vertex( g1, source(*ei, g1), g2 );
      g2_t  = m.find_vertex( g1, target(*ei, g1), g2 );
      out_s = m.find_vertex( g1, source(*ei, g1), g_out );
      out_t = m.find_vertex( g1, target(*ei, g1), g_out );

      assert(out_s.second == true && out_t.second == true);

      if ( ! (g2_s.second && g2_t.second && edge(g2_s.first, g2_t.first, g2).second) ) {
        typename graph_traits<MutableGraph>::edge_descriptor new_e;
        bool inserted;
        boost::tie(new_e, inserted) = add_edge(out_s.first, out_t.first, g_out);
        //        copy_edge(*ei, new_e); // -> should copy vertex properties here
      }
    }

  }

} // namespace boost

#endif // BOOST_GRAPH_DIFFERENCE_HPP
