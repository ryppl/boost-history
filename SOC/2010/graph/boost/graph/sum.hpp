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
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/named_function_params.hpp>

namespace boost {

  namespace detail {

    // FIXME: Make labeled_graph wrappers graph adaptors and pass gin and
    // gout as the template paraameter, not the wrapped type.
    template<typename LabelledInGraph,
             typename LabelledOutGraph,
             typename Copy,
             typename Merge>
    void union_vertices(LabelledInGraph const& gin,
                        LabelledOutGraph& gout,
                        Copy copy,
                        Merge merge)
    {
      typedef typename LabelledInGraph::graph_type InGraph;
      typedef typename LabelledInGraph::vertex_label_type Label;
      typedef typename graph_traits<InGraph>::vertex_descriptor InVertex;
      typedef typename LabelledOutGraph::graph_type OutGraph;
      typedef typename graph_traits<OutGraph>::vertex_descriptor OutVertex;

      // For convenience so I don't have to keep called ".graph" when passing
      // these graphs to other functions. I wouldn't have to do this if the
      // the labeled wrappers were actually graph adaptors.
      InGraph const& in = gin.graph();
      OutGraph& out = gout.graph();

      // Copy g1 in to go
      typename graph_traits<InGraph>::vertex_iterator vi, vi_end;
      for(tie(vi, vi_end) = vertices(in); vi != vi_end; ++vi) {
        InVertex v_in = *vi;
        Label const& label = gin.vertex_label(v_in);
        InVertex v_out = gout.vertex(label);

        // If the output graph did not contain this vertex, then copy it.
        // Otherwise, merge it.
        if(v_out == graph_traits<OutGraph>::null_vertex()) {
          OutVertex v_new = add_vertex(out);
          copy(v_in, in, v_new, out);
          gout.vertex_label(v_new, label);
        } else {
          merge(v_in, in, v_out, out);
        }
      }
    }

    template<typename LabelledInGraph,
             typename LabelledOutGraph,
             typename Copy,
             typename Merge>
    void union_edges(LabelledInGraph const& gin,
                     LabelledOutGraph& gout,
                     Copy copy,
                     Merge merge)
    {
      typedef typename LabelledInGraph::graph_type InGraph;
      typedef typename LabelledInGraph::edge_label_type EdgeLabel;
      typedef typename graph_traits<InGraph>::edge_descriptor InEdge;
      typedef typename LabelledOutGraph::graph_type OutGraph;
      typedef typename graph_traits<OutGraph>::vertex_descriptor OutVertex;
      typedef typename graph_traits<OutGraph>::edge_descriptor OutEdge;

      InGraph const& in = gin.graph();
      OutGraph& out = gout.graph();

      typename graph_traits<InGraph>::edge_iterator ei, ei_end;
      for(tie(ei, ei_end) = edges(in); ei != ei_end; ++ei) {
        InEdge e_in = *ei;
        EdgeLabel const& label = gin.edge_label(e_in);
        OutEdge e_out = gout.edge(label);

        // NOTE: There isn't an null_edge. However, a default constructed
        // edge descriptor should be invalid. If we can't rely on this, then
        // there really isn't a way that we can implement labeling since
        // we sometimes have to return invalid descriptors.

        // If no labeled edge exists in the output graph, then we have to
        // copy the edge. Otherwise, merge it.
        if(e_out == OutEdge()) {
          // Find the vertices in the outuput graph that correspond to the
          // input graph and insert the edge.
          OutVertex u = gout.vertex(gin.vertex_label(source(*ei, in)));
          OutVertex v = gout.vertex(gin.vertex_label(target(*ei, in)));
          OutEdge e_new = add_edge(u, v, out).first;
          copy(e_in, in, e_new, out);
        } else {
          merge(e_in, in, e_out, out);
        }
      }
    }

    template<typename LabelledInGraph,
             typename LabelledOutGraph,
             typename CopyVertex,
             typename MergeVertex,
             typename CopyEdge,
             typename MergeEdge>
    void graph_sum_impl(LabelledInGraph const& g1,
                        LabelledInGraph const& g2,
                        LabelledOutGraph& go,
                        CopyVertex copy_vertex,
                        MergeVertex merge_vertex,
                        CopyEdge copy_edge,
                        MergeEdge merge_edge)

