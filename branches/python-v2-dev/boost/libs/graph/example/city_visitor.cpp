//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Generic Graph Component Library
//
// You should have received a copy of the License Agreement for the
// Generic Graph Component Library along with the software;  see the
// file LICENSE.  If not, contact Office of Research, University of Notre
// Dame, Notre Dame, IN  46556.
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
//

#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/property_map.hpp>
#include <boost/graph/graph_utility.hpp> // for boost::make_list


/*
  Example of using a visitor with the depth first search 
    and breadth first search algorithm

  Sacramento ---- Reno ---- Salt Lake City
     |
  San Francisco
     |
  San Jose ---- Fresno
     |
  Los Angeles ---- Los Vegas ---- Pheonix
     |
  San Diego  


  The visitor has three main functions: 
  
  discover_vertex(u,g) is invoked when the algorithm first arrives at the
    vertex u. This will happen in the depth first or breadth first
    order depending on which algorithm you use.

  examine_edge(e,g) is invoked when the algorithm first checks an edge to see
    whether it has already been there. Whether using BFS or DFS, all
    the edges of vertex u are examined immediately after the call to
    visit(u).

  finish_vertex(u,g) is called when after all the vertices reachable from vertex
    u have already been visited.    

 */

using namespace std;
using namespace boost;


struct city_arrival : public base_visitor<city_arrival>
{
  city_arrival(string* n) : names(n) { }
  typedef on_discover_vertex event_filter;
  template <class Vertex, class Graph>
  inline void operator()(Vertex u, Graph&) {
    cout << endl << "arriving at " << names[u] << endl
         << "  neighboring cities are: ";
  }
  string* names;
};

struct neighbor_cities : public base_visitor<neighbor_cities>
{
  neighbor_cities(string* n) : names(n) { }
  typedef on_examine_edge event_filter;
  template <class Edge, class Graph>
  inline void operator()(Edge e, Graph& g) {
    cout << names[ target(e, g) ] << ", ";
  }
  string* names;
};

struct finish_city : public base_visitor<finish_city>
{
  finish_city(string* n) : names(n) { }
  typedef on_finish_vertex event_filter;
  template <class Vertex, class Graph>
  inline void operator()(Vertex u, Graph&) {
    cout << endl << "finished with " << names[u] << endl;
  }
  string* names;
};

int main(int, char*[]) 
{

  enum { SanJose, SanFran, LA, SanDiego, Fresno, LosVegas, Reno,
         Sacramento, SaltLake, Pheonix, N };

  string names[] = { "San Jose", "San Francisco",  "San Jose",
                     "San Francisco", "Los Angeles", "San Diego", 
                     "Fresno", "Los Vegas", "Reno", "Sacramento",
                     "Salt Lake City", "Pheonix" };

  typedef std::pair<int,int> E;
  E edge_array[] = { E(Sacramento, Reno), E(Sacramento, SanFran),
                     E(Reno, SaltLake),
                     E(SanFran, SanJose),
                     E(SanJose, Fresno), E(SanJose, LA),
                     E(LA, LosVegas), E(LA, SanDiego),
                     E(LosVegas, Pheonix) };

  /* Create the graph type we want. */
  typedef adjacency_list<vecS, vecS, undirectedS> Graph;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the edge iterator constructor
  Graph G(N);
  for (std::size_t j = 0; j < sizeof(edge_array)/sizeof(E); ++j)
    add_edge(edge_array[j].first, edge_array[j].second, G);
#else
  Graph G(edge_array, edge_array + sizeof(edge_array)/sizeof(E), N);
#endif

  cout << "*** Depth First ***" << endl;
  depth_first_search
    (G, 
     visitor(make_dfs_visitor(boost::make_list(city_arrival(names),
                                               neighbor_cities(names),
                                               finish_city(names)))));
  cout << endl;

  /* Get the source vertex */
  boost::graph_traits<Graph>::vertex_descriptor 
    s = vertex(SanJose,G);

  cout << "*** Breadth First ***" << endl;
  breadth_first_search
    (G, s, visitor(make_bfs_visitor(boost::make_list(city_arrival(names), 
                                                     neighbor_cities(names), 
                                                     finish_city(names)))));
  
  return 0;
}
