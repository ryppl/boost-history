// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/clustering_coefficient.hpp>

#include <iostream>

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    typedef graph_archetype<
            undirected_tag,
            allow_parallel_edge_tag
        > Graph;
    {
        typedef vertex_list_graph_archetype<Graph> VertexListGraph;
        typedef incidence_graph_archetype<VertexListGraph> IncidenceGraph;
        typedef adjacency_graph_archetype<IncidenceGraph> AdjacencyGraph;
        typedef adjacency_matrix_archetype<AdjacencyGraph> AdjacencyMatrix;
        typedef graph_traits<AdjacencyMatrix>::vertex_descriptor Vertex;
        typedef writable_property_map_archetype<Vertex, float> ClusteringMap;

        AdjacencyMatrix& g = static_object<AdjacencyMatrix>::get();
        Vertex v = static_object<Vertex>::get();
        ClusteringMap cm;

        all_clustering_coefficients(g, cm);
    }

    {
        typedef incidence_graph_archetype<Graph> IncidenceGraph;
        typedef adjacency_graph_archetype<IncidenceGraph> AdjacencyGraph;
        typedef adjacency_matrix_archetype<AdjacencyGraph> AdjacencyMatrix;
        typedef graph_traits<AdjacencyMatrix>::vertex_descriptor Vertex;
        typedef writable_property_map_archetype<Vertex, float> ClusteringMap;

        AdjacencyMatrix& g = static_object<AdjacencyMatrix>::get();
        Vertex v = static_object<Vertex>::get();
        ClusteringMap cm;

        num_paths_through_vertex(g, v);
        num_triangles_on_vertex(g, v);
        clustering_coefficient(g, v);
    }

    {
        typedef vertex_list_graph_archetype<Graph> VertexListGraph;
        typedef graph_traits<VertexListGraph>::vertex_descriptor Vertex;
        typedef readable_property_map_archetype<Vertex, float> ClusteringMap;

        VertexListGraph& g = static_object<VertexListGraph>::get();
        ClusteringMap cm;
        mean_clustering_coefficient(g, cm);
    }

    return 0;
}
