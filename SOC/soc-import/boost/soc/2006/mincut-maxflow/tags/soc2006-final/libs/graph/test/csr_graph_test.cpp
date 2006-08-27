// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Jeremiah Willcock
//           Douglas Gregor
//           Andrew Lumsdaine

// The libstdc++ debug mode makes this test run for hours...
#ifdef _GLIBCXX_DEBUG
#  undef _GLIBCXX_DEBUG
#endif

// Test for the compressed sparse row graph type
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/test/minimal.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/random/linear_congruential.hpp>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <boost/lexical_cast.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/limits.hpp>
#include <string>
#include <boost/graph/iteration_macros.hpp>

// Algorithms to test against
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

struct vertex_props{
  long x;
};

int test_main(int argc, char* argv[])
{
  using namespace boost;
  typedef compressed_sparse_row_graph<directedS, vertex_props> tGraph;
  typedef graph_traits<tGraph>::vertex_descriptor vertex_descriptor;
  tGraph g;
  
  vertex_descriptor v = add_vertex(g);
  std::cout << g[v].x << std::endl;
  return 0;
}
