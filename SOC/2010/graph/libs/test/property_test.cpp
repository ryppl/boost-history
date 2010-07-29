#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/intersection.hpp>
#include <boost/graph/sum.hpp>
#include <boost/graph/union.hpp>

using namespace boost;
using namespace std;

struct Vertex_Label;
struct Edge_Label;
struct Graph_Label;

struct Graph_Label_Hack {
  struct Graph_Label * hack;
};

typedef adjacency_list < vecS, vecS, bidirectionalS, Vertex_Label, Edge_Label, property<graph_label_t, Graph_Label_Hack> > Graph;

typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;

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

// name vertices and edges
template <class Graph>
void auto_label(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  size_t label = 10; // just to make vertex name != vertex index
  // vertices
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    g[*vi].name = label;
    get_property(g, graph_label).hack->vertices[label] = *vi;
    label++;
  }
  // edges (does not work for parallel edges - they will have the same name)
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    size_t src, targ;
    src  = g[source(*ei, g)].name - 10;
    targ = g[target(*ei, g)].name - 10;
    label = 100 + 10 * src + targ;
    g[*ei].name = label;
    get_property(g, graph_label).hack->edges[label] = *ei;
  }
}


// rename vertices and edges (only because of union! To make the sets disjoint)
template <class Graph>
void relabel(Graph &g, size_t delta_v, size_t delta_e) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  // vertices
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    get_property(g, graph_label).hack->vertices.erase( g[*vi].name );
    g[*vi].name += delta_v;
    get_property(g, graph_label).hack->vertices[ g[*vi].name ] = *vi;
  }
  // edges (does not work for parallel edges - they will have the same name)
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    get_property(g, graph_label).hack->edges.erase( g[*ei].name );
    g[*ei].name += delta_e;
    get_property(g, graph_label).hack->edges[ g[*ei].name ] = *ei;
  }
}


// check to see if the naming is ok
template <class Graph>
void check(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    assert( get_property(g, graph_label).hack->vertices[ g[*vi].name ] == *vi);
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    assert( get_property(g, graph_label).hack->edges[ g[*ei].name ] == *ei);
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

  Graph g1, g2, g_int, g_sum, g_union;

  get_property(g1, graph_label).hack = new(Graph_Label);
  get_property(g2, graph_label).hack = new(Graph_Label);
  get_property(g_int,   graph_label).hack = new(Graph_Label);
  get_property(g_sum,   graph_label).hack = new(Graph_Label);
  get_property(g_union, graph_label).hack = new(Graph_Label);

  generate_random_graph(g1, 4,  8, gen, false);
  generate_random_graph(g2, 5, 13, gen, false);

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

  // for union, the vertex and edge set needs to be disjoint.
  relabel(g2, 80, 800);
  cout << "Graph 2 with new names (g2'):" << endl;
  check(g2);
  print(g2);
  cout << endl;

  cout << "Union of g1 and g2':" << endl;
  graph_union(g1, g2, g_union);
  check(g_union);
  print(g_union);
  // cout << endl;

  return EXIT_SUCCESS;
}
