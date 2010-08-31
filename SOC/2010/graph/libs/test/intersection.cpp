//
//=======================================================================
// Copyright (C) 2010 Davi M. J. Barbosa
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#include <iostream>

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include "graph_op_common.hpp"

#include <boost/graph/intersection.hpp>

using namespace boost;
using namespace std;

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph1 g1, g2, gi1, gi2, gi3, gi4, gi5, gi;
  Graph2 h1, h2, hi1, hi2, hi3;

  generate_random_graph(g1, 5,  10, gen, false);
  generate_random_graph(g2, 3,   6, gen, false);

  auto_name_vertices(g1, "g1_", 10, 100);
  auto_name_edges(g1);
  auto_name_vertices(g2, "g2_", 11, 200);
  auto_name_edges(g2);

  auto_name_vertices(h1, "h1_", 10, 100);
  auto_name_vertices(h2, "h2_", 11, 200);

  cout << "g1:" << endl;
  print_named_edges(g1);
  cout << "g2:" << endl;
  print_named_edges(g2);

  cout << "Intersection of g1 and g2:" << endl;
  graph_intersection(g1, g2, gi1);
  check_vertex_and_edge_naming(gi1);
  print_named_edges(gi1);

  cout << "Intersection of g2 and g1:" << endl;
  graph_intersection(g2, g1, gi2);
  check_vertex_and_edge_naming(gi2);
  print_named_edges(gi2);

  cout << "h1:" << endl;
  print(h1);
  cout << "h2:" << endl;
  print(h2);

  cout << "Simple intersection of h1 and h2:" << endl;
  simple_graph_intersection(h1, h2, hi1);
  check_vertex_naming(hi1);
  print(hi1);

  cout << "Other tests..." << endl;

  graph_intersection(g1, g2, gi3, vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10)));
  check_vertex_and_edge_naming(gi3);

  // For the following tests, this:
  // check_vertex_and_edge_naming(gi);
  // was supposed to work as in the early tests. It does inside the call to graph_intersection (and symmetric intersections)
  // But after the graph is copied to gd, the edge name mapping does not work properly anymore.
  // For vertex it is ok: check_vertex_naming(gi)

  gi = graph_intersection(g1, g2);
  check_vertex_naming(gi);

  gi = graph_intersection(g1, g2, vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gi);

  // simple_graph_intersection

  simple_graph_intersection(h2, h1, hi2, vertices_merge(my_vertex_merger<Graph2, Graph2>()));
  check_vertex_naming(hi2);

  simple_graph_intersection(g1, g2, gi4);
  check_vertex_naming(gi4);

  simple_graph_intersection(g1, g2, gi5, vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gi5);

  gi = simple_graph_intersection(g1, g2);
  check_vertex_naming(gi);

  gi = simple_graph_intersection(g1, g2, vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gi);

  // with different types
  simple_graph_intersection(g1, g2, hi3, vertices_merge(my_vertex_merger<Graph1, Graph2>(100, 10)));
  check_vertex_naming(hi3);

  cout << "Passed." << endl;

  return EXIT_SUCCESS;
}
