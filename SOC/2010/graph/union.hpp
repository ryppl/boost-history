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

#ifndef BOOST_GRAPH_UNION_HPP
#define BOOST_GRAPH_UNION_HPP

#include <boost/config.hpp>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/type_traits/conversion_traits.hpp>

namespace boost {

  template <class VertexListGraph, class MutableGraph> 
  void disjoint_union(const VertexListGraph& G1, const VertexListGraph& G2, MutableGraph& G)
  {
    copy_graph(G1, G);
    copy_graph(G2, G);
  }

} // namespace boost

#endif // BOOST_GRAPH_UNION_HPP
