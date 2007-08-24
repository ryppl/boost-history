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
    // This is the base type for all graphs.
    typedef graph_archetype<
            undirected_tag,
            allow_parallel_edge_tag
        > Graph;
    {
        typedef vertex_list_graph_archetype<Graph> VertexListGraph;
        typedef graph_traits<VertexListGraph>::vertex_descriptor Vertex;
        typedef writable_property_map_archetype<Vertex, float> CentralityMap;
        typedef readable_property_map_archetype<Vertex, size_t> DistanceMap;
        typedef readable_property_map_archetype<Vertex, DistanceMap> DistanceMatrix;
        typedef distance_measure_archetype<VertexListGraph, size_t, float> Measure;

        VertexListGraph& g = static_object<VertexListGraph>::get();
        CentralityMap cm;
        DistanceMatrix dm;
        Measure m;

        all_closeness_centralities(g, dm, cm);
        all_closeness_centralities(g, dm, cm, m);
    }

    {
        typedef vertex_list_graph_archetype<Graph> VertexListGraph;
        typedef graph_traits<VertexListGraph>::vertex_descriptor Vertex;
        typedef readable_property_map_archetype<Vertex, size_t> DistanceMap;
        typedef distance_measure_archetype<VertexListGraph, size_t, float> Measure;

        VertexListGraph& g = static_object<VertexListGraph>::get();
        DistanceMap dm;
        Measure m;

        closeness_centrality(g, dm);
        closeness_centrality(g, dm, m);
    }

    return 0;
}
