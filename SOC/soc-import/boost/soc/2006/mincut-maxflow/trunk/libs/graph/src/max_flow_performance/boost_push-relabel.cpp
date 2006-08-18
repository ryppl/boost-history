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
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>
//use DIMACS max-flow file as stdin
#include <sys/time.h>
#include <sys/resource.h>

float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

int
main()
{
  using namespace boost;
  typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
  typedef adjacency_list < vecS, vecS, directedS,
  no_property,
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
  float time=timer();
  long flow = push_relabel_max_flow(g, s, t);
  double elapsed= timer()-time;
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout << "flow: " << flow <<std::endl;
  std::cout << "time: " << elapsed <<std::endl;
  
  return EXIT_SUCCESS;
}
