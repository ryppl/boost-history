#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

/*
  Sample output:

  original graph:
  0 --> 3 2 3 
  1 --> 3 
  2 --> 0 
  3 --> 2 
  1(0,3) 2(0,2) 3(0,3) 4(1,3) 5(2,0) 6(3,2) 

  removing edges connecting 0 to 3
  0 --> 2 
  1 --> 3 
  2 --> 0 
  3 --> 2 
  2(0,2) 4(1,3) 5(2,0) 6(3,2) 
  removing edges with weight greater than 3
  0 --> 2 
  1 --> 
  2 --> 
  3 --> 
  2(0,2) 


 */

using namespace boost;

typedef adjacency_list<vecS, vecS, bidirectionalS, 
  no_property, property<edge_weight_t, int> > Graph;

struct has_target {
  has_target(graph_traits<Graph>::vertex_descriptor v,
	     Graph& g_) : targ(v), g(g_) { }
  bool operator()(graph_traits<Graph>::edge_descriptor e) {
    return target(e, g) == targ;
  }
  graph_traits<Graph>::vertex_descriptor targ;
  Graph& g;
};

struct has_weight_greater_than {
  has_weight_greater_than(int w_, Graph& g_) : w(w_), g(g_) { }
  bool operator()(graph_traits<Graph>::edge_descriptor e) {
    return get(edge_weight, g, e) > w;
  }
  int w;
  Graph& g;
};

int
main()
{
  typedef std::pair<std::size_t,std::size_t> Edge;
  Edge edges[6] = { Edge(0,3), Edge(0,2), Edge(0, 3),
		    Edge(1,3),
		    Edge(2, 0),
		    Edge(3, 2) };

  Graph g(4, edges, edges + 6);
  property_map<Graph, edge_weight_t>::type 
    weight = get(edge_weight, g);

  int w = 0;
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
    weight[*ei] = ++w;

  std::cout << "original graph:" << std::endl;
  print_graph(g, get(vertex_index, g));
  print_edges2(g, get(vertex_index, g), get(edge_weight, g));
  std::cout << std::endl;

  std::cout << "removing edges connecting 0 to 3" << std::endl;
  remove_out_edge_if(vertex(0,g), has_target(vertex(3,g), g), g);
  print_graph(g, get(vertex_index, g));
  print_edges2(g, get(vertex_index, g), get(edge_weight, g));

  std::cout << "removing edges with weight greater than 3" << std::endl;
  remove_edge_if(has_weight_greater_than(3, g), g);
  print_graph(g, get(vertex_index, g));
  print_edges2(g, get(vertex_index, g), get(edge_weight, g));

  return 0;
}
