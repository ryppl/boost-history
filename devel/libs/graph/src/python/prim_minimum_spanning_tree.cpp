// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include "graph.hpp"
#include "digraph.hpp"
#include <boost/python.hpp>
#include "dijkstra_visitor.hpp"

namespace boost { namespace graph { namespace python {

template<typename Graph>
void
prim_minimum_spanning_tree
  (Graph& g, typename Graph::Vertex s,
   const vector_property_map<typename Graph::Vertex,
                             typename Graph::VertexIndexMap>* in_predecessor,
   const vector_property_map<double, typename Graph::VertexIndexMap>* in_distance,
   const vector_property_map<double, typename Graph::EdgeIndexMap>* in_weight,
   const dijkstra_visitor<Graph>& visitor)
{
  typedef vector_property_map<typename Graph::Vertex,
                              typename Graph::VertexIndexMap>
    PredecessorMap;
  typedef vector_property_map<double, typename Graph::VertexIndexMap>
    DistanceMap;
  typedef vector_property_map<double, typename Graph::EdgeIndexMap>
    WeightMap;

  PredecessorMap predecessor = 
    in_predecessor? *in_predecessor
    : PredecessorMap(g.num_vertices(), g.get_vertex_index_map());

  DistanceMap distance = 
    in_distance? *in_distance
    : DistanceMap(g.num_vertices(), g.get_vertex_index_map());

  WeightMap weight = in_weight? *in_weight 
                   : g.template get_edge_map<double>("weight");

  typedef typename dijkstra_visitor<Graph>::default_arg default_visitor;
  bool has_default_visitor = dynamic_cast<const default_visitor*>(&visitor);

  if (!has_default_visitor) {
    boost::prim_minimum_spanning_tree
      (g, 
       predecessor,
       root_vertex(s).
       vertex_index_map(g.get_vertex_index_map()).
       visitor(typename dijkstra_visitor<Graph>::ref(visitor)).
       distance_map(distance).
       weight_map(weight));
  } else {
    boost::prim_minimum_spanning_tree
      (g, 
       predecessor,
       root_vertex(s).
       vertex_index_map(g.get_vertex_index_map()).
       distance_map(distance).
       weight_map(weight));
  }
}

template<typename Graph>
void export_prim_minimum_spanning_tree_in_graph()
{
  dijkstra_visitor<Graph>::declare("DijkstraVisitor", 
                                   "DefaultDijkstraVisitor");
}

void export_prim_minimum_spanning_tree()
{
  using boost::python::arg;
  using boost::python::def;

  def("prim_minimum_spanning_tree", &prim_minimum_spanning_tree<Graph>,
      (arg("graph"), arg("root_vertex"),
       arg("predecessor_map") = 
         (vector_property_map<Graph::Vertex, Graph::VertexIndexMap>*)0,
       arg("distance_map") = 
         (vector_property_map<double, Graph::VertexIndexMap>*)0,
       arg("weight_map") = 
         (vector_property_map<double, Graph::EdgeIndexMap>*)0,
       arg("visitor") = dijkstra_visitor<Graph>::default_arg()));

  def("prim_minimum_spanning_tree", &prim_minimum_spanning_tree<Digraph>,
      (arg("graph"), arg("root_vertex"),
       arg("predecessor_map") = 
         (vector_property_map<Digraph::Vertex, Digraph::VertexIndexMap>*)0,
       arg("distance_map") = 
         (vector_property_map<double, Digraph::VertexIndexMap>*)0,
       arg("weight_map") = 
         (vector_property_map<double, Digraph::EdgeIndexMap>*)0,
       arg("visitor") = dijkstra_visitor<Digraph>::default_arg()));
}

template void export_prim_minimum_spanning_tree_in_graph<Graph>();
template void export_prim_minimum_spanning_tree_in_graph<Digraph>();

} } } // end namespace boost::graph::python
