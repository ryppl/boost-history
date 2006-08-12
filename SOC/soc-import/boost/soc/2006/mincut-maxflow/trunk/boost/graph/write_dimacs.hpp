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

namespace boost {

template <class Graph, class CapacityMap, class IndexMap>
void write_dimacs_max_flow(const Graph& g,
                         CapacityMap capacity, 
                         IndexMap idx,
                         typename graph_traits<Graph>::vertex_descriptor src,
                         typename graph_traits<Graph>::vertex_descriptor sink,
                         std::ostream& out)
{
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename graph_traits<Graph>::edges_size_type edges_size_type;
  typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
   
  out << "c DIMACS max-flow file generated from boost::write_dimacs_max_flow" << std::endl;
  out << "p max " << num_vertices(g) << " " << num_edges(g) << std::endl; //print problem description "max" and number of verts and edges
  out << "n " << get(idx, src) + 1 << " s" << std::endl;; //say which one is source
  out << "n " << get(idx, sink) + 1 << " t" << std::endl; //say which one is sink
  
  //output the edges
  edge_iterator ei, e_end;
  for(tie(ei,e_end) = edges(g); ei!=e_end; ++ei){
    out << "a " << idx[ source(*ei, g) ] + 1 << " " << idx[ target(*ei, g) ] + 1 << " " << get(capacity,*ei) << std::endl;
  }
}

} // namespace boost
