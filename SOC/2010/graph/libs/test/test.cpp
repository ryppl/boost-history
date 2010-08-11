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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/global_vertex_mapping.hpp>

#include <boost/graph/union.hpp>
#include <boost/graph/sum.hpp>
#include <boost/graph/intersection.hpp>
#include <boost/graph/difference.hpp>


using namespace boost;
using namespace std;

// create a typedef for the Graph type
typedef adjacency_list < vecS, vecS, bidirectionalS, property < vertex_name_t, char > > Graph;
typedef pair<int, int> Edge;

/*******************************************
 * Just playing with BGL in this beginning
 */

template <class Graph> struct exercise_vertex {
  exercise_vertex(Graph& g_) : g(g_) {}
  typedef graph_traits<Graph> GraphTraits;
  typedef typename GraphTraits::vertex_descriptor Vertex;

  void operator()(const Vertex& v) const
  {
    typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
    typename GraphTraits::out_edge_iterator out_i, out_end;
    typename GraphTraits::in_edge_iterator in_i, in_end;
    typename GraphTraits::adjacency_iterator ai, ai_end;
    typename GraphTraits::edge_descriptor e;

    cout << "vertex " << index[v] << "" << endl;
    cout << " out-edges: ";
    for (tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i) {
      e = *out_i;
      Vertex src = source(e, g), targ = target(e, g);
      cout << "(" << index[src] << "," << index[targ] << ") ";
    }
    cout << endl;

    cout << " in-edges: ";
    for (tie(in_i, in_end) = in_edges(v,g); in_i != in_end; ++in_i) {
      e = *in_i;
      Vertex src = source(e, g), targ = target(e, g);
      cout << "(" << index[src] << "," << index[targ] << ") ";
    }
    cout << endl;

    cout << " adjacent vertices: ";
    for (tie(ai, ai_end) = adjacent_vertices(v, g); ai != ai_end; ++ai)
      cout << index[*ai] <<  " ";
    cout << endl;


  }

  //...
  Graph& g;
};


void prt_vertices(Graph g) {
  // get the property map for vertex indices
  property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
  graph_traits<Graph>::vertex_iterator vi, vi_end;

  cout << "vertices = ";
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    cout << index[*vi] <<  " ";
  cout << endl;
}

void prt_edges(Graph g) {
  // get the property map for vertex indices
  property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
  graph_traits<Graph>::edge_iterator ei, ei_end;

  cout << "edges = ";
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    cout << "(" << index[source(*ei, g)]
         << "," << index[target(*ei, g)] << ") ";
  cout << endl;
}

void explore(Graph g) {
  exercise_vertex<Graph> blerg(g);

  for_each(vertices(g).first, vertices(g).second,
           exercise_vertex<Graph>(g));

  graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    blerg(*vi);
}

void prt(Graph g) {
  property_map < Graph, vertex_name_t >::type name_map = get(vertex_name, g);
  print_graph(g, name_map);
  prt_vertices(g);
  prt_edges(g);
}




/*******************************************
 * To create graphs...
 */

// if d == 1 creates a cycle
Graph create_graph(int n, int d)
{
  // declare a graph object
  Graph g(n);
  char name = 'a';
  property_map < Graph, vertex_name_t >::type name_map = get(vertex_name, g);
  graph_traits < Graph >::vertex_iterator vi, vi_end;

  // add the edges to the graph object
  for (int i = 0; i < n; ++i) {
    add_edge(i, (i-d+n)%n, g);
    add_edge(i, (i+d)%n, g);
  }

  for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++, name++)
    name_map[*vi] = name;

  return g;
}

//void add_source(char bla, Graph &g, char name)
void to_source(Graph &g, size_t v, char name)
{
  property_map < Graph, vertex_name_t >::type name_map = get(vertex_name, g);
  graph_traits < Graph >::vertex_iterator vi, vi_end;
  property_map < Graph, vertex_index_t >::type index = get(vertex_index, g);

  //  int v = add_vertex(g);
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++)
    if (v != *vi)
      add_edge(v, *vi, g);

  //  name_map[* --vertices(g).second] = name;
  name_map[v] = name;
}

void to_sink(Graph &g, size_t v, char name)
{
  property_map < Graph, vertex_name_t >::type name_map = get(vertex_name, g);
  graph_traits < Graph >::vertex_iterator vi, vi_end;
  property_map < Graph, vertex_index_t >::type index = get(vertex_index, g);

  //  int v = add_vertex(g);
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++)
    if (v != *vi)
      add_edge(*vi, v, g);

  //  name_map[* --vertices(g).second] = name;
  name_map[v] = name;
}



int main(int,char*[])
{
  Graph h1 = create_graph(5,1);
  Graph h2 = create_graph(5,2);
  graph_traits < Graph >::vertex_descriptor w, z;

  to_source(h1, 5, 'x');
  to_sink(h1, 5, 'x');

  to_sink(h2, 5, 'y');
  // x and y are the "same"

  w = add_vertex(h1); // vertex 6 (w) is only in h1
  add_edge(*vertices(h1).first, w, h1);
  add_edge(w, *vertices(h1).first, h1);
  get(vertex_name, h1)[w] = 'w';

  z = add_vertex(h2); // vertex 6 (w) is only in h1
  add_edge(*vertices(h2).first, w, h2);
  add_edge(w, *vertices(h2).first, h2);
  get(vertex_name, h2)[z] = 'z';

  cout << "graph h1:" << endl;
  prt(h1);
  cout << "graph h2:" << endl;
  prt(h2);

  Graph h_du;
  cout << "Disjoint union: (h1 union h2)" << endl;
  graph_disjoint_union(h1, h2, h_du);
  prt(h_du);
  // is there any way to clear h_du and use the same variable for all?

  GlobalVertexMapping < Graph, graph_traits<Graph>::vertex_descriptor, string > globalId(*vertices(h1).second, -2);

  globalId.add_graph(h1,"h1");
  globalId.add_graph(h2,"h2");

  globalId.associate(h1, w, 20);
  globalId.associate(h2, z, 40);

  graph_traits < Graph >::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(h1); vi != vi_end; vi++)
    globalId.associate(h1, *vi, *vi);
  for (tie(vi, vi_end) = vertices(h2); vi != vi_end; vi++)
    globalId.associate(h2, *vi, *vi);

  cout << "Global id:" << endl;
  globalId.show_associations();

  // in this example (graph sum), it creates some parallel edges (e.g., a --> x appears twice)
  // because x and y are the considered as the same
  Graph h_s;
  cout << "Graph sum: (h1 + h2)" << endl;
  gvm_graph_sum(h1, h2, globalId, h_s);
  prt(h_s);

  Graph h_i;
  cout << "Graph intersection: (h1 intersection h2)" << endl;
  gvm_graph_intersection(h1, h2, globalId, h_i);
  prt(h_i);

  Graph h_diff;
  cout << "Graph difference: (h1 - h2)" << endl;
  gvm_graph_difference(h1, h2, globalId, h_diff);
  prt(h_diff);

  return 0;
}
