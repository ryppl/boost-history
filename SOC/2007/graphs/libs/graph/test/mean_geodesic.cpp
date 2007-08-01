// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <tr1/unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/geodesic_distance.hpp>

using namespace std;
using namespace boost;

template <typename T>
struct numeric
{
    numeric(T x) : value(x) { }
    T value;
};

template <typename Value>
numeric<Value>
make_numeric(Value x)
{ return numeric<Value>(x); }

template <typename Value>
ostream& operator <<(ostream& os, const numeric<Value>& x)
{
    if(x.value == numeric_values<Value>::infinity()) {
        os << "i";
    }
    else {
        os << x.value;
    }
    return os;
}

struct VertexProp
{
    int dummy;
};

struct EdgeProp
{
    int weight;
};

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

    g[e[0]].weight = 1;
    g[e[1]].weight = 1;
    g[e[2]].weight = 1;
    g[e[3]].weight = 1;
    g[e[4]].weight = 1;
};

template <typename Graph, typename PropertyMap>
void print_map(const Graph& g, PropertyMap pm)
{
    typename Graph::vertex_iterator i, end;
    cout << "{ ";
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << make_numeric(pm[*i]) << " ";
    }
    cout << "}\n";
}

template <typename Graph, typename Matrix>
void print_matrix(const Graph& g, Matrix m)
{
    cout << "[\n";
    typename Graph::vertex_iterator i, j, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        print_map(g, m[*i]);
    }
    cout << "]\n";
}

template <typename Graph>
void test()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;

    typedef exterior_vertex_property<Graph, float> GeodesicProperty;
    typedef typename GeodesicProperty::container_type GeodesicContainer;
    typedef typename GeodesicProperty::map_type GeodesicMap;

    typedef exterior_vertex_property<Graph, int> DistanceProperty;
    typedef typename DistanceProperty::matrix_type DistanceMatrix;

    typedef typename property_map<Graph, int EdgeProp::*>::type WeightMap;

    Graph g;
    build_graph(g);

    GeodesicContainer geodesics(num_vertices(g));
    GeodesicMap geo(geodesics);
    DistanceMatrix dist(num_vertices(g));
    WeightMap weights(get(&EdgeProp::weight, g));

    floyd_warshall_all_pairs_shortest_paths(g, dist, weight_map(weights));
    mean_geodesic(g, dist, geo);

    print_matrix(g, dist);
    print_map(g, geo);

    std::cout << "mgeo: " << make_numeric(graph_mean_geodesic(g, dist)) << "\n";

}

int
main(int argc, char *argv[])
{
    typedef undirected_graph<VertexProp, EdgeProp> Graph;
    typedef directed_graph<VertexProp, EdgeProp> Digraph;

    cout << "\n*** undirected_graph<> *** \n";
    test<Graph>();

    cout << "\n*** directed_graph<> *** \n";
    test<Digraph>();
}
