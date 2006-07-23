//=======================================================================
// Copyright 2006 
// Authors: Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

/*
  Writes maximal flow problem in extended DIMACS format to an OutputIterator
  Vertex indices are read from an IndexMap and shiftet by 1.
  so their new range is [1..num_vertices(g)]
*/

/* ----------------------------------------------------------------- */

#include <vector>
#include <string>
#include <ostream>
#include <sstream>

namespace boost {

template <class Graph, class CapacityMap, class IndexMap, class OutputIterator>
void write_dimacs_max_flow(Graph& g,
                         CapacityMap capacity, 
                         IndexMap idx,
                         typename graph_traits<Graph>::vertex_descriptor& src,
                         typename graph_traits<Graph>::vertex_descriptor& sink,
                         OutputIterator out)
{
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename graph_traits<Graph>::edges_size_type edges_size_type;
  typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
  
  if( ! num_vertices(g) ){
    return;
  }
  
  *out++ = "c DIMACS max-flow file generated from boost::write_dimacs_max_flow";
  std::stringstream s;
  s << "p max " << num_vertices(g) << " " << num_edges(g); //print problem description "max" and number of verts and edges
  *out++ = s.str();
  
  s.str("");
  s << "n " << get(idx, src) + 1 << " s"; //say which one is source
  *out++ = s.str();
  
  s.str("");
  s << "n " << get(idx, sink) + 1 << " t"; //say which one is sink
  *out++ = s.str();
  
  //now we start writing the edges
  edge_iterator ei, e_end;
  for(tie(ei,e_end) = edges(g); ei!=e_end; ++ei){
    s.str("");
    s << "a " << idx[ source(*ei, g) ] + 1 << " " << idx[ target(*ei, g) ] + 1 << " " << get(capacity,*ei);
    *out++ = s.str();
  }
}

} // namespace boost
