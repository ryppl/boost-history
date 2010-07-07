#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/transpose_graph.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/union.hpp>
#include <boost/graph/sum.hpp>
#include <boost/graph/intersection.hpp>
#include <boost/graph/difference.hpp>
#include <boost/graph/join.hpp>


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
void to_source(Graph &g, int v, char name)
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

void to_sink(Graph &g, int v, char name)
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
  to_source(h1, 5, 'x');
  to_sink(h1, 5, 'x');

  to_sink(h2, 5, 'y');
  // x and y are the "same"

  add_edge(0, 6, h1); // vertex 6 (w) is only in h1
  add_edge(6, 0, h1);
  get(vertex_name, h1)[6] = 'w';

  add_edge(7, 0, h2); // vertex 7 (z) is only in h2
  add_edge(0, 7, h2);
  get(vertex_name, h2)[7] = 'z';

  prt(h1);
  prt(h2);

  Graph h_du;
  cout << "Disjoint union:" << endl;
  disjoint_union(h1, h2, h_du);
  prt(h_du);
  // is there any way to clear h_du and use the same variable for all?


  // in this example (graph sum), it creates some parallel edges (e.g., a --> x appears twice)
  // because x and y are the considered as the same
  Graph h_s;
  cout << "Graph sum:" << endl;
  graph_sum(h1, h2, h_s);
  prt(h_s);


  Graph h_i;
  cout << "Graph intersection:" << endl;
  intersection(h1, h2, h_i);
  prt(h_i);


  Graph h_diff;
  cout << "Graph difference:" << endl;
  difference(h1, h2, h_diff);
  prt(h_diff);

  // Not working
//  Graph h_j;
//  cout << "Graph join:" << endl;
//  graph_join(h1, h2, h_j);
//  prt(h_j);

  


  return 0;
}
