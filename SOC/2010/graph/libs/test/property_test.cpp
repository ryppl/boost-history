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
#include <utility>
#include <algorithm>

#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/sum.hpp>
#include <boost/graph/difference.hpp>
#include <boost/graph/intersection.hpp>
#include <boost/graph/disjoint_union.hpp>
#include <boost/graph/complement.hpp>
#include <boost/graph/join.hpp>

#include "graph_op_common.hpp"

using namespace boost;
using namespace std;

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  Graph1 g1, g2, g_simple_compl, g_compl, g_rcompl, g_int, g_sum, g_diff, g_sym_diff, g_sym_diff2, g_union, g_join, g_join2;

  generate_random_graph(g1, 4, 10, gen, false);
  generate_random_graph(g2, 3,  5, gen, false);

  auto_name_vertices(g1, "g1_", 10, 100);
  auto_name_edges(g1);
  auto_name_vertices(g2, "g2_", 10, 200);
  auto_name_edges(g2);

  cout << "Graph 1 (g1):" << endl;
  check_vertex_and_edge_naming(g1);
  print_named_edges(g1);
  cout << endl;

  cout << "Graph 2 (g2):" << endl;
  check_vertex_and_edge_naming(g2);
  print_named_edges(g2);
  cout << endl;

  cout << "Complement of g1:" << endl;
  graph_complement(g1, g_simple_compl); // ignore name mapping (but copy vertex properties)
  // check_vertex_and_edge_naming(g_compl); // graph_complement don't set graph_label
  print_named_edges(g_simple_compl);
  cout << endl;

  cout << "Complement of g1:" << endl;
  g_compl = graph_complement(g1, vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>()));
  check_vertex_naming(g_compl); // graph_complement don't set edge names in graph_label, but my_vertex_copier do it for vertices
  print_named_edges(g_compl);
  cout << endl;

  cout << "Reflexive complement of g1:" << endl;
  graph_reflexive_complement
    (g1, g_rcompl,
     vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>())
     .edge_visitor(my_edge_visitor<Graph1>()));
  check_vertex_and_edge_naming(g_rcompl);
  print_named_edges(g_rcompl);
  cout << endl;

  cout << "Intersection of g1 and g2:" << endl;
  graph_intersection(g1, g2, g_int);
  check_vertex_and_edge_naming(g_int);
  print_named_edges(g_int);
  cout << endl;

  cout << "Sum of g1 and g2:" << endl;
  graph_sum(g1, g2, g_sum);
  check_vertex_and_edge_naming(g_sum);
  print_named_edges(g_sum);
  cout << endl;

  cout << "Difference of g1 and g2:" << endl;
  graph_difference(g1, g2, g_diff);
  check_vertex_and_edge_naming(g_diff);
  print_named_edges(g_diff);
  cout << endl;

  cout << "Vertex Symmetric Difference of g1 and g2:" << endl;
  graph_vertex_symmetric_difference(g1, g2, g_sym_diff);
  check_vertex_and_edge_naming(g_sym_diff);
  print_named_edges(g_sym_diff);
  cout << endl;

  cout << "Edge Symmetric Difference of g1 and g2:" << endl;
  graph_vertex_symmetric_difference(g1, g2, g_sym_diff2);
  check_vertex_and_edge_naming(g_sym_diff2);
  print_named_edges(g_sym_diff2);
  cout << endl;

  // For union and join, the vertex and edge set are considered to be disjoint.
  // They ignore the vertex and edge names

  cout << "Disjoint union of g1 and g2:" << endl;
  graph_disjoint_union(g1, g2, g_union);
  print_named_edges(g_union);
  cout << endl;

  cout << "Join of g1 and g2:" << endl;
  graph_join(g1, g2, g_join);
  print_named_edges(g_join);
  cout << endl;

  cout << "Another join of g1 and g2: (g2 with new labels)" << endl;
  auto_name_vertices(g2, "g2_", 20, 200);
  auto_name_edges(g2);
  graph_join(g1, g2, g_join2,
             vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>())
             .edge_copy(my_edge_copier_and_setter<Graph1, Graph1>())
             .edge_visitor(my_edge_visitor<Graph1>()));
  check_vertex_and_edge_naming(g_join2); // passes the test because we are using our own functions
  print_named_edges(g_join2);
  //  cout << endl;

  return EXIT_SUCCESS;
}
