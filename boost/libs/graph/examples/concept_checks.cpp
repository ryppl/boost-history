//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/stanford_graph.hpp>

//
// These concept checks verify that the BGL classes implement the
// complete interfaces defined in the graph concepts. The concept
// checks are also useful for verifying whether user-defined graph
// classes satisfy the desired graph concepts. Note that these concept
// checks by themselves do not verify whether the classes model the
// concepts, for there are run-time requirements that are not checked
// here.
//

int
main(int,char*[])
{
  using namespace boost;
  {
    typedef adjacency_list<vecS, vecS, directedS, 
      property<vertex_color_t, int>,
      property<edge_weight_t, int>
    > Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    BOOST_FUNCTION_REQUIRES(Graph, VertexAndEdgeListGraphConcept);
    BOOST_FUNCTION_REQUIRES(Graph, MutableGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Vertex, vertex_color_t, PropertyGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Edge, edge_weight_t, PropertyGraphConcept);
    // the builtin id property is readable but not writable
    typedef property_map<Graph, vertex_index_t>::const_type IndexMap;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    BOOST_FUNCTION_REQUIRES2(IndexMap, Vertex, ReadablePropertyMapConcept);
  }
  {
    typedef adjacency_list<vecS, vecS, bidirectionalS, 
      property<vertex_color_t, int>,
      property<edge_weight_t, int>
    > Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    BOOST_FUNCTION_REQUIRES(Graph, VertexAndEdgeListGraphConcept);
    BOOST_FUNCTION_REQUIRES(Graph, BidirectionalGraphConcept);
    BOOST_FUNCTION_REQUIRES(Graph, MutableGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Vertex, vertex_color_t, PropertyGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Edge, edge_weight_t, PropertyGraphConcept);
    // the builtin id property is readable but not writable
    typedef property_map<Graph, vertex_index_t>::const_type IndexMap;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    BOOST_FUNCTION_REQUIRES2(IndexMap, Vertex, ReadablePropertyMapConcept);
  }
  {
    typedef adjacency_list< listS, listS, directedS, 
      property<vertex_color_t, int>,
      property<edge_weight_t, int>
    > Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    BOOST_FUNCTION_REQUIRES(Graph, VertexAndEdgeListGraphConcept);
    BOOST_FUNCTION_REQUIRES(Graph, MutableGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Vertex, vertex_color_t, PropertyGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Edge, edge_weight_t, PropertyGraphConcept);
  }
  {
    typedef adjacency_list< listS, listS, undirectedS, 
      property<vertex_color_t, int>,
      property<edge_weight_t, int>
    > Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    BOOST_FUNCTION_REQUIRES(Graph, VertexAndEdgeListGraphConcept);
    BOOST_FUNCTION_REQUIRES(Graph, MutableGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Vertex, vertex_color_t, PropertyGraphConcept);
    BOOST_FUNCTION_REQUIRES3(Graph, Edge, edge_weight_t, PropertyGraphConcept);
  }
  {
    typedef std::pair<int,int> E;
    typedef edge_list<E*,E,ptrdiff_t> EdgeList;
    BOOST_FUNCTION_REQUIRES(EdgeList, EdgeListGraphConcept);
  }
  { // Stanford GraphBase Graph
    typedef Graph* Graph;
    BOOST_FUNCTION_REQUIRES(Graph, VertexListGraphConcept);
  }
  return 0;
}
