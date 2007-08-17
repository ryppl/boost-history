// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

//[mean_geodesic_example
#include <iostream>
#include <iomanip>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/constant_property_map.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/geodesic_distance.hpp>
#include "helper.hpp"

using namespace std;
using namespace boost;

// The Actor type stores the name of each vertex in the graph.
struct Actor
{
    std::string name;
};

// Declare the graph type and its vertex and edge types.
typedef undirected_graph<Actor> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

// Declare a matrix type and its corresponding property map that
// will contain the distances between each pair of vertices.
typedef exterior_vertex_property<Graph, int> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;

// Declare the weight map so that each edge returns the same value.
typedef constant_property_map<Edge, int> WeightMap;

// Declare a container and its corresponding property map that
// will contain the resulting mean geodesic distances of each
// vertex in the graph.
typedef exterior_vertex_property<Graph, float> GeodesicProperty;
typedef GeodesicProperty::container_type GeodesicContainer;
typedef GeodesicProperty::map_type GeodesicMap;


int
main(int argc, char *argv[])
{
    // Create the graph and read it from standard input.
    Graph g;
    read_graph(g, cin);

    // Compute the distances between all pairs of vertices using
    // the Floyd-Warshall algorithm. Note that the weight map is
    // created so that every edge has a weight of 1.
    DistanceMatrix distances(num_vertices(g));
    DistanceMatrixMap dm(distances, g);
    WeightMap wm(1);
    floyd_warshall_all_pairs_shortest_paths(g, dm, weight_map(wm));

    // Compute the mean geodesic distances for each vertex in
    // the graph.
    GeodesicContainer geodesics(num_vertices(g));
    GeodesicMap gm(geodesics, g);
    mean_geodesic(g, dm, gm);

    // Using the previously computed mean geodesic distances, compute
    // the mean geodesic distance for the entire graph.
    float geo = graph_mean_geodesic(g, gm);

    // Print the mean geodesic distance of each vertex and finally,
    // the graph itself.
    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << setw(12) << setiosflags(ios::left)
             << g[*i].name << get(gm, *i) << endl;
    }
    cout << "mean geodesic distance: " << geo << endl;


    return 0;
}
//]
