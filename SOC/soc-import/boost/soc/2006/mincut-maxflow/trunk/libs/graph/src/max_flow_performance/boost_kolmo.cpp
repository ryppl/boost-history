//=======================================================================
// Copyright 2006 Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/kolmogorov_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/graph/read_dimacs.hpp>

//use DIMACS max-flow file as stdin
#include <sys/time.h>
#include <sys/resource.h>
#include <valgrind/callgrind.h>

float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

template <typename EdgeDescriptor>
struct Pixel{
  boost::default_color_type color;
  long distance;
  EdgeDescriptor predecessor;
};

template <typename EdgeDescriptor>
struct Link{
  long capacity;
  long residual_capacity;
  EdgeDescriptor edge_reverse;
};

template <typename Graph, typename CapMap, typename RevEdgeMap>
    struct duplicate_reverse_edge_cleaner{
  duplicate_reverse_edge_cleaner(Graph& g, CapMap cap, RevEdgeMap rev):m_g(g), m_cap(cap), m_rev(rev){
  }

  bool operator()(typename boost::property_traits<RevEdgeMap>::value_type e){
    using namespace boost;
    if(m_cap[e] > 0){
      return false;
    }
    //if this edge has zero capacity and another edge connecting those two vertices is already present, remove this edge
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename graph_traits<Graph>::out_edge_iterator out_edge_iterator;
    vertex_descriptor source_node = source(e, m_g);
    vertex_descriptor target_node = target(e, m_g);
    //now we check if there's a paralel edge to the current one
    out_edge_iterator oei, oe_end;
    for(tie(oei, oe_end) = out_edges(source_node, m_g); oei != oe_end; ++oei){
      vertex_descriptor temp_node = target(*oei, m_g);
      if(temp_node == target_node && *oei != e){
        // yeah, theres another edge like this one
        // edge e isn't necessary, so it can be removed
        // but take care, that the edge that refers to this gets a new reverse_edge 
        edge_descriptor my_partner = m_rev[e];
        m_rev[my_partner] = *oei;
        return true;
      }
    }
    return false;
  }
  
  Graph& m_g;
  CapMap m_cap;
  RevEdgeMap m_rev;
};

int
main()
{
  using namespace boost;
  using namespace std;
  typedef adjacency_list_traits <vecS, vecS, directedS> Traits;
  typedef Traits::edge_descriptor edge_descriptor;

  typedef Link<Traits::edge_descriptor> tEdge;
  typedef Pixel<Traits::edge_descriptor> tVertex;
  
  typedef adjacency_list <vecS, vecS, directedS, tVertex, tEdge> Graph;
  Graph g;

  typedef property_map<Graph, long tEdge::*>::type tCapacityMap;
  tCapacityMap cap_map = get(&tEdge::capacity, g);
  typedef property_map<Graph, edge_descriptor tEdge::*>::type tReverseEdgeMap;
  tReverseEdgeMap rev_edge_map = get(&tEdge::edge_reverse, g);
  
  Traits::vertex_descriptor s, t;
  read_dimacs_max_flow(g, cap_map, rev_edge_map, s, t);

//   duplicate_reverse_edge_cleaner<Graph, tCapacityMap, tReverseEdgeMap> filter(g, cap_map, rev_edge_map);
//   cap_map[*edges(g).first] = 0;
//   rev_edge_map[*edges(g).first] = *edges(g).first;
//   std::cout << "Before: " << num_edges(g) << std::endl;
//   remove_edge_if(filter, g);
//   std::cout << "Before: " << num_edges(g) << std::endl;

  CALLGRIND_START_INSTRUMENTATION();
  float time = timer();
  long flow = kolmogorov_max_flow(g, get(&tEdge::capacity, g), get(&tEdge::residual_capacity, g), get(&tEdge::edge_reverse, g) ,
                                  get(&tVertex::predecessor, g), get(&tVertex::color, g), get(&tVertex::distance, g), get(vertex_index, g), s, t);
  double elapsed = timer() - time;
  CALLGRIND_STOP_INSTRUMENTATION();
  
  std::cout << "flow: " << flow << std::endl;
  std::cout << "time: " << elapsed << std::endl;
  return EXIT_SUCCESS;
}
