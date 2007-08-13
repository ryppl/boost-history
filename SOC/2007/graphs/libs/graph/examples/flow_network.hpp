// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_EXAMPLE_FLOW_NETWORK_HPP
#define BOOST_GRAPH_EXAMPLE_FLOW_NETWORK_HPP

#include <string>
#include <map>

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/constant_property_map.hpp>

//[flow_network_types
struct Actor
{
    std::string name;
};

typedef boost::directed_graph<Actor> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
//]

typedef boost::exterior_vertex_property<Graph, int> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;

typedef boost::constant_property_map<Edge, int> WeightMap;

typedef boost::exterior_vertex_property<Graph, unsigned> CentralityProperty;
typedef CentralityProperty::container_type CentralityContainer;
typedef CentralityProperty::map_type CentralityMap;

typedef boost::exterior_vertex_property<Graph, float> ClosenessProperty;
typedef ClosenessProperty::container_type ClosenessContainer;
typedef ClosenessProperty::map_type ClosenessMap;

#endif
