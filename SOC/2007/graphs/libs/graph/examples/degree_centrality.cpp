// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "social_network.hpp"
#include "helper.hpp"

#include <iostream>
#include <iomanip>

#include <boost/graph/degree_centrality.hpp>

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    //[setup_social_network
    Graph g;
    map<string, Vertex> verts;
    //]

    // Read in and build the graph
    //[build_social_network
    for(string line; getline(cin, line); ) {
        if(line.empty()) continue;
        size_t index = line.find_first_of(',');
        string first(line, 0, index);
        string second(line, index + 1);

        Vertex u = add_named_vertex(g, first, verts);
        Vertex v = add_named_vertex(g, second, verts);
        add_edge(u, v, g);
    }
    //]

    // Compute the degree centrality for graph
    //[measure_social_network
    CentralityContainer cents(num_vertices(g));
    CentralityMap cm(cents, g);
    degree_centrality(g, cm);
    //]

    // Print the degree centrality of each vertex
    //[print_social_network
    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << setiosflags(ios::left) << setw(12)
             << g[*i].name << cm[*i] << "\n";
    }
    //]

    return 0;
}
