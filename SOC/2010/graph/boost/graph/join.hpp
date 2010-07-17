//
//=======================================================================
// Copyright 1997-2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Lie-Quan Lee, Andrew Lumsdaine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_JOIN_HPP
#define BOOST_GRAPH_JOIN_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void graph_join(const VertexListGraph& G1, const VertexListGraph& G2, MutableGraph& G)
  {
    typename graph_traits < MutableGraph >::vertex_iterator vi, vi_end;
    typename graph_traits < MutableGraph >::vertex_iterator ui, ui_end;
    copy_graph(G1, G);
    copy_graph(G2, G);
    
    for (tie(vi, vi_end) = vertices(G1); vi != vi_end; ++vi) {
      for (tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui) {
        if (vertex(*ui, G1) == true) // not working
          std::cout << "Vertex: " << *ui << " is in G1" << std::endl;
        else
          std::cout << "Vertex: " << *ui << " is NOT in G1 (G - G1)" << std::endl;
      }
    }

  }

} // namespace boost

#endif // BOOST_GRAPH_JOIN_HPP
