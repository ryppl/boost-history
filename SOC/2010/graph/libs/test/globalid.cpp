#include <iostream>
#include <utility>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/global_vertex_mapping.hpp>

#include <boost/graph/difference.hpp>
#include <boost/graph/intersection.hpp>
#include <boost/graph/sum.hpp>

using namespace boost;
using namespace std;

// create a typedef for the Graph type
typedef adjacency_list < vecS, vecS, bidirectionalS, property < vertex_name_t, char > > Graph;
typedef pair < int, int > Edge;

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


int main(int,char*[])
{
  // Make convenient labels for the vertices
  enum { E, D, C, B, A, N };
  const int num_vertices = N;
  const char name[] = "ABCDE";
  Edge edge_array[] =
  { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
    Edge(C,E), Edge(B,D), Edge(D,E), };
  const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);
  typedef graph_traits<Graph>::vertex_iterator vertex_iter;

  // declare a graph object, adding the edges and edge properties
  Graph g1(edge_array, edge_array + num_edges, num_vertices);
  property_map < Graph, vertex_index_t>::type vertex_id = get(vertex_index, g1);
  property_map < Graph, vertex_name_t >::type name_map1 = get(vertex_name, g1);

  for (std::pair<vertex_iter, vertex_iter> vp = vertices(g1); vp.first != vp.second; ++vp.first) {
    int id = get(vertex_id, *vp.first);
    name_map1[id] = name[id];
  }

  graph_traits<Graph>::edge_iterator ei, ei_end;
  cout << "edges = ";
  for (tie(ei, ei_end) = edges(g1); ei != ei_end; ++ei)
    cout << "(" << vertex_id[source(*ei, g1)]
         << "," << vertex_id[target(*ei, g1)] << ") ";
  cout << endl;


  print_graph(g1, name_map1);

  Graph g2 = create_graph(5,1);
  property_map < Graph, vertex_name_t >::type name_map2 = get(vertex_name, g2);
  print_graph(g2, name_map2);




  GlobalVertexMapping < Graph, int, string > globalId(*vertices(g1).second, -2);
  globalId.add_graph(g1,"g1");
  globalId.add_graph(g2,"g2");
  globalId.associate(g1,0,0);
  globalId.associate(g1,1,1);
  globalId.associate(g1,1,0); // does nothing because (g1,1) is already associated to 1
  globalId.associate(g1,0,1); // does nothing because (g1,0) is already associated to 0
  globalId.associate(g1,2,2);
  globalId.associate(g1,3,3);
  globalId.associate(g1,4,4);

  globalId.associate(g2,0,4);
  globalId.associate(g2,1,3);
  globalId.associate(g2,2,2);
  globalId.associate(g2,3,1);
  globalId.associate(g2,4,0);

  globalId.show_associations();

  assert( globalId.equal(g1,0, g2,4));
  assert( globalId.equal(g1,1, g2,3));
  assert( globalId.equal(g1,2, g2,2));
  assert( globalId.equal(g1,3, g2,1));
  assert( globalId.equal(g1,4, g2,0));

  assert(!globalId.equal(g1,0, g2,0));
  assert(!globalId.equal(g1,0, g2,1));
  assert(!globalId.equal(g1,0, g2,2));
  assert(!globalId.equal(g1,0, g2,3));
  assert(!globalId.equal(g1,1, g2,0));
  assert(!globalId.equal(g1,1, g2,1));
  assert(!globalId.equal(g1,1, g2,2));
  assert(!globalId.equal(g1,1, g2,4));

  assert( globalId.get_id(g1,0).first == 0 );
  assert( globalId.get_id(g1,1).first == 1 );
  assert( globalId.get_id(g2,0).first == 4 );
  assert( globalId.get_id(g2,3).first == 1 );
  assert( globalId.get_id(g2,4).first == 0 );

  cout << "g1 + g2:" << endl;
  Graph gs;
  graph_sum(g1, g2, globalId, gs);
  print_graph(gs);

  cout << "g1 - g2:" << endl;
  Graph gd;
  difference(g1, g2, globalId, gd);
  print_graph(gd);

  cout << "g1 intersection g2:" << endl;
  Graph gi;
  intersection(g1, g2, globalId, gi);
  print_graph(gi);

  return 0;
}
