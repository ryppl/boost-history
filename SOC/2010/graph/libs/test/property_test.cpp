//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/sum.hpp>
#include <boost/graph/difference.hpp>
#include <boost/graph/intersection.hpp>
#include <boost/graph/disjoint_union.hpp>
#include <boost/graph/complement.hpp>
#include <boost/graph/join.hpp>

using namespace boost;
using namespace std;

struct Vertex_Label;
struct Edge_Label;
struct Graph_Label;

typedef adjacency_list < vecS, vecS, bidirectionalS, Vertex_Label, Edge_Label, Graph_Label, listS > Graph;

// Hack: Instead of using Graph definition here to define the
// descriptors, we define them using the same parameters used to
// define Graph:
typedef adjacency_list_traits< vecS, vecS, bidirectionalS, listS>::vertex_descriptor Vertex;
typedef adjacency_list_traits< vecS, vecS, bidirectionalS, listS>::edge_descriptor Edge;

struct Vertex_Label {
  size_t name;
};
struct Edge_Label {
  size_t name;
};
struct Graph_Label {
  unordered_map<size_t, Vertex> vertices;
  unordered_map<size_t, Edge> edges;
};

// First look at main()
// These are auxiliary functions

// Vertex copier that sets the name mapping
template <typename G1, typename G2>
struct my_vertex_copier {
  typedef typename graph_traits<G1>::vertex_descriptor Vertex1;
  typedef typename graph_traits<G2>::vertex_descriptor Vertex2;
  void operator()(const Vertex1& v1, const G1& g1, Vertex2& v2, G2& g2) const {
    put(get(vertex_all, g2), v2, get(get(vertex_all, g1), v1));
    get_property(g2).vertices[ g1[v1].name ] = v2;
  }
};

// Edge copier that sets the name mapping
template <typename G1, typename G2>
struct my_edge_copier {
  typedef typename graph_traits<G1>::edge_descriptor Edge1;
  typedef typename graph_traits<G2>::edge_descriptor Edge2;
  void operator()(const Edge1& e1, const G1& g1, Edge2& e2, G2& g2) const {
    put(get(edge_all, g2), e2, get(get(edge_all, g1), e1));
    get_property(g2).edges[ g1[e1].name ] = e2;
  }
};

// edge visitor for new edges
template <typename EdgeDescriptor, typename Graph>
struct my_edge_visitor {
  void operator()(const EdgeDescriptor &e, Graph &g)
  {
    typename graph_traits<Graph>::vertex_descriptor
      u = source(e, g), v = target(e, g);
    g[e].name = g[u].name * 100 + g[v].name;
    get_property(g).edges[ g[e].name ] = e;
  }
};

// name vertices and edges
template <class Graph>
void auto_label(Graph &g, size_t start_vertex_label = 10, size_t delta_edge_label = 0) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  size_t label = start_vertex_label; // just to make vertex name != vertex index
  // vertices
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    g[*vi].name = label;
    get_property(g).vertices[label] = *vi;
    label++;
  }
  // edges (does not work for parallel edges - they will have the same name)
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    size_t src = g[source(*ei, g)].name, targ = g[target(*ei, g)].name;
    label = delta_edge_label + 100 * src + targ;
    g[*ei].name = label;
    get_property(g).edges[label] = *ei;
  }
}

// check to see if the naming is ok
template <class Graph>
void check(Graph &g, bool check_edges = true) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    assert( get_property(g).vertices[ g[*vi].name ] == *vi);
  if ( check_edges )
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
      assert( get_property(g).edges[ g[*ei].name ] == *ei);
}

// print a graph showing vertex and edge names
template <class Graph>
void print(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
  typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    cout << "Vertex " << index[*vi] << " [name: " << g[*vi].name << "] -->";
    for (tie(out_i, out_end) = out_edges(*vi, g); out_i != out_end; ++out_i) {
      cout << " " << index[target(*out_i, g)] << " (edge " << *out_i << ": " << g[*out_i].name << ");";
    }
    cout << endl;
  }
}

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph g1, g2, g_simple_compl, g_compl, g_rcompl, g_int, g_sum, g_diff, g_sym_diff, g_union, g_join, g_join2;

  generate_random_graph(g1, 3,  5, gen, false);
  generate_random_graph(g2, 4, 10, gen, false);

  auto_label(g1);
  auto_label(g2);

  cout << "Graph 1 (g1):" << endl;
  check(g1);
  print(g1);
  cout << endl;

  cout << "Graph 2 (g2):" << endl;
  check(g2);
  print(g2);
  cout << endl;

  cout << "Complement of g1:" << endl;
  graph_complement(g1, g_simple_compl); // ignore name mapping (but copy vertex properties)
  // check(g_compl); // graph_complement don't set graph_label
  print(g_simple_compl);
  cout << endl;

  cout << "Complement of g1:" << endl;
  my_vertex_copier<Graph, Graph> c;
  g_compl = graph_complement(g1, vertex_copy(c));
  check(g_compl, false); // graph_complement don't set edge names in graph_label, but my_vertex_copier do it for vertices
  print(g_compl);
  cout << endl;

  cout << "Reflexive complement of g1:" << endl;
  graph_reflexive_complement(g1, g_rcompl, vertex_copy(c).edge_visitor(my_edge_visitor<Edge, Graph>()));
  check(g_rcompl);
  print(g_rcompl);
  cout << endl;

  cout << "Intersection of g1 and g2:" << endl;
  graph_intersection(g1, g2, g_int);
  check(g_int);
  print(g_int);
  cout << endl;

  cout << "Sum of g1 and g2:" << endl;
  graph_sum(g1, g2, g_sum);
  check(g_sum);
  print(g_sum);
  cout << endl;

  cout << "Difference of g1 and g2:" << endl;
  graph_difference(g1, g2, g_diff);
  check(g_diff);
  print(g_diff);
  cout << endl;

  cout << "Vertex Symmetric Difference of g1 and g2:" << endl;
  graph_vertex_symmetric_difference(g1, g2, g_sym_diff);
  check(g_sym_diff);
  print(g_sym_diff);
  cout << endl;

  // For union and join, the vertex and edge set are considered to be disjoint.
  // They ignore the vertex and edge names

  cout << "Disjoint union of g1 and g2:" << endl;
  graph_disjoint_union(g1, g2, g_union);
  print(g_union);
  cout << endl;

  cout << "Join of g1 and g2:" << endl;
  graph_join(g1, g2, g_join);
  print(g_join);
  cout << endl;

  cout << "Another join of g1 and g2: (g2 with new labels)" << endl;
  auto_label(g2, 20, 200);
  graph_join(g1, g2, g_join2,
             vertex_copy(my_vertex_copier<Graph, Graph>())
             .edge_copy(my_edge_copier<Graph, Graph>())
             .edge_visitor(my_edge_visitor<Edge, Graph>()));
  check(g_join2); // passes the test because we are using our own functions
  print(g_join2);
  //  cout << endl;

  return EXIT_SUCCESS;
}
