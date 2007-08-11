// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/closeness_centrality.hpp>

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
        typedef writable_property_map_archetype<Vertex, float> CentralityMap;
        typedef readable_property_map_archetype<Vertex, size_t> DistanceMap;
        typedef readable_property_map_archetype<Vertex, DistanceMap> DistanceMatrix;

        Graph& g = static_object<Graph>::get();
        CentralityMap cm;
        DistanceMatrix dm;

        closeness_centrality(g, dm, cm);
    }

    return 0;
}
