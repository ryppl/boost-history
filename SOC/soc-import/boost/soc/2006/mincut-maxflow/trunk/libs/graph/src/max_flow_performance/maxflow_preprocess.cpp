//=======================================================================
// Copyright 2006 Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <iostream>
#include <iterator>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>
#include <boost/graph/write_dimacs.hpp>
#include <boost/graph/filtered_graph.hpp>
/*
* This small program reads an dimacs_max_flow problem in dimacs format from stdin
* and removes all direct SOURCE->NODE->SINK connects and writes the file back to stdout
* the "removed" flow is written in a comment in the dimacs file.
*/

template <typename EdgeCapacityMap>
struct zero_edge_capacity{
  
  zero_edge_capacity() { }
  zero_edge_capacity(EdgeCapacityMap cap_map):m_cap_map(cap_map){};

  template <typename Edge>
      bool operator() (const Edge& e) const {
    return  get(m_cap_map, e) == 0 ;
      }

      EdgeCapacityMap m_cap_map;
};

int main()
{
  using namespace boost;
  typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
  typedef adjacency_list < vecS, vecS, directedS,
  no_property,
  property < edge_capacity_t, long,
  property < edge_residual_capacity_t, long,
  property < edge_reverse_t, Traits::edge_descriptor > > > > Graph;
  
  typedef graph_traits<Graph>::out_edge_iterator out_edge_iterator;
  typedef graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  
  Graph g;

  typedef property_map < Graph, edge_capacity_t >::type tCapMap;
  typedef tCapMap::value_type tCapMapValue;
  tCapMap capacity = get(edge_capacity, g);
  
  property_map < Graph, edge_residual_capacity_t >::type
      residual_capacity = get(edge_residual_capacity, g);
  property_map < Graph, edge_reverse_t >::type rev = get(edge_reverse, g);
  vertex_descriptor s, t;
  /*reading the graph from stdin*/
  read_dimacs_max_flow(g, capacity, rev, s, t);
  
  /*process graph*/
  tCapMapValue augmented_flow = 0;
  //we take the source node and check for each outgoing edge e which has a target(p) if we can augment that path
  
  out_edge_iterator oei,oe_end;
  for(tie(oei, oe_end) = out_edges(s, g); oei != oe_end; ++oei){
    edge_descriptor from_source = *oei;
    vertex_descriptor v = target(from_source, g);
    edge_descriptor to_sink;
    bool is_there;
    tie(to_sink, is_there) = edge(v, t, g);
    if( is_there ){
      if( get(capacity, to_sink) > get(capacity, from_source) ){ 
	tCapMapValue to_augment=get(capacity, from_source);
	capacity[from_source]=0;
	capacity[to_sink]-=to_augment;
        augmented_flow += to_augment;
      }else{
	tCapMapValue to_augment=get(capacity, to_sink);	
	capacity[to_sink]=0;
	capacity[from_source]-=to_augment;
        augmented_flow += to_augment;
      }
    }
  }

  /*write the graph back to stdout */
  zero_edge_capacity<tCapMap> filter(capacity);
  //remove zero edges (most of them are the reverse edges)
  remove_edge_if(filter, g);
  write_dimacs_max_flow(g, capacity, identity_property_map(),s, t, std::cout);
  std::cerr << "removed " << augmented_flow << " from SOURCE->NODE->SINK connects" <<std::endl;
  return 0;
}
