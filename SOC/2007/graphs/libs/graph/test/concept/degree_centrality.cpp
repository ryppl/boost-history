// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/degree_centrality.hpp>

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    {
        typedef incidence_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > IncidenceGraph;
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag,
                IncidenceGraph
            > Graph;
        typedef Graph::vertex_descriptor Vertex;
        typedef writable_property_map_archetype<Vertex, size_t> CentralityMap;
        typedef degree_measure_archetype<Graph> Measure;

        Graph& g = static_object<Graph>::get();
        CentralityMap cm;
        Measure m;

        all_degree_centralities(g, cm);
        all_degree_centralities(g, cm, m);
    }

    {
        typedef vertex_list_graph_archetype<
                directed_tag,
                allow_parallel_edge_tag
            > VertexListGraph;
        typedef bidirectional_graph_archetype<
                directed_tag,
                allow_parallel_edge_tag,
                VertexListGraph
            > Graph;
        typedef Graph::vertex_descriptor Vertex;
        typedef read_write_property_map_archetype<Vertex, size_t> CentralityMap;
        typedef degree_measure_archetype<Graph> Measure;

        Graph& g = static_object<Graph>::get();
        CentralityMap cm;
        Measure m;

        all_influence_values(g, cm);
        all_prestige_values(g, cm);
        all_degree_centralities(g, cm, m);
    }

    {
        typedef incidence_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > Graph;
        typedef Graph::vertex_descriptor Vertex;
        typedef degree_measure_archetype<Graph> Measure;

        Graph& g = static_object<Graph>::get();
        Vertex v = static_object<Vertex>::get();
        Measure m;

        degree_centrality(g, v);
        degree_centrality(g, v, m);
    }

    {
        typedef bidirectional_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > Graph;
        typedef Graph::vertex_descriptor Vertex;
        typedef degree_measure_archetype<Graph> Measure;

        Graph& g = static_object<Graph>::get();
        Vertex v = static_object<Vertex>::get();
        Measure m;

        influence(g, v);
        prestige(g, v);
        degree_centrality(g, v, m);
    }

    return 0;
}
