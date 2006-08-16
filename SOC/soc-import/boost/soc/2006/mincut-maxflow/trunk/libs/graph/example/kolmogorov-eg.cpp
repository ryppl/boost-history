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

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/kolmogorov_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>
#include <boost/graph/graph_utility.hpp>

// Use a DIMACS network flow file as stdin.
// kolmogorov-eg < max_flow.dat
//
// Sample output:
// c  The total flow:
// s 13
// 
// c flow values:
// f 0 6 3
// f 0 1 6
// f 0 2 4
// f 1 5 1
// f 1 0 0
// f 1 3 5
// f 2 4 4
// f 2 3 0
// f 2 0 0
// f 3 7 5
// f 3 2 0
// f 3 1 0
// f 4 5 0
// f 4 6 4
// f 5 4 0
// f 5 7 1
// f 6 7 7
// f 6 4 0
// f 7 6 0
// f 7 5 0

int
main()
{
  using namespace boost;

  typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
  typedef adjacency_list < vecS, vecS, directedS,
    property < vertex_name_t, std::string,
    property < vertex_index_t, long,
    property < vertex_color_t, boost::default_color_type,
    property < vertex_distance_t, long,
    property < vertex_predecessor_t, Traits::edge_descriptor > > > > >,
    
    property < edge_capacity_t, long,
    property < edge_residual_capacity_t, long,
    property < edge_reverse_t, Traits::edge_descriptor > > > > Graph;
 
  Graph g;
  property_map < Graph, edge_capacity_t >::type
      capacity = get(edge_capacity, g);
  property_map < Graph, edge_residual_capacity_t >::type
      residual_capacity = get(edge_residual_capacity, g);
  property_map < Graph, edge_reverse_t >::type rev = get(edge_reverse, g);
  Traits::vertex_descriptor s, t;
  read_dimacs_max_flow(g, capacity, rev, s, t);

  std::vector<default_color_type> color(num_vertices(g));
  std::vector<long> distance(num_vertices(g));
  long flow = kolmogorov_max_flow(g ,s, t);

  std::cout << "c  The total flow:" << std::endl;
  std::cout << "s " << flow << std::endl << std::endl;

  std::cout << "c flow values:" << std::endl;
  graph_traits < Graph >::vertex_iterator u_iter, u_end;
  graph_traits < Graph >::out_edge_iterator ei, e_end;
  for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
    for (tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei)
      if (capacity[*ei] > 0)
        std::cout << "f " << *u_iter << " " << target(*ei, g) << " "
          << (capacity[*ei] - residual_capacity[*ei]) << std::endl;

  return EXIT_SUCCESS;
}
