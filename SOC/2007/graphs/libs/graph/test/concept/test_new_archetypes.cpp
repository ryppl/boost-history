// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "new_archetypes.hpp"
#include <boost/graph/graph_traits.hpp>

using namespace std;
using namespace boost;

bool is_incident(const incidence_graph_tag&)
{ return true; }

bool is_bidirectional(const bidirectional_graph_tag&)
{ return true; }

bool is_adjacent(const adjacency_graph_tag&)
{ return true; }

bool is_vertex_list(const vertex_list_graph_tag&)
{ return true; }

bool is_edge_list(const edge_list_graph_tag&)
{ return true; }

int
main(int argc, char *argv[])
{
    // test the base-most graph concept
    {
        typedef graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > Graph;
        typedef graph_traits<Graph>::vertex_descriptor Vertex;
        typedef graph_traits<Graph>::edge_descriptor Edge;
        typedef graph_traits<Graph>::directed_category Directed;
        typedef graph_traits<Graph>::edge_parallel_category Parallel;
        typedef graph_traits<Graph>::traversal_category Traversal;

        graph_traits<Graph>::null_vertex();
    }

    // an incidence graph allows access to the out-edges of a vertex
    {
        typedef incidence_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > IncidenceGraph;
        typedef IncidenceGraph::vertex_descriptor Vertex;
        typedef IncidenceGraph::edge_descriptor Edge;
        typedef IncidenceGraph::directed_category Directed;
        typedef IncidenceGraph::edge_parallel_category Parallel;
        typedef IncidenceGraph::traversal_category Traversal;
        typedef IncidenceGraph::degree_size_type Degree;
        typedef IncidenceGraph::out_edge_iterator OutEdgeIterator;

        is_incident(Traversal());
        IncidenceGraph& g = static_object<IncidenceGraph>::get();
        Vertex v = static_object<Vertex>::get();
        out_edges(v, g);
        out_degree(v, g);
        Edge e = static_object<Edge>::get();
        source(e, g);
        target(e, g);
    }

    // bidirectional graphs provide accesst to both the out and in edges
    {
        typedef bidirectional_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > BidirectionalGraph;
        typedef BidirectionalGraph::vertex_descriptor Vertex;
        typedef BidirectionalGraph::edge_descriptor Edge;
        typedef BidirectionalGraph::directed_category Directed;
        typedef BidirectionalGraph::edge_parallel_category Parallel;
        typedef BidirectionalGraph::traversal_category Traversal;
        typedef BidirectionalGraph::degree_size_type Degree;
        typedef BidirectionalGraph::out_edge_iterator OutEdgeIterator;
        typedef BidirectionalGraph::in_edge_iterator InEdgeIterator;

        is_incident(Traversal());
        is_bidirectional(Traversal());
        BidirectionalGraph& g = static_object<BidirectionalGraph>::get();
        Vertex v = static_object<Vertex>::get();
        out_edges(v, g);
        out_degree(v, g);
        in_edges(v, g);
        in_degree(v, g);
        Edge e = static_object<Edge>::get();
        source(e, g);
        target(e, g);
    }

    // This instantiates the non-compound adjacency list archetype
    // It shouldn't be implementing any of the other graph concepts
    {
        typedef adjacency_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > AdjacencyGraph;
        typedef AdjacencyGraph::vertex_descriptor Vertex;
        typedef AdjacencyGraph::edge_descriptor Edge;
        typedef AdjacencyGraph::directed_category Directed;
        typedef AdjacencyGraph::edge_parallel_category Parallel;
        typedef AdjacencyGraph::traversal_category Traversal;
        typedef AdjacencyGraph::adjacency_iterator AdjacencyIterator;

        is_adjacent(Traversal());
        AdjacencyGraph& g = static_object<AdjacencyGraph>::get();
        Vertex v = static_object<Vertex>::get();
        adjacent_vertices(v, g);
    }

    // Make a bidirectional adjacency list (this is like part of a
    // bidirected graph).
    {
        typedef bidirectional_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > BidirectionalGraph;
        typedef adjacency_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag,
                BidirectionalGraph
            > AdjacencyGraph;
        typedef AdjacencyGraph::traversal_category Traversal;
        typedef AdjacencyGraph::vertex_descriptor Vertex;
        typedef AdjacencyGraph::edge_descriptor Edge;

        // test to see if these hold
        is_incident(Traversal());
        is_bidirectional(Traversal());
        is_adjacent(Traversal());
        AdjacencyGraph& g = static_object<AdjacencyGraph>::get();
        Vertex v = static_object<Vertex>::get();
        out_edges(v, g);
        out_degree(v, g);
        in_edges(v, g);
        in_degree(v, g);
        adjacent_vertices(v, g);
        Edge e = static_object<Edge>::get();
        source(e, g);
        target(e, g);
    }

    // test a solo vertex list graph
    {
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > VertexListGraph;
        typedef VertexListGraph::vertex_descriptor Vertex;
        typedef VertexListGraph::edge_descriptor Edge;
        typedef VertexListGraph::directed_category Directed;
        typedef VertexListGraph::edge_parallel_category Parallel;
        typedef VertexListGraph::traversal_category Traversal;
        typedef VertexListGraph::vertices_size_type Vertices;
        typedef VertexListGraph::vertex_iterator VertexIterator;

        is_vertex_list(Traversal());
        VertexListGraph& g = static_object<VertexListGraph>::get();
        vertices(g);
        num_vertices(g);
    }

    // test an incident vertex list graph (common requirements for a
    // number of measures).
    {
        typedef incidence_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > IncidenceGraph;
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag,
                IncidenceGraph
            > VertexListGraph;
        typedef VertexListGraph::vertex_descriptor Vertex;
        typedef VertexListGraph::edge_descriptor Edge;
        typedef VertexListGraph::directed_category Directed;
        typedef VertexListGraph::edge_parallel_category Parallel;
        typedef VertexListGraph::traversal_category Traversal;
        typedef VertexListGraph::vertices_size_type Vertices;
        typedef VertexListGraph::vertex_iterator VertexIterator;

        is_incident(Traversal());
        is_vertex_list(Traversal());
        VertexListGraph& g = static_object<VertexListGraph>::get();
        vertices(g);
        num_vertices(g);
        Vertex v = static_object<Vertex>::get();
        out_edges(v, g);
        out_degree(v, g);
        Edge e = static_object<Edge>::get();
        source(e, g);
        target(e, g);
    }

    // test a solo edge list graph
    {
        typedef edge_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > EdgeListGraph;
        typedef EdgeListGraph::vertex_descriptor Vertex;
        typedef EdgeListGraph::edge_descriptor Edge;
        typedef EdgeListGraph::directed_category Directed;
        typedef EdgeListGraph::edge_parallel_category Parallel;
        typedef EdgeListGraph::traversal_category Traversal;
        typedef EdgeListGraph::vertices_size_type Edges;
        typedef EdgeListGraph::edge_iterator EdgeIterator;

        is_edge_list(Traversal());
        EdgeListGraph& g = static_object<EdgeListGraph>::get();
        edges(g);
        num_edges(g);
    }

    // test a bidirected, vertex/edge list graph
    {
        typedef bidirectional_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > BidirectionalGraph;
        typedef edge_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag,
                BidirectionalGraph
            > EdgeListGraph;
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag,
                EdgeListGraph
            > VertexListGraph;
        typedef VertexListGraph::vertex_descriptor Vertex;
        typedef VertexListGraph::edge_descriptor Edge;
        typedef VertexListGraph::directed_category Directed;
        typedef VertexListGraph::edge_parallel_category Parallel;
        typedef VertexListGraph::traversal_category Traversal;
        typedef VertexListGraph::vertices_size_type Edges;
        typedef VertexListGraph::edge_iterator EdgeIterator;

        is_incident(Traversal());
        is_bidirectional(Traversal());
        is_vertex_list(Traversal());
        is_edge_list(Traversal());
        VertexListGraph& g = static_object<VertexListGraph>::get();
        vertices(g);
        num_vertices(g);
        edges(g);
        num_edges(g);
        Vertex v = static_object<Vertex>::get();
        out_edges(v, g);
        out_degree(v, g);
        in_edges(v, g);
        in_degree(v, g);
        Edge e = static_object<Vertex>::get();
        source(e, g);
        target(e, g);
    }

    // test a mutable property graph
    {
        typedef bidirectional_graph_archetype <
                undirected_tag,
                allow_parallel_edge_tag
            > BidirectionalGraph;
        typedef property_graph_archetype <
                BidirectionalGraph,
                vertex_index_t,     // the property type
                size_t              // the value type of the property (should be archetype?)
            > IndexedGraph;
        typedef IndexedGraph::vertex_descriptor Vertex;

        IndexedGraph& g = static_object<IndexedGraph>::get();
        Vertex v = static_object<Vertex>::get();
        get(vertex_index, g);
        get(vertex_index, g, v);
        put(vertex_index, g, v, size_t());
    }

    // test a mutable property graph
    {
        typedef bidirectional_graph_archetype <
                undirected_tag,
                allow_parallel_edge_tag
            > BidirectionalGraph;
        typedef property_graph_archetype <
                BidirectionalGraph,
                vertex_index_t,     // the property type
                size_t              // the value type of the property (should be archetype?)
            > IndexedGraph;
        typedef IndexedGraph::vertex_descriptor Vertex;

        const IndexedGraph& g = static_object<IndexedGraph>::get();
        Vertex v = static_object<Vertex>::get();
        get(vertex_index, g);
        get(vertex_index, g, v);

        // This causes a compiler error if uncommented (as it should!)
        // put(vertex_index, g, v, size_t());
    }

    return 0;
}
