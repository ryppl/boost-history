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

#include <boost/graph/difference.hpp>

using namespace boost;
using namespace std;

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph1 g1, g2, gd1, gd2, gd3, gd4, gd5, gd6, gd7, gd8, gd9, gd10, gd;
  Graph2 h1, h2, hd1, hd2, hd3, hd4, hd5, hd6, hd7, hd8, hd9, hd;

  generate_random_graph(g1, 5,  10, gen, false);
  generate_random_graph(g2, 3,   6, gen, false);

  auto_name_vertices(g1, "g1_", 10, 100);
  auto_name_edges(g1);
  auto_name_vertices(g2, "g2_", 11, 200);
  auto_name_edges(g2);

  cout << "g1:" << endl;
  print_named_edges(g1);
  cout << "g2:" << endl;
  print_named_edges(g2);

  cout << "Difference of g1 and g2:" << endl;
  graph_difference(g1, g2, gd1);
  check_vertex_and_edge_naming(gd1);
  print_named_edges(gd1);

  cout << "Difference of g2 and g1:" << endl;
  graph_difference(g2, g1, gd2);
  check_vertex_and_edge_naming(gd2);
  print_named_edges(gd2);

  cout << "Vertex Symmetric Difference of g1 and g2:" << endl;
  graph_vertex_symmetric_difference(g1, g2, gd3);
  check_vertex_and_edge_naming(gd3);
  print_named_edges(gd3);

  cout << "Vertex Symmetric Difference of g2 and g1:" << endl;
  graph_vertex_symmetric_difference(g1, g2, gd4);
  check_vertex_and_edge_naming(gd4);
  print_named_edges(gd4);

  cout << "Edge Symmetric Difference of g1 and g2:" << endl;
  graph_edge_symmetric_difference(g1, g2, gd5);
  check_vertex_and_edge_naming(gd5);
  print_named_edges(gd5);

  cout << "Edge Symmetric Difference of g2 and g1:" << endl;
  graph_edge_symmetric_difference(g1, g2, gd6);
  check_vertex_and_edge_naming(gd6);
  print_named_edges(gd6);

  cout << "h1:" << endl;
  print(h1);
  cout << "h2:" << endl;
  print(h2);

  cout << "Simple difference of h1 and h2:" << endl;
  simple_graph_difference(h1, h2, hd1);
  check_vertex_naming(hd1);
  print(hd1);

  cout << "Simple vertex symmetric difference of h1 and h2:" << endl;
  simple_graph_vertex_symmetric_difference(h1, h2, hd2);
  check_vertex_naming(hd2);
  print(hd2);

  cout << "Simple edge symmetric difference of h1 and h2:" << endl;
  simple_graph_edge_symmetric_difference(h1, h2, hd3);
  check_vertex_naming(hd3);
  print(hd3);

  cout << "Other tests..." << endl;

  graph_difference(g1, g2, gd7, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_and_edge_naming(gd7);

  graph_vertex_symmetric_difference(g1, g2, gd8, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_and_edge_naming(gd8);

  graph_edge_symmetric_difference(g1, g2, gd9, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_and_edge_naming(gd9);

  simple_graph_difference(h1, h2, hd4, vertex_copy(my_vertex_copier<Graph2, Graph2>(100, 10)));
  check_vertex_naming(hd4);

  simple_graph_vertex_symmetric_difference(h1, h2, hd5, vertex_copy(my_vertex_copier<Graph2, Graph2>(100, 10)));
  check_vertex_naming(hd5);

  simple_graph_edge_symmetric_difference(h1, h2, hd6, vertex_copy(my_vertex_copier<Graph2, Graph2>(100, 10)));
  check_vertex_naming(hd6);

  // For the following tests, this:
  // check_vertex_and_edge_naming(gd);
  // was supposed to work as in the early tests. It does inside the call to graph_difference (and symmetric differences)
  // But after the graph is copied to gd, the edge name mapping does not work properly anymore.
  // For vertex it is ok: check_vertex_naming(gd)

  gd = graph_difference(g1, g2);
  check_vertex_naming(gd);

  gd = graph_difference(g1, g2, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gd);

  gd = graph_vertex_symmetric_difference(g1, g2);
  check_vertex_naming(gd);

  gd = graph_vertex_symmetric_difference(g1, g2, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gd);

  gd = graph_edge_symmetric_difference(g1, g2);
  check_vertex_naming(gd);

  gd = graph_edge_symmetric_difference(g1, g2, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gd);


  hd = simple_graph_difference(h1, h2);
  check_vertex_naming(hd);

  hd = simple_graph_difference(h1, h2, vertices_merge(my_vertex_merger<Graph2, Graph2>(100, 10)));
  check_vertex_naming(hd);

  hd = simple_graph_vertex_symmetric_difference(h1, h2);
  check_vertex_naming(hd);

  hd = simple_graph_vertex_symmetric_difference(h1, h2, vertices_merge(my_vertex_merger<Graph2, Graph2>(100, 10)));
  check_vertex_naming(hd);

  hd = simple_graph_edge_symmetric_difference(h1, h2);
  check_vertex_naming(hd);

  hd = simple_graph_edge_symmetric_difference(h1, h2, vertices_merge(my_vertex_merger<Graph2, Graph2>(100, 10)));
  check_vertex_naming(hd);

  // with different types
  simple_graph_difference
    (g1, g2, hd7,
     vertex_copy(my_vertex_copier<Graph1, Graph2>(100, 10)));
  check_vertex_naming(hd7);

  simple_graph_vertex_symmetric_difference
    (g1, g2, hd8,
     vertex_copy(my_vertex_copier<Graph1, Graph2>(100, 10)));
  check_vertex_naming(hd8);

  simple_graph_edge_symmetric_difference
    (g1, g2, hd9,
     vertex_copy(my_vertex_copier<Graph1, Graph2>(100, 10))
     .vertices_merge(my_vertex_merger<Graph1, Graph2>(100, 10)));
  check_vertex_naming(hd9);

  cout << "Passed." << endl;

  return EXIT_SUCCESS;
}
