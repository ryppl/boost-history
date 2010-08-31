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

#include <boost/graph/sum.hpp>

using namespace boost;
using namespace std;

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph1 g1, g2, gs1, gs2, gs3, gs4, gs5, gs6, gs;
  Graph2 h1, h2, hs1, hs2, hs3;

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

  cout << "Sum of g1 and g2:" << endl;
  graph_sum(g1, g2, gs1);
  check_vertex_and_edge_naming(gs1);
  print_named_edges(gs1);

  cout << "Sum of g2 and g1:" << endl;
  graph_sum(g2, g1, gs2);
  check_vertex_and_edge_naming(gs2);
  print_named_edges(gs2);

  cout << "h1:" << endl;
  print(h1);
  cout << "h2:" << endl;
  print(h2);

  cout << "Simple union of h1 and h2:" << endl;
  simple_graph_sum(h1, h2, hs1);
  check_vertex_naming(hs1);
  print(hs1);

  cout << "Other tests..." << endl;

  graph_sum(g1, g2, gs3, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_and_edge_naming(gs3);

  graph_sum(g1, g2, gs4,
            vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10))
            .vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10))
            );
  check_vertex_and_edge_naming(gs4);
  //  print_named_edges(gs4);

  // For the following tests, this:
  // check_vertex_and_edge_naming(gs);
  // was supposed to work as in the early tests. It does inside the call to graph_sum
  // But after the graph is copied to gd, the edge name mapping does not work properly anymore.
  // For vertex it is ok: check_vertex_naming(gs)

  gs = graph_sum(g1, g2);
  check_vertex_naming(gs);

  gs = graph_sum(g1, g2, vertex_copy(my_vertex_copier<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gs);

  // simple_graph_sum

  simple_graph_sum(h2, h1, hs2, vertices_merge(my_vertex_merger<Graph2, Graph2>()));
  check_vertex_naming(hs2);

  simple_graph_sum(g1, g2, gs5);
  check_vertex_naming(gs5);

  simple_graph_sum(g1, g2, gs6, vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gs6);

  gs = simple_graph_sum(g1, g2);
  check_vertex_naming(gs);

  gs = simple_graph_sum(g1, g2, vertices_merge(my_vertex_merger<Graph1, Graph1>(100, 10)));
  check_vertex_naming(gs);

  // with different types
  simple_graph_sum
    (g1, g2, hs3,
     vertex_copy(my_vertex_copier<Graph1, Graph2>(100, 10))
     .vertices_merge(my_vertex_merger<Graph1, Graph2>(100, 10))
     );
  check_vertex_naming(hs3);

  cout << "Passed." << endl;

  return EXIT_SUCCESS;
}
