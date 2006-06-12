//=======================================================================
// Copyright 2006 University of Mannheim.
// Author: Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef BOOST_KOLMOGOROV_MAX_FLOW_HPP
#define BOOST_KOLMOGOROV_MAX_FLOW_HPP

#include <boost/config.hpp>
#include <cassert>
#include <vector>
#include <list>
#include <iosfwd>
#include <algorithm> // for std::min and std::max

#include <boost/pending/queue.hpp>
#include <boost/limits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/named_function_params.hpp>

namespace boost {

  namespace detail {
  } //namespace detail

  template <class Graph, 
            class CapacityEdgeMap, class ResidualCapacityEdgeMap,
            class ReverseEdgeMap, class VertexIndexMap>
//   typename property_traits<CapacityEdgeMap>::value_type
						void
  kolmogorov_max_flow
						
    (Graph& g, 
     typename graph_traits<Graph>::vertex_descriptor src,
     typename graph_traits<Graph>::vertex_descriptor sink,
     CapacityEdgeMap cap, ResidualCapacityEdgeMap res,
     ReverseEdgeMap rev, VertexIndexMap index_map)
  {
    typedef typename property_traits<CapacityEdgeMap>::value_type FlowValue;
    
  }
  
} // namespace boost

#endif // BOOST_KOLMOGOROV_MAX_FLOW_HPP

