//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace boost;
using namespace std;

struct Vertex_Type1;
struct Edge_Type1;
struct Graph_Type1;

struct Vertex_Type2;
struct Graph_Type2;

typedef adjacency_list < vecS, vecS, bidirectionalS, Vertex_Type1, Edge_Type1,  Graph_Type1, listS > Graph1;
typedef adjacency_list < vecS, vecS, bidirectionalS, Vertex_Type2, no_property, Graph_Type2, listS > Graph2;

// Hack: Instead of using Graph definition here to define the
// descriptors, we define them using the same parameters used to
// define Graph1 and Graph2:
typedef adjacency_list_traits< vecS, vecS, bidirectionalS, listS>::vertex_descriptor Vertex;
typedef adjacency_list_traits< vecS, vecS, bidirectionalS, listS>::edge_descriptor Edge;

struct Vertex_Type1 {
  size_t name;
  string str;
  int id;
  int value;
};
struct Edge_Type1 {
  size_t name;
};
struct Graph_Type1 {
  unordered_map<size_t, Vertex> vertices;
  unordered_map<size_t, Edge> edges;
};

struct Vertex_Type2 {
  size_t name;
  string str;
  int id;
  float value;
};
struct Graph_Type2 {
  unordered_map<size_t, Vertex> vertices;
};


// Vertex copier
template <typename G1, typename G2>
struct my_vertex_copier {
  my_vertex_copier(int _add = 0, float _factor = 1)
    : add(_add), factor(_factor) { }

  template <typename Vertex1, typename Vertex2>
  void operator()(const Vertex1& v1, const G1& g1, Vertex2& v2, G2& g2) const {
    // put(get(vertex_all, g2), v2, get(get(vertex_all, g1), v1)); // Only works if the types are the same
    g2[v2].name  = g1[v1].name;
    g2[v2].str   = g1[v1].str;
    g2[v2].id    = g1[v1].id + add;
    g2[v2].value = g1[v1].value*factor;
  }
  int add;
  float factor;
};

// Vertex copier that sets the name mapping
// It is the same as detail::default_vertex_copy + detail::default_set_vertex_label
template <typename G1, typename G2>
struct my_vertex_copier_and_setter {
  my_vertex_copier_and_setter(int _add = 0, float _factor = 1) {
    copier = my_vertex_copier<G1,G2>(_add,_factor);
  }

  template <typename Vertex1, typename Vertex2>
  void operator()(const Vertex1& v1, const G1& g1, Vertex2& v2, G2& g2) const {
    copier(v1, g1, v2, g2); // does "g2[v2].name  = g1[v1].name;"
    get_property(g2).vertices[ g1[v1].name ] = v2;
  }

  my_vertex_copier<G1, G2> copier;
};

// Edge copier that sets the name mapping
// It is the same as detail::default_edge_copy + detail::default_set_edge_label
template <typename G1, typename G2>
struct my_edge_copier_and_setter {
  typedef typename graph_traits<G1>::edge_descriptor Edge1;
  typedef typename graph_traits<G2>::edge_descriptor Edge2;
  void operator()(const Edge1& e1, const G1& g1, Edge2& e2, G2& g2) const {
    put(get(edge_all, g2), e2, get(get(edge_all, g1), e1));
    get_property(g2).edges[ g1[e1].name ] = e2;
  }
};

// Vertex merger similar to my_vertex_copier
template <typename InGraph, typename OutGraph>
struct my_vertex_merger {
  my_vertex_merger(int _add = 0, float _factor = 1)
    : add(_add), factor(_factor) { }

  template <typename InVertex, typename OutVertex>
  void operator()(const InVertex&  v1, const InGraph& g1,
                  const InVertex&  v2, const InGraph& g2,
                  OutVertex& v_out, OutGraph& g_out) {
    g_out[v_out].name  = g1[v1].name;
    g_out[v_out].str   = g1[v1].str + "+" + g2[v2].str;
    g_out[v_out].id    = g1[v1].id + add;
    g_out[v_out].value = g1[v1].value*factor;
  }
  int add;
  float factor;
};



// edge visitor for new edges
// set a name for the new edge
template <typename Graph>
struct my_edge_visitor {
  typedef typename graph_traits<Graph>::edge_descriptor Edge;
  void operator()(const Edge &e, Graph &g)
  {
    typename graph_traits<Graph>::vertex_descriptor
      u = source(e, g), v = target(e, g);
    g[e].name = g[u].name * 100 + g[v].name;
    get_property(g).edges[ g[e].name ] = e;
  }
};


// name vertices
template <class Graph>
void auto_name_vertices(Graph &g, string graph_name, size_t start_vertex_name = 10, int value_offset = 0) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  int id = 0; // id == vertex index
  size_t name = start_vertex_name; // just to make vertex name != vertex index
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    stringstream tmp;
    tmp << id;
    g[*vi].str = graph_name + tmp.str();
    g[*vi].id = id;
    g[*vi].value = id*id + value_offset;
    g[*vi].name = name;
    get_property(g).vertices[name] = *vi;
    name++;
    id++;
  }
}

// name edges
template <class Graph>
void auto_name_edges(Graph &g, size_t delta_edge_name = 0) {
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  size_t name;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    size_t src = g[source(*ei, g)].name, targ = g[target(*ei, g)].name;
    name = delta_edge_name + 100 * src + targ;
    g[*ei].name = name;
    get_property(g).edges[name] = *ei;
  }
}

// check to see if the naming is consistent
template <class Graph>
void check_vertex_naming(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    assert( get_property(g).vertices[ g[*vi].name ] == *vi);
}

// check to see if the naming is consistent
template <class Graph>
void check_vertex_and_edge_naming(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    assert( get_property(g).vertices[ g[*vi].name ] == *vi);
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    assert( get_property(g).edges[ g[*ei].name ] == *ei);
}

// print a graph showing vertex and edge names
template <class Graph>
void print_named_edges(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
  typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    cout << "Vertex " << index[*vi]
         << " [name: " << g[*vi].name << ", id: " << g[*vi].id
         << ", str: " << g[*vi].str << ", value: " << g[*vi].value << "] -->";
    for (tie(out_i, out_end) = out_edges(*vi, g); out_i != out_end; ++out_i) {
      //      cout << " " << index[target(*out_i, g)] << " (edge " << *out_i << ": " << g[*out_i].name << ");";
      cout << " [" << index[target(*out_i, g)] << ", name: " << g[*out_i].name << "];";
    }
    cout << endl;
  }
}

// print a graph showing only vertex names
template <class Graph>
void print(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
  typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    cout << "Vertex " << index[*vi]
         << " [name: " << g[*vi].name << ", id: " << g[*vi].id
         << ", str: " << g[*vi].str << ", value: " << g[*vi].value << "] -->";
    for (tie(out_i, out_end) = out_edges(*vi, g); out_i != out_end; ++out_i) {
      cout << " " << index[target(*out_i, g)];
    }
    cout << endl;
  }
}
