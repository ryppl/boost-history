// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "social_network.hpp"
#include "helper.hpp"

#include <iostream>
#include <iomanip>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/eccentricity.hpp>

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    Graph g;
    map<string, Vertex> verts;

    // Read in and build the graph
    for(string line; getline(cin, line); ) {
        if(line.empty()) continue;
        size_t index = line.find_first_of(',');
        string first(line, 0, index);
        string second(line, index + 1);

        Vertex u = add_named_vertex(g, first, verts);
        Vertex v = add_named_vertex(g, second, verts);
        add_edge(u, v, g);
    }

    DistanceMatrix distances(num_vertices(g));
    DistanceMatrixMap dm(distances, g);
    WeightMap wm(1);
    floyd_warshall_all_pairs_shortest_paths(g, dm, weight_map(wm));

    // Compute the degree centrality for graph
    //[compute_eccentricity
    EccentricityContainer eccs(num_vertices(g));
    EccentricityMap em(eccs, g);
    eccentricity(g, dm, em);
    int radius = graph_radius(g, em);
    int diameter = graph_diameter(g, em);
    //]

    //[print_radius_diameter
    cout << setw(10) << setiosflags(ios::left) << "radius" << radius << "\n";
    cout << setw(10) << setiosflags(ios::left) << "diameter" << diameter << "\n";
    //]

    return 0;
}
