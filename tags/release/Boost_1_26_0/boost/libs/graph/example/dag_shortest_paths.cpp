//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#include <boost/graph/dag_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>

int main()
{
  using namespace boost;
  typedef adjacency_list<vecS, vecS, directedS, 
    property<vertex_distance_t, int>, property<edge_weight_t, int> > graph_t;
  graph_t g(6);
  enum verts { r, s, t, u, v, x };
  add_edge(r, s, 5, g);
  add_edge(r, t, 3, g);
  add_edge(s, t, 2, g);
  add_edge(s, u, 6, g);
  add_edge(t, u, 7, g);
  add_edge(t, v, 4, g);
  add_edge(t, x, 2, g);
  add_edge(u, v, -1, g);
  add_edge(u, x, 1, g);
  add_edge(v, x, -2, g);

  property_map<graph_t, vertex_distance_t>::type
    d_map = get(vertex_distance, g);

#ifdef BOOST_MSVC
  // VC++ has trouble with the named-parameter mechanism, so
  // we make a direct call to the underlying implementation function.
  std::vector<default_color_type> color(num_vertices(g));
  std::vector<std::size_t> pred(num_vertices(g));
  default_dijkstra_visitor vis;
  std::less<int> compare;
  closed_plus<int> combine;
  property_map<graph_t, edge_weight_t>::type w_map = get(edge_weight, g);
  dag_shortest_paths(g, s, d_map, w_map, &color[0], &pred[0], 
     vis, compare, combine, std::numeric_limits<int>::max(), 0);
#else
  dag_shortest_paths(g, s, distance_map(d_map));
#endif

  graph_traits<graph_t>::vertex_iterator vi , vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    std::cout << *vi << ": " << d_map[*vi] << std::endl;
  return 0;
}
