// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/degree_centrality.hpp>

#include "flow_network.hpp"
#include "helper.hpp"

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

    // Compute the influence and prestige for the graph
    //[compute_influence_prestige
    CentralityContainer influence(num_vertices(g));
    CentralityMap im(influence, g);
    degree_centrality(g, im, measure_influence(g));

    CentralityContainer prestige(num_vertices(g));
    CentralityMap pm(prestige, g);
    degree_centrality(g, pm, measure_prestige(g));
    //]

    // Print the degree centrality of each vertex
    //[print_influence_prestige
    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        Vertex v = *i;
        cout << setiosflags(ios::left) << setw(12)
             << g[v].name << "\t"
             << im[v] << "\t"
             << pm[v] << "\n";
    }
    //]

    return 0;
}
