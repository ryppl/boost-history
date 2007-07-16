// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <tr1/unordered_map>
#include <typeinfo>
#include <cxxabi.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/constant_property_map.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;
using namespace __cxxabiv1;

template <typename T>
const char *
typename_of(const T&)
{
    return __cxa_demangle(typeid(T).name(), 0, 0, 0);
}

template <typename T>
const char *
typename_of()
{
    return __cxa_demangle(typeid(T).name(), 0, 0, 0);
}

template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;

    static const unsigned N = 5;
    vector<Vertex> v(N);
    vector<Edge> e;

    // add some vertices
    for(size_t i = 0; i < N; ++i) {
        // v[i] = add_vertex(g);
        v[i] = add_vertex(g);
    }

    // add some edges (with weights)
    e.push_back(add_edge(v[0], v[1], g).first);
    e.push_back(add_edge(v[1], v[2], g).first);
    e.push_back(add_edge(v[2], v[0], g).first);
    e.push_back(add_edge(v[3], v[4], g).first);
    e.push_back(add_edge(v[4], v[0], g).first);
};

void test_1()
{
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;

    typedef exterior_vertex_property<Graph, int> DistanceProperty;
    typedef DistanceProperty::container_type DistanceContainer;
    typedef DistanceProperty::map_type DistanceMap;
    typedef constant_property_map<Edge, int> WeightMap;

    Graph g;
    build_graph(g);

    DistanceContainer distances(num_vertices(g));
    DistanceMap dists(make_property_map(distances));
    WeightMap weights(1);

    dijkstra_shortest_paths(g, *vertices(g).first,
                            weight_map(weights).
                            distance_map(dists));

    copy(distances.begin(), distances.end(), ostream_iterator<int>(cout, " "));
    cout << "\n";
}

void test_2()
{
    typedef undirected_graph<> Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;

    typedef exterior_vertex_property<Graph, int> DistanceProperty;
    typedef DistanceProperty::container_type DistanceContainer;
    typedef DistanceProperty::map_type DistanceMap;

    typedef constant_property_map<Edge, int> WeightMap;

    Graph g;
    build_graph(g);

    DistanceContainer distances(num_vertices(g));
    DistanceMap dists(make_property_map(distances));
    WeightMap weights(1);

    dijkstra_shortest_paths(g, *vertices(g).first,
                            weight_map(weights).
                            distance_map(dists));

    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << distances[*i] << " ";
    }
    cout << "\n";
}


int
main(int argc, char *argv[])
{
    test_1();
    test_2();
}
