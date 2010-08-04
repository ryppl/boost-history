#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/union.hpp>

using namespace boost;
using namespace std;

struct VertexType1
{
  string name;
  int id;
  int value;
};

struct VertexType2
{
  string name;
  int id;
  float value;
};

typedef adjacency_list < vecS, vecS, bidirectionalS, VertexType1 > Graph1;
typedef adjacency_list < vecS, vecS, bidirectionalS, VertexType2 > Graph2;

typedef Graph1::vertex_descriptor Vertex1;
typedef Graph2::vertex_descriptor Vertex2;
typedef graph_traits<Graph1>::vertex_iterator VertexIterator1;
typedef graph_traits<Graph2>::vertex_iterator VertexIterator2;

template <typename G1, typename G2>
struct vertex_copier {
  vertex_copier(const G1& g1, G2& g2)
    : vertex_all_map1(get(vertex_all, g1)), 
      vertex_all_map2(get(vertex_all, g2)) { }
  
  template <typename Vertex1, typename Vertex2>
  void operator()(const Vertex1& v1, Vertex2& v2) const {
    //    cout << "Copying vertex " << v1 << endl;
    put(vertex_all_map2, v2, get(vertex_all_map1, v1));
  }
  typename property_map<G1, vertex_all_t>::const_type vertex_all_map1;
  mutable typename property_map<G2, vertex_all_t>::type vertex_all_map2;
};


template <typename G1, typename G2>
struct mycopier {
  mycopier(const G1& _g1, G2& _g2, int _add, float _factor)
    : g1(_g1), g2(_g2), add(_add), factor(_factor) { }
  
  template <typename Vertex1, typename Vertex2>
  void operator()(const Vertex1& v1, Vertex2& v2) const {
    g2[v2].name = g1[v1].name;
    g2[v2].id = g1[v1].id + add;
    g2[v2].value = g1[v1].value*factor;
  }
  const G1& g1;
  G2& g2;
  int add;
  float factor;
};



// print a graph showing vertex and edge names
template <class Graph>
void print(Graph &g) {
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
  typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    cout << "Vertex " << index[*vi] << " [name: " << g[*vi].name << ", id: " << g[*vi].id << ", value: " << g[*vi].value << "] -->";
    for (tie(out_i, out_end) = out_edges(*vi, g); out_i != out_end; ++out_i) {
      cout << " " << index[target(*out_i, g)];
    }
    cout << endl;
  }
}

template <class Graph>
void auto_label(Graph &g, string name, int value_offset)
{
  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  int id = 0;

  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    stringstream tmp;
    tmp << id;
    g[*vi].name = name + tmp.str();
    g[*vi].id = id;
    g[*vi].value = id*id + value_offset;
    id++;
  }
}


int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph1 g1, g2, g3, g4;
  Graph2 h1, h2, h3;

  generate_random_graph(g1, 3,  3, gen, false);
  generate_random_graph(g2, 4,  6, gen, false);

  auto_label(g1, "g1_", 100);
  auto_label(g2, "g2_", 200);

  cout << "g1:" << endl;
  print(g1);
  cout << "g2:" << endl;
  print(g2);

  graph_disjoint_union(g1, g2, g3);
  cout << "g3:" << endl;
  print(g3);

  vertex_copier<Graph1, Graph1> c1(g1, g4), c2(g2, g4);
  graph_disjoint_union(g1, g2, g4, vertex_copy(make_pair(c1, c2)));
  cout << "g4:" << endl;
  print(g4);

  mycopier<Graph1, Graph2> cc1(g1, h1, 10, -2.1111), cc2(g2, h1, 100, 2);
  graph_disjoint_union(g1, g2, h1, vertex_copy(make_pair(cc1, cc2)));
  cout << "h1:" << endl;
  print(h1);

  return EXIT_SUCCESS;
}
