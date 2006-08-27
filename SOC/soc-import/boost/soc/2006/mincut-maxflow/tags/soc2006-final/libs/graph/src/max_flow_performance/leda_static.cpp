/*******************************************************************************
+
+  LEDA 5.1  
+
+
+  mfs.c
+
+
+  Copyright (c) 1995-2005
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/

// $Revision: 1.1.1.1 $  $Date: 2005/09/12 10:38:09 $

#define LEDA_CHECKING_OFF

#include <LEDA/graph/graph.h>
#include <LEDA/graph/static_graph.h>
#include <LEDA/graph/dimacs_graph.h>
#include <LEDA/system/assert.h>

#include <LEDA/graph/node_slot.h>
#include <LEDA/graph/edge_slot.h>

#include <LEDA/graph/templates/max_flow.h>

#include <sys/time.h>
#include <sys/resource.h>

#include <iostream>

using namespace leda;
using namespace std;

float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

int main(int argc, char** argv)
{
  typedef static_graph<opposite_graph,data_slots<3>,data_slots<2> > st_graph;
  typedef st_graph::node node;
  typedef st_graph::edge edge;


  st_graph G;
  node s=0;
  node t=0;

  edge_slot<int,st_graph,0>   cap(G);
  edge_slot<int,st_graph,1>  flow(G);

  typedef node_slot<node,st_graph,0> succ_array;
  typedef node_slot<int, st_graph,1> excess_array;
  typedef node_slot<int, st_graph,2> dist_array;

  dimacs_mf D(cin);
  D.translate(G,s,t,cap);

  
  float time = timer();
  max_flow<int,st_graph,succ_array,excess_array,dist_array> mf;
  mf.run(G,s,t,cap,flow);
  long flow_value = mf.flow(); 
  double elapsed = timer() - time;
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout << "flow: " << flow_value << std::endl;
  std::cout << "time: " << elapsed << std::endl;

  return 0;
}




