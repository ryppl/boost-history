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

#include <boost/graph/join.hpp>

// Vertex and edge names are NOT used by join
// They are here only to test how properties are copied
// and how it works with a custom copier function

using namespace boost;
using namespace std;

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph1 g1, g2, g3, g4;
  Graph2 h;

  generate_random_graph(g1, 3,  3, gen, false);
  generate_random_graph(g2, 4,  6, gen, false);

  auto_name_vertices(g1, "g1_", 10, 100);
  auto_name_vertices(g2, "g2_", 60, 200);

  cout << "g1:" << endl;
  print(g1);
  cout << "g2:" << endl;
  print(g2);

  cout << "Join of g1 and g2:" << endl;
  graph_join(g1, g2, g3);
  print(g3);

  cout << "Join of g1 and g2:" << endl;
  graph_join(g1, g2, g4, vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>()));
  check_vertex_naming(g4);
  print(g4);

  cout << "Other tests..." << endl;

  g4 = graph_join(g1, g2);

  auto_name_edges(g1);
  auto_name_edges(g2);
  Graph1 g5;
  graph_join(g1, g2, g5,
             vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>())
             .edge_copy(my_edge_copier_and_setter<Graph1, Graph1>())
             .edge_visitor(my_edge_visitor<Graph1>())
             );
  check_vertex_and_edge_naming(g5);

  g4 = graph_join(g1, g2,
                  vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>())
                  .edge_copy(my_edge_copier_and_setter<Graph1, Graph1>())
                  .edge_visitor(my_edge_visitor<Graph1>())
                  );
  // This was supposed to be exactly like the g5 example and work with:
  // check_vertex_and_edge_naming(g4);
  // and it works just before, inside and at the end of that call to graph_join
  // After the graph is copied to g4, the edge name mapping does not work properly anymore.
  // For vertex it is ok:
  check_vertex_naming(g1);

  cout << "Passed." << endl;

  cout << "Join of g1 and g2: (output with different type + special copy vertex)" << endl;
  graph_join(g1, g2, h, vertex_copy(my_vertex_copier<Graph1, Graph2>(100, 10)));
  print(h);

  return EXIT_SUCCESS;
}
