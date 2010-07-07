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

#ifndef BOOST_GRAPH_SUM_HPP
#define BOOST_GRAPH_SUM_HPP

#include <boost/config.hpp>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/type_traits/conversion_traits.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void graph_sum(const VertexListGraph& G1, const VertexListGraph& G2, MutableGraph& G)
  {
    typename graph_traits < MutableGraph >::edge_iterator ei, ei_end;

    copy_graph(G1, G);
    //    std::cout << "Graph sum - edges: ";
    for (tie(ei, ei_end) = edges(G2); ei != ei_end; ++ei) {
      //      typename property_map<MutableGraph, vertex_index_t>::type index = get(vertex_index, G);
      //      std::cout << "(" << index[source(*ei, G)] << "," << index[target(*ei, G)] << ") ";
      add_edge(source(*ei, G), target(*ei, G), G);
    }
    // std::cout << std::endl;
  }

} // namespace boost

#endif // BOOST_GRAPH_SUM_HPP
