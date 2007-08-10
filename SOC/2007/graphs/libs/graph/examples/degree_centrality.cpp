// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/degree_centrality.hpp>

#include "helper.hpp"

using namespace std;
using namespace boost;

//[declare_social_network
struct Person
{
    string name;
};

typedef undirected_graph<Person> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef exterior_vertex_property<Graph, size_t> CentralityProperty;
typedef CentralityProperty::container_type CentralityContainer;
typedef CentralityProperty::map_type CentralityMap;

typedef map<string, Vertex> VertexMap;
//]

int
main(int argc, char *argv[])
{
    //[setup_social_network
    Graph g;
    map<string, Vertex> verts;
    //]

    //[build_social_network
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

    //[measure_social_network
    // Compute the degree centrality for graph
    CentralityContainer cents(num_vertices(g));
    CentralityMap cm(cents, g);
    degree_centrality(g, cm);
    //]

    //[print_social_network
    // Print the degree centrality of each vertex
    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << g[*i].name << ": " << cm[*i] << "\n";
    }
    //]

    return 0;
}
