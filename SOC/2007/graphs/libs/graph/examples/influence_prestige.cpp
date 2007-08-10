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

#include "helper.hpp"

using namespace std;
using namespace boost;

//[declare_information_network
struct Person
{
    string name;
};

typedef directed_graph<Person> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef exterior_vertex_property<Graph, size_t> CentralityProperty;
typedef CentralityProperty::container_type CentralityContainer;
typedef CentralityProperty::map_type CentralityMap;

typedef map<string, Vertex> VertexMap;
//]

int
main(int argc, char *argv[])
{
    //[setup_information_network
    Graph g;
    map<string, Vertex> verts;
    //]

    //[build_information_network
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
    //]

    //[measure_information_network
    // Compute the influence and prestige for the graph
    CentralityContainer influence(num_vertices(g));
    CentralityMap im(influence, g);
    degree_centrality(g, im, measure_influence(g));

    CentralityContainer prestige(num_vertices(g));
    CentralityMap pm(prestige, g);
    degree_centrality(g, pm, measure_prestige(g));
    //]

    //[print_information_network
    // Print the degree centrality of each vertex
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
