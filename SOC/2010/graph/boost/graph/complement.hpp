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

#ifndef BOOST_GRAPH_COMPLEMENT_HPP
#define BOOST_GRAPH_COMPLEMENT_HPP

#include <utility>
#include <boost/graph/global_vertex_mapping.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {
  namespace detail {
    template <class VertexListGraph, class MutableGraph> 
    void graph_complement_impl(const VertexListGraph& g_in, MutableGraph& g_out, bool reflexive)
    {
      typedef typename graph_traits<VertexListGraph>::vertex_descriptor InVertex;
      typedef typename graph_traits<MutableGraph>::vertex_descriptor OutVertex;

      detail::vertex_copier<VertexListGraph, MutableGraph> copy_vertex = detail::make_vertex_copier(g_in, g_out);

      auto & gl_in  = get_property(g_in,  graph_label).hack->vertices;
      auto & gl_out = get_property(g_out, graph_label).hack->vertices;

      typename graph_traits < VertexListGraph >::vertex_iterator vi, vi_end;
      typename graph_traits < VertexListGraph >::vertex_iterator ui, ui_end;

      // copy vertices from g_in
      for (tie(vi, vi_end) = vertices(g_in); vi != vi_end; ++vi) {
        OutVertex new_v = add_vertex(g_out);
        copy_vertex(*vi, new_v);
        assert( g_out[new_v].name == g_in[*vi].name); // copy_vertex already did it
        gl_out[ g_in[*vi].name ] = new_v;
      }

      // create edges
      for (tie(ui, ui_end) = vertices(g_in); ui != ui_end; ++ui) {
        for (tie(vi, vi_end) = vertices(g_in); vi != vi_end; ++vi) {
          if ( (reflexive || ui != vi) && edge(*ui, *vi, g_in).second == false ) {
            typename graph_traits<MutableGraph>::edge_descriptor new_e;
            bool inserted;
            boost::tie(new_e, inserted) = add_edge(*ui, *vi, g_out);
            assert(inserted);
          }
        }
      }
    }
  }// namespace detail

  template <class VertexListGraph, class MutableGraph> 
  void graph_complement(const VertexListGraph& g_in, MutableGraph& g_out)
  {
    detail::graph_complement_impl(g_in, g_out, false);
  }

  template <class VertexListGraph, class MutableGraph> 
  void graph_reflexive_complement(const VertexListGraph& g_in, MutableGraph& g_out)
  {
    detail::graph_complement_impl(g_in, g_out, true);
  }

} // namespace boost

#endif // BOOST_GRAPH_COMPLEMENT_HPP
