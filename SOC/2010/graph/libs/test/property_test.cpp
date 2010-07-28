#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/intersection.hpp>

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

template <class Graph>
void auto_label(Graph &g) { // to name vertices and edges
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
    g[*ei].name = 100 + 10 * src + targ;
  }
}

template <class Graph>
void check(Graph &g) { // simple check to see if the naming is ok
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    assert(get_property(g, graph_label).hack->vertices[g[*vi].name] == *vi);
}

template <class Graph>
void print(Graph &g) { // print a graph showing vertex and edge names
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

  Graph g1, g2, g_out;

  get_property(g1, graph_label).hack = new(Graph_Label);
  get_property(g2, graph_label).hack = new(Graph_Label);
  get_property(g_out, graph_label).hack = new(Graph_Label);

  generate_random_graph(g1, 5, 10, gen, false);
  generate_random_graph(g2, 6, 16, gen, false);

  auto_label(g1);
  auto_label(g2);

  check(g1);
  check(g2);

  cout << "Graph 1:" << endl;
  print(g1);
  cout << "Graph 2:" << endl;
  print(g2);

  cout << "Intersection of g1 and g2:" << endl;
  intersec(g1, g2, g_out);
  print(g_out);

  return EXIT_SUCCESS;
}
