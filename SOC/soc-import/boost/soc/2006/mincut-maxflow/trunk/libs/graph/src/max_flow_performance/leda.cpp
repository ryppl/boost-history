//=======================================================================
// Copyright 2006 Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <iostream>
#include <string>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/graph/graph_alg.h>
#include <LEDA/graph/dimacs_graph.h>
#include <LEDA/graph/templates/max_flow.h>

#include <sys/time.h>
#include <sys/resource.h>

//#define USE_TEMPLATED_VERSION 1

float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

int
main()
{
#ifdef USE_TEMPLATED_VERSION
  typedef long tEdgeVal;
#else
  typedef int tEdgeVal;
#endif
  
  using namespace leda;
  graph g;
  node source, sink;
  edge_array<tEdgeVal> capacity(g);
  read_dimacs_mf(std::cin, g, source, sink, capacity); 
  
  edge_array<tEdgeVal> flow(g);
  float time = timer();
#ifdef USE_TEMPLATED_VERSION 
  tEdgeVal flow_value = MAX_FLOW_T(g, source, sink, capacity, flow);
#else
  tEdgeVal flow_value = MAX_FLOW(g, source, sink, capacity, flow);
#endif
  double elapsed = timer() - time;
#ifdef USE_TEMPLATED_VERSION
  assert(CHECK_MAX_FLOW_T(g, source, sink , capacity, flow));
#else
  assert(CHECK_MAX_FLOW(g, source, sink , capacity, flow));
#endif   
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout << "flow: " << flow_value << std::endl;
  std::cout << "time: " << elapsed << std::endl;

  return EXIT_SUCCESS;
}
