// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/degree_centrality.hpp>

using namespace boost;

int
main(int argc, char *argv[])
{
    {
        typedef descriptor_archetype vertex_type;
        typedef incidence_graph_archetype<
                vertex_type,
                undirected_tag,
                allow_parallel_edge_tag
            > incidence_graph_type;
        typedef vertex_list_graph_archetype<
                vertex_type,
                undirected_tag,
                allow_parallel_edge_tag,
                incidence_graph_type
            > graph_type;
        typedef graph_traits<graph_type>::degree_size_type degree_type;
        typedef read_write_property_map_archetype<
                vertex_type,
                degree_type
            > centrality_map_type;

        graph_type& g = static_object<graph_type>::get();
        centrality_map_type cm;

        degree_centrality(g, cm);
        degree_centrality(g, cm, measure_influence(g));
    }

    // There isn't a bidirectional graph archetype so I don't really know
    // how to test this. Even If I build one, it won't compile very
    // easily.
    {
        typedef descriptor_archetype vertex_type;
        typedef bidirectional_graph_archetype<
                vertex_type,
                directed_tag,
                allow_parallel_edge_tag
            > bidirectional_graph_type;
        typedef vertex_list_graph_archetype<
                vertex_type,
                directed_tag,
                allow_parallel_edge_tag,
                bidirectional_graph_type
            > graph_type;
        typedef graph_traits<graph_type>::degree_size_type degree_type;
        typedef read_write_property_map_archetype<
                vertex_type,
                degree_type
            > centrality_map_type;

        graph_type& g = static_object<graph_type>::get();
        centrality_map_type cm;

        // These don't actually work - apparently, it's not very easy
        // to generate archetypes across concept hierarchies and have
        // them concept-checked very easily.

        // degree_centrality(g, cm);
        // degree_centrality(g, cm, measure_prestige(g));
    }

    return 0;
}
