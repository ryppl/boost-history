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

#ifndef BOOST_GRAPH_DIFFERENCE_HPP
#define BOOST_GRAPH_DIFFERENCE_HPP

#include <boost/config.hpp>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/type_traits/conversion_traits.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void difference(const VertexListGraph& G1, const VertexListGraph& G2, MutableGraph& G)
  {
    typename graph_traits < MutableGraph >::edge_iterator ei, ei_end;
    // Don't forget: it is probably better to add only edges that are not in G2 instead of add all and remove them later

    copy_graph(G1, G);
    for (tie(ei, ei_end) = edges(G2); ei != ei_end; ++ei) {
      if (edge(source(*ei, G2), target(*ei, G2), G).second == true)
        remove_edge(source(*ei, G), target(*ei, G), G);
    }

  }

} // namespace boost

#endif // BOOST_GRAPH_DIFFERENCE_HPP
