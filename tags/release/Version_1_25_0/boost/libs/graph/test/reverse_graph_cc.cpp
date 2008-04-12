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
#include <boost/graph/graph_archetypes.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/reverse_graph.hpp>

int main(int,char*[])
{
  using namespace boost;
  // Check const reverse_graph
  {
    typedef adjacency_list< vecS, vecS, bidirectionalS, 
      property<vertex_color_t, int>,
      property<edge_weight_t, int>
    > AdjList;
    typedef reverse_graph<AdjList> Graph;
    function_requires< VertexListGraphConcept<Graph> >();
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    function_requires< ReadablePropertyGraphConcept<Graph, Vertex, vertex_color_t> >();
    function_requires< ReadablePropertyGraphConcept<Graph, Edge, edge_weight_t> >();
  }
  // Check non-const reverse_graph
  {
    typedef adjacency_list< vecS, vecS, bidirectionalS, 
      property<vertex_color_t, int>,
      property<edge_weight_t, int>
    > AdjList;
    typedef reverse_graph<AdjList,AdjList&> Graph;
    function_requires< VertexListGraphConcept<Graph> >();
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    function_requires< PropertyGraphConcept<Graph, Vertex, vertex_color_t> >();
    function_requires< PropertyGraphConcept<Graph, Edge, edge_weight_t> >();
  }
  return 0;
}
