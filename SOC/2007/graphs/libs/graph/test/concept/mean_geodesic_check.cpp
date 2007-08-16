// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/geodesic_distance.hpp>

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    {
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > Graph;
        typedef graph_traits<Graph>::vertex_descriptor Vertex;
        typedef writable_property_map_archetype<Vertex, float> CentralityMap;
        typedef readable_property_map_archetype<Vertex, size_t> DistanceMap;
        typedef readable_property_map_archetype<Vertex, DistanceMap> DistanceMatrix;
        typedef distance_measure_archetype<Graph, size_t, float> Measure;

        Graph& g = static_object<Graph>::get();
        CentralityMap cm;
        DistanceMatrix dm;
        Measure m;

        mean_geodesic(g, dm, cm);
        mean_geodesic(g, dm, cm, m);
    }

    {
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > Graph;
        typedef graph_traits<Graph>::vertex_descriptor Vertex;
        typedef readable_property_map_archetype<Vertex, size_t> DistanceMap;
        typedef distance_measure_archetype<Graph, size_t, float> Measure;

        Graph& g = static_object<Graph>::get();
        DistanceMap dm;
        Measure m;

        vertex_mean_geodesic(g, dm);
        vertex_mean_geodesic(g, dm, m);
    }

    {
        typedef vertex_list_graph_archetype<
                undirected_tag,
                allow_parallel_edge_tag
            > Graph;
        typedef graph_traits<Graph>::vertex_descriptor Vertex;
        typedef readable_property_map_archetype<Vertex, float> CentralityMap;
        typedef distance_measure_archetype<Graph, float, float> Measure;

        Graph& g = static_object<Graph>::get();
        CentralityMap cm;
        Measure m;

        graph_mean_geodesic(g, cm);
        graph_mean_geodesic(g, cm, m);
    }

    return 0;
}
