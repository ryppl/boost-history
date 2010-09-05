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

#include <typeinfo>

#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

#include "graph_op_common.hpp"

#include <boost/graph/complement.hpp>

// Vertex and edge names are NOT used by complement
// They are here only to test how properties are copied
// and how it works with a custom copier function

using namespace boost;
using namespace std;

int main(int,char*[])
{
  boost::mt19937 gen;
  gen.seed(uint32_t(time(0)));

  cout << "Complement test" << endl;

  cout << "Graph g: " << endl;
  Graph2 g;
  generate_random_graph(g, 4, 10, gen, false, false);
  auto_name_vertices(g, "g");
  check_vertex_naming(g);
  print(g);

  cout << "Complement of g:" << endl;
  Graph2 gc = graph_complement(g);
  //  check_vertex_naming(gc); // graph_complement don't set graph_label
  print(gc);

  cout << "Reflexive Complement of g:" << endl;
  Graph2 gc2 = graph_reflexive_complement(g, vertex_copy(my_vertex_copier_and_setter<Graph2, Graph2>()));
  check_vertex_naming(gc2); // vertex_copier_and_setter is doing it
  print(gc2);

  cout << endl;

  cout << "Graph h: " << endl;
  Graph1 h;
  generate_random_graph(h, 4, 12, gen, false, true);
  auto_name_vertices(h, "h");
  auto_name_edges(h);
  check_vertex_and_edge_naming(h);
  print_named_edges(h);

  cout << "Complement of h:" << endl;
  Graph1 hc1;
  graph_complement
    (h, hc1,
     vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>())
     .edge_visitor(my_edge_visitor<Graph1>())
     );
  check_vertex_and_edge_naming(hc1);
  print_named_edges(hc1);

  cout << "Reflexive Complement of h:" << endl;
  Graph1 hc2;
  graph_reflexive_complement
    (h, hc2,
     vertex_copy(my_vertex_copier_and_setter<Graph1, Graph1>())
     .edge_visitor(my_edge_visitor<Graph1>())
     .vertex_index_map(get(vertex_index, h))
     );
  check_vertex_and_edge_naming(hc2);
  print_named_edges(hc2);

  cout << endl;

  cout << "Testing with different types and other tests..." << endl;

  Graph1 g_other;
  graph_complement
    (g, g_other,
     vertex_copy(my_vertex_copier_and_setter<Graph2, Graph1>(100,10))
     .edge_visitor(my_edge_visitor<Graph1>()));
  check_vertex_and_edge_naming(g_other);

  Graph2 h_other;
  graph_complement
    (h, h_other,
     vertex_copy(my_vertex_copier_and_setter<Graph1, Graph2>(10,100)));
  check_vertex_naming(h_other);

  Graph1 h_out;
  graph_complement(h, h_out);

  cout << "Passed." << endl;
  return EXIT_SUCCESS;
}
