// Copyright (c) 2006, Stephan Diederich
//
// This code may be used under either of the following two licences:
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Neither the name of the Boost nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
// WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// Or:
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <fstream>

#include <boost/test/minimal.hpp>
//three max_flows we test here
#include <boost/graph/kolmogorov_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmunds_karp_max_flow.hpp>
//boost utilities we use
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/property_map.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/lexical_cast.hpp>

/***************
* test which compares results of the three different max_flow implementations
* command line parameters:
*   number_of_vertices: defaults to 100
*   number_of_edges:    defaults to 1000
*   seeed:              defaults to 1
***************/

using namespace boost;

int test_main(int argc, char* argv[])
{
  
  typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
  typedef adjacency_list < vecS, vecS, directedS,
  property < vertex_index_t, long,
  property < vertex_color_t, boost::default_color_type,
  property < vertex_distance_t, long,
  property < vertex_predecessor_t, Traits::edge_descriptor > > > >,
  property < edge_capacity_t, long,
  property < edge_residual_capacity_t, long,
  property < edge_reverse_t, Traits::edge_descriptor > > > > Graph;
  
  typedef graph_traits<Graph>::edge_descriptor tEdge;
  typedef graph_traits<Graph>::vertex_descriptor tVertex;
  
  graph_traits<Graph>::vertices_size_type n_verts = 100;
  graph_traits<Graph>::edges_size_type n_edges = 1000;
  std::size_t seed = 1;

  if (argc > 1) n_verts = lexical_cast<std::size_t>(argv[1]);
  if (argc > 2) n_edges = lexical_cast<std::size_t>(argv[2]);
  if (argc > 3) seed = lexical_cast<std::size_t>(argv[3]);
  
  Graph g;  
  const int cap_low = 1;
  const int cap_high = 1000;
  
  //init random numer generator
  minstd_rand gen(seed);
  //generate graph
  generate_random_graph(g, n_verts, n_edges, gen);   
  
  //init an uniform distribution int generator
  typedef variate_generator<minstd_rand, uniform_int<int> > tIntGen;
  tIntGen int_gen(gen, uniform_int<int>(cap_low, cap_high));
  //init edge-capacities
  randomize_property<edge_capacity_t, Graph, tIntGen> (g,int_gen);
  
  //get source and sink node
  tVertex source_vertex = random_vertex(g, gen);
  tVertex sink_vertex = graph_traits<Graph>::null_vertex();
  while(sink_vertex == graph_traits<Graph>::null_vertex() || sink_vertex == source_vertex)
    sink_vertex = random_vertex(g, gen);
  
  //add reverse edges (ugly... how to do better?!)
  property_map < Graph, edge_reverse_t >::type rev = get(edge_reverse, g);
  property_map < Graph, edge_capacity_t >::type cap = get(edge_capacity, g);
  std::list<tEdge> edges_copy;
  graph_traits<Graph>::edge_iterator ei, e_end;
  tie(ei, e_end) = edges(g);  
  std::copy(ei, e_end, std::back_insert_iterator< std::list<tEdge> >(edges_copy));
  while( ! edges_copy.empty()){
    tEdge old_edge=edges_copy.front();
    edges_copy.pop_front();
    tVertex source_vertex = target(old_edge, g);  
    tVertex target_vertex = source(old_edge, g);
    bool inserted;
    tEdge new_edge;        
    tie(new_edge,inserted) = add_edge(source_vertex, target_vertex, g); 
    assert(inserted);
    rev[old_edge] = new_edge;
    rev[new_edge] = old_edge ;
    cap[new_edge] = 0;
  }
  
  typedef property_traits< property_map<Graph, edge_capacity_t>::const_type>::value_type tEdgeVal;
  
  tEdgeVal kolmo = kolmogorov_max_flow(g,source_vertex,sink_vertex); 
  tEdgeVal push_relabel = push_relabel_max_flow(g,source_vertex,sink_vertex);
  tEdgeVal edmunds_karp = edmunds_karp_max_flow(g,source_vertex,sink_vertex);
  
  BOOST_REQUIRE( kolmo == push_relabel );
  BOOST_REQUIRE( push_relabel == edmunds_karp );

  return 0;
}
