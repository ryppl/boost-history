//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_DEFAULT_HPP
#define BOOST_GRAPH_DEFAULT_HPP

#include <utility>
#include <boost/graph/graph_traits.hpp>

namespace boost {
  namespace detail {
    // To visit brand new edges. Default behavior: it does nothing
    template <typename Graph>
    struct default_edge_visitor {
      typedef typename graph_traits<Graph>::edge_descriptor EdgeDescriptor;
      void operator()(const EdgeDescriptor &, Graph &) const { }
    };

    // To set labels
    template <typename Graph>
    struct default_set_vertex_label {
      typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
      void operator()(size_t label, Vertex& v, Graph& g) const {
        g[v].name = label;
        get_property(g).vertices[ label ] = v;
      }
    };

    template <typename Graph>
    struct default_set_edge_label {
      typedef typename graph_traits<Graph>::edge_descriptor Edge;
      void operator()(size_t label, Edge& e, Graph& g) const {
        g[e].name = label;
        get_property(g).edges[ label ] = e;
      }
    };

    // Merge two vertices or two edges that are identified as the same (using the label)
    // Default behavior is to copy the properties from the first input, ignoring the second
    template <typename InGraph, typename OutGraph>
    struct default_vertices_merge {
      typedef typename graph_traits<InGraph>::vertex_descriptor InVertex;
      typedef typename graph_traits<OutGraph>::vertex_descriptor OutVertex;
      void operator()(const InVertex& v1, const InGraph&g1,
                      const InVertex& v2, const InGraph&g2,
                      OutVertex& v_out, OutGraph& g_out) const
      {
        put(get(vertex_all, g_out), v_out, get(get(vertex_all, g1), v1));
      }
    };

    template <typename InGraph, typename OutGraph>
    struct default_edges_merge {
      typedef typename graph_traits<InGraph>::edge_descriptor InEdge;
      typedef typename graph_traits<OutGraph>::edge_descriptor OutEdge;
      void operator()(const InEdge& e1, const InGraph&g1,
                      const InEdge& e2, const InGraph&g2,
                      OutEdge& e_out, OutGraph& g_out) const
      {
        put(get(edge_all, g_out), e_out, get(get(edge_all, g1), e1));
      }
    };

    // To copy vertex and edge properties. Similar to code in copy.hpp,
    // but here operator() receives both graphs as parameters
    template <typename InGraph, typename OutGraph>
    struct default_vertex_copy {
      typedef typename graph_traits<InGraph>::vertex_descriptor InVertex;
      typedef typename graph_traits<OutGraph>::vertex_descriptor OutVertex;
      void operator()(const InVertex& v_in, const InGraph&g_in,
                      OutVertex& v_out, OutGraph& g_out) const
      {
        put(get(vertex_all, g_out), v_out, get(get(vertex_all, g_in), v_in));
      }
    };

    template <typename InGraph, typename OutGraph>
    struct default_edge_copy {
      typedef typename graph_traits<InGraph>::edge_descriptor InEdge;
      typedef typename graph_traits<OutGraph>::edge_descriptor OutEdge;
      void operator()(const InEdge& e_in, const InGraph&g_in,
                      OutEdge& e_out, OutGraph& g_out) const
      {
        put(get(edge_all, g_out), e_out, get(get(edge_all, g_in), e_in));
      }
    };

    // Wraps the copier interface taking four arguments (including
    // both graphs - as defined above) to the interface used by
    // copy.hpp (taking only two arguments).

    template <typename GraphIn, typename GraphOut, typename Copier>
    struct wrap_vertex_copier {
      wrap_vertex_copier(const GraphIn& g_in, GraphOut& g_out, Copier copier)
        : g_in(g_in), g_out(g_out), copier(copier) { }

      template <typename VertexIn, typename VertexOut>
      inline void operator()(const VertexIn& v_in, VertexOut& v_out) {
        copier(v_in, g_in, v_out, g_out);
      }
    private:
      const GraphIn& g_in;
      GraphOut& g_out;
      Copier copier;
    };

    template <typename Copier, typename GraphIn, typename GraphOut>
    inline typename detail::wrap_vertex_copier<GraphIn, GraphOut, Copier>
    make_vertex_copier_wrapper(const GraphIn& g1, GraphOut& g2, Copier c)
    {
      return detail::wrap_vertex_copier<GraphIn, GraphOut, Copier> (g1, g2, c);
    }

    template <typename GraphIn, typename GraphOut, typename Copier>
    struct wrap_edge_copier {
      wrap_edge_copier(const GraphIn& g_in, GraphOut& g_out, Copier copier)
        : g_in(g_in), g_out(g_out), copier(copier) { }

      template <typename EdgeIn, typename EdgeOut>
      inline void operator()(const EdgeIn& e_in, EdgeOut& e_out) {
        copier(e_in, g_in, e_out, g_out);
      }
    private:
      const GraphIn& g_in;
      GraphOut& g_out;
      Copier copier;
    };

    template <typename Copier, typename GraphIn, typename GraphOut>
    inline typename detail::wrap_edge_copier<GraphIn, GraphOut, Copier>
    make_edge_copier_wrapper(const GraphIn& g1, GraphOut& g2, Copier c)
    {
      return detail::wrap_edge_copier<GraphIn, GraphOut, Copier> (g1, g2, c);
    }
  } // namespace detail
} // namespace boost

#endif // BOOST_GRAPH_DEFAULT_HPP
