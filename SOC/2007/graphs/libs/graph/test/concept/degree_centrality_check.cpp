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
        typedef descriptor_archetype Vertex;
        typedef incidence_graph_archetype<
                Vertex,
                undirected_tag,
                allow_parallel_edge_tag
            > IncidenceGraph;
        typedef vertex_list_graph_archetype<
                Vertex,
                undirected_tag,
                allow_parallel_edge_tag,
                IncidenceGraph
            > Graph;
        typedef writable_property_map_archetype<Vertex, size_t> CentralityMap;

        Graph& g = static_object<Graph>::get();
        CentralityMap cm;

        degree_centrality(g, cm);
        degree_centrality(g, cm, measure_influence(g));
    }

    // There isn't a bidirectional graph archetype so I don't really know
    // how to test this. Even If I build one, it won't compile very
    // easily.
    {
        typedef descriptor_archetype Vertex;
        typedef bidirectional_graph_archetype<
                Vertex,
                directed_tag,
                allow_parallel_edge_tag
            > BidirectionalGraph;
        typedef vertex_list_graph_archetype<
                Vertex,
                directed_tag,
                allow_parallel_edge_tag,
                BidirectionalGraph
            > graph_type;
        typedef read_write_property_map_archetype<Vertex, size_t> CentralityMap;

        // These don't actually work - apparently, it's not very easy
        // to generate archetypes across concept hierarchies and have
        // them concept-checked very easily.

        // graph_type& g = static_object<graph_type>::get();
        // centrality_map_type cm;

        // degree_centrality(g, cm);
        // degree_centrality(g, cm, measure_prestige(g));
    }

    return 0;
}