    {
      union_vertices(g1, go, copy_vertex, merge_vertex);
      union_vertices(g2, go, copy_vertex, merge_vertex);
      union_edges(g1, go, copy_edge, merge_edge);
//       union_edges(g2, go, copy_edge, merge_edge);
    }


    template <typename VertexListGraph, typename MutableGraph,
              typename CopyVertex, typename MergeVertices, typename SetVertexLabel,
              typename CopyEdge, typename MergeEdges>
    void simple_graph_sum_impl(const VertexListGraph& g1,
                               const VertexListGraph& g2,
                               MutableGraph& g_out,
                               CopyVertex copy_vertex,
                               MergeVertices merge_vertices,
                               SetVertexLabel set_vertex_label,
                               CopyEdge copy_edge,
                               MergeEdges merge_edges)
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

        if ( ! (gl2.vertices.find( src  ) != gl2.vertices.end() &&
                gl2.vertices.find( targ ) != gl2.vertices.end() &&
                edge(gl2.vertices.find(src)->second, gl2.vertices.find(targ)->second, g2).second) ) { // if ei=(src,targ) is not in g2
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          copy_edge(*ei, g1, new_e, g_out);
        } else {
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          merge_edges(*ei, g1,
                      edge(gl2.vertices.find(src)->second, gl2.vertices.find(targ)->second, g2).first, g2,
                      new_e, g_out);
        }
      }
      // copy edges from g2 if it is *not* already there
      for (tie(ei, ei_end) = edges(g2); ei != ei_end; ++ei) {
        size_t src  = g2[source(*ei, g2)].name;
        size_t targ = g2[target(*ei, g2)].name;
        assert( gl_out.vertices.find(src)  != gl_out.vertices.end() );
        assert( gl_out.vertices.find(targ) != gl_out.vertices.end() );

        if ( ! (edge(gl_out.vertices[src], gl_out.vertices[targ], g_out).second) ) { // ei=(src,targ) is not yet in g_out
          boost::tie(new_e, inserted) = add_edge(gl_out.vertices[src], gl_out.vertices[targ], g_out);
          assert( inserted );
          copy_edge(*ei, g2, new_e, g_out);
        }
      }
    }
  } // namespace detail


  // NOTE: This function can only be used if InGraph and OutGraph realize the
  // default requiremens for labeling vertices and edges. Both graphs must
  // define the following properties:
  // - vertex_name
  // - edge_name
  // - graph_vertex_label
  // - graph_edge_label
  template <typename InGraph, typename OutGraph>
  void graph_sum(InGraph const& g1, InGraph const& g2, OutGraph& g_out)
  {
    function_requires< VertexAndEdgeListGraphConcept<InGraph> >();
    function_requires< MutableGraphConcept<OutGraph> >();

    typedef typename property_map<InGraph, vertex_name_t>::const_type InVertexNameMap;
    typedef typename property_map<InGraph, graph_named_vertices_t>::const_type InVertexMap;
    typedef typename property_map<InGraph, edge_name_t>::const_type InEdgeNameMap;
    typedef typename property_map<InGraph, graph_named_edges_t>::const_type InEdgeMap;
    function_requires< VertexLabeledDomainConcept<InGraph, InVertexNameMap, InVertexMap> >();
    function_requires< EdgeLabeledDomainConcept<InGraph, InEdgeNameMap, InEdgeMap> >();

    typedef typename property_map<OutGraph, vertex_name_t>::type OutVertexNameMap;
    typedef typename property_map<OutGraph, graph_named_vertices_t>::type OutVertexMap;
    typedef typename property_map<OutGraph, edge_name_t>::type OutEdgeNameMap;
    typedef typename property_map<OutGraph, graph_named_edges_t>::type OutEdgeMap;
    function_requires< VertexLabeledDomainConcept<OutGraph, OutVertexNameMap, OutVertexMap> >();
    function_requires< EdgeLabeledDomainConcept<OutGraph, OutEdgeNameMap, OutEdgeMap> >();

    typedef labeled_graph<InGraph, InVertexNameMap, InVertexMap, InEdgeNameMap, InEdgeMap> LabelledInGraph;
    typedef labeled_graph<OutGraph, OutVertexNameMap, OutVertexMap, OutEdgeNameMap, OutEdgeMap> LabelledOutGraph;


    LabelledInGraph gin1(g1,
                         get(vertex_name, g1), get(graph_named_vertices, g1),
                         get(edge_name, g1), get(graph_named_edges, g1));
    LabelledInGraph gin2(g2,
                         get(vertex_name, g2), get(graph_named_vertices, g2),
                         get(edge_name, g2), get(graph_named_edges, g2));
    LabelledOutGraph gout(g_out,
                          get(vertex_name, g_out), get(graph_named_vertices, g_out),
                          get(edge_name, g_out), get(graph_named_edges, g_out));

    detail::graph_sum_impl(gin1, gin2, gout,
                           detail::default_vertex_copy<InGraph, OutGraph>(),
                           detail::default_binary_vertex_merge<InGraph, OutGraph>(),
                           detail::default_edge_copy<InGraph, OutGraph>(),
                           detail::default_binary_edge_merge<InGraph, OutGraph>());
  }

  template <typename VertexListGraph, typename MutableGraph,
            typename P, typename T, typename R>
  void graph_sum(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                 const bgl_named_params<P, T, R>& params)
  {
    /*
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
      */
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

  // For simple graphs

  template <typename VertexListGraph, typename MutableGraph>
  void simple_graph_sum(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    detail::simple_graph_sum_impl
      (g1, g2, g_out,
       detail::default_vertex_copy<VertexListGraph, MutableGraph>(),
       detail::default_vertices_merge<VertexListGraph, MutableGraph>(),
       detail::default_set_vertex_label<MutableGraph>(),
       detail::default_edge_copy<VertexListGraph, MutableGraph>(),
       detail::default_edges_merge<VertexListGraph, MutableGraph>()
       );
  }

  template <typename VertexListGraph, typename MutableGraph,
            typename P, typename T, typename R>
  void simple_graph_sum(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                 const bgl_named_params<P, T, R>& params)
  {
    detail::simple_graph_sum_impl
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
                    detail::default_edges_merge<VertexListGraph, MutableGraph>())
       );
  }

  template <typename VertexListGraph>
  VertexListGraph simple_graph_sum(const VertexListGraph& g1, const VertexListGraph& g2)
  {
    VertexListGraph g_out;
    simple_graph_sum(g1, g2, g_out);
    return g_out;
  }

  template <typename VertexListGraph, typename P, typename T, typename R>
  VertexListGraph simple_graph_sum(const VertexListGraph& g1, const VertexListGraph& g2,
                 const bgl_named_params<P, T, R>& params)
  {
    VertexListGraph g_out;
    simple_graph_sum(g1, g2, g_out, params);
    return g_out;
  }

  // simple_graph_union is an alias to simple_graph_sum
  template <typename VertexListGraph, typename MutableGraph>
  inline void simple_graph_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out)
  {
    simple_graph_sum(g1, g2, g_out);
  }

  template <typename VertexListGraph, typename MutableGraph,
            typename P, typename T, typename R>
  inline void simple_graph_union(const VertexListGraph& g1, const VertexListGraph& g2, MutableGraph& g_out,
                          const bgl_named_params<P, T, R>& params)
  {
    simple_graph_sum(g1, g2, g_out, params);
  }

  template <typename VertexListGraph>
  inline VertexListGraph simple_graph_union(const VertexListGraph& g1, const VertexListGraph& g2)
  {
    return simple_graph_sum(g1, g2);
  }

  template <typename VertexListGraph, typename P, typename T, typename R>
  inline VertexListGraph simple_graph_union(const VertexListGraph& g1, const VertexListGraph& g2,
                                     const bgl_named_params<P, T, R>& params)
  {
    return simple_graph_sum(g1, g2, params);
  }
} // namespace boost

#endif // BOOST_GRAPH_SUM_HPP
