//=======================================================================
// Copyright (c) 2010 Texas A&M University
// Authors: Andrew Sutton
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef BOOST_LABELED_GRAPH
#define BOOST_LABELED_GRAPH

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

// FIXME: Rewrite these classes as legitimate graph adaptors.

namespace boost {

/**
 * A vertex_labeled_graph is a lightweight wrapper on a graph object, a
 * name property map (mapping from vertex to a name), and a vertex property
 * map (mapping from a name to a vertex).
 */
template<typename Graph, typename NameMap, typename VertexMap>
class vertex_labeled_graph
{
public:
  typedef Graph graph_type;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename property_traits<NameMap>::value_type vertex_label_type;

  vertex_labeled_graph(Graph& g, NameMap names, VertexMap vertices)
    : _graph(g), _names(names), _vertices(vertices)
  { }

  vertex_descriptor vertex(vertex_label_type const& l) const
  { return get(_vertices, l); }

  vertex_label_type const& vertex_label(vertex_descriptor v) const
  { return get(_names, v); }

  void vertex_label(vertex_descriptor v, vertex_label_type const& l)
  {
    put(_names, v, l);
    put(_vertices, l, v);
  }

  graph_type& graph()
  { return _graph; }

  graph_type const& graph() const
  { return _graph; }

private:
  Graph&    _graph;
  NameMap   _names;
  VertexMap _vertices;
};

/**
 * An edge_labeled_graph is a lightweight wrapper on a graph object, a
 * name property map (mapping from an edge to a name), and an edge property
 * map (mapping from a name to an edge).
 */
template<typename Graph, typename NameMap, typename EdgeMap>
class edge_labeled_graph
{
public:
  typedef Graph graph_type;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename property_traits<NameMap>::value_type edge_label_type;

  edge_labeled_graph(Graph& g, NameMap names, EdgeMap edges)
    : _graph(g), _names(names), _edges(edges)
  { }

  edge_descriptor edge(edge_label_type const& l) const
  { return get(_edges, l); }

  edge_label_type const& edge_label(edge_descriptor e) const
  { return get(_names, e); }

  void edge_label(edge_descriptor v, edge_label_type const& l)
  {
    put(_names, v, l);
    put(_edges, l, v);
  }

  graph_type& graph()
  { return _graph; }

  graph_type const& graph() const
  { return _graph; }

private:
  Graph&    _graph;
  NameMap   _names;
  EdgeMap   _edges;
};

/**
 * A labeled graph is both vertex and edge-labeled.
 */
template<typename Graph,
         typename VertexNameMap,
         typename VertexMap,
         typename EdgeNameMap,
         typename EdgeMap>
class labeled_graph
{
public:
  typedef Graph graph_type;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename property_traits<VertexNameMap>::value_type vertex_label_type;
  typedef typename property_traits<EdgeNameMap>::value_type edge_label_type;

  labeled_graph(Graph const& g,
                VertexNameMap vnames, VertexMap vertices,
                EdgeNameMap enames, EdgeMap edges)
    : _graph(const_cast<Graph&>(g))
    , _vertex_names(vnames), _vertices(vertices)
    , _edge_names(enames), _edges(edges)
  { }

  // Vertex/Vertex label operations
  vertex_descriptor vertex(vertex_label_type const& l) const
  { return get(_vertices, l); }

  vertex_label_type const& vertex_label(vertex_descriptor v) const
  { return get(_vertex_names, v); }

  void vertex_label(vertex_descriptor v, vertex_label_type const& l)
  {
    put(_vertex_names, v, l);
    put(_vertices, l, v);
  }

  // Edge, edge label operations
  edge_descriptor edge(edge_label_type const& l) const
  { return get(_edges, l); }

  edge_label_type const& edge_label(edge_descriptor v) const
  { return get(_edge_names, v); }

  void edge_label(edge_descriptor v, edge_label_type const& l)
  {
    put(_edge_names, v, l);
    put(_vertices, l, v);
  }

  graph_type& graph()
  { return _graph; }

  graph_type const& graph() const
  { return _graph; }

private:
  Graph&          _graph;
  VertexNameMap   _vertex_names;
  VertexMap       _vertices;
  EdgeNameMap     _edge_names;
  EdgeMap         _edges;
};

// FIXME: This will need to be updated when labeled graph becomes a more
// viable graph adaptor.
template<typename Graph, typename NameMap, typename VertexMap>
struct VertexLabeledDomainConcept {
  static void constraints() {
    function_requires< GraphConcept<Graph> >();
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

    // NOTE: There is an implied convertability between the value_type of
    // the NameMap and the key_type of the VertexMap. If the name map is
    // invalid (i.e., has value_type error_property_not_found), then the
    // concept will fail, which is the correct behavior.
    function_requires< ReadWritePropertyMapConcept<NameMap, Vertex> >();
    typedef typename property_traits<NameMap>::value_type Label;
    function_requires< ReadWritePropertyMapConcept<VertexMap, Label> >();
  }
  Graph g;
  NameMap names;
  VertexMap verts;
};

// FIXME: As with above, we'll introduce more requirements as labeled_graph
// becomes more mature.
template<typename Graph, typename NameMap, typename EdgeMap>
struct EdgeLabeledDomainConcept {
  static void constraints() {
    function_requires< GraphConcept<Graph> >();
    typedef typename graph_traits<Graph>::edge_descriptor Edge;

    // NOTE: There is an implied convertability between the value_type of
    // the NameMap and the key_type of the Edge. If the name map is
    // invalid (i.e., has value_type error_property_not_found), then the
    // concept will fail, which is the correct behavior.
    function_requires< ReadWritePropertyMapConcept<NameMap, Edge> >();
    typedef typename property_traits<NameMap>::value_type Label;
    function_requires< ReadWritePropertyMapConcept<EdgeMap, Label> >();
  }
  Graph g;
  NameMap names;
  EdgeMap edges;
};

} // namespace boost

#endif
