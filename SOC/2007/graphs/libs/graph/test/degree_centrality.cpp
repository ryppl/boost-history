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

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/degree_centrality.hpp>

using namespace std;
using namespace boost;

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
    add_edge(v[0], v[1], g);
    add_edge(v[1], v[2], g);
    add_edge(v[2], v[0], g);
    add_edge(v[3], v[4], g);
    add_edge(v[4], v[0], g);
};

template <typename Graph, typename PropertyMap>
void print_map(const Graph& g, PropertyMap pm)
{
    typename Graph::vertex_iterator i, end;
    cout << "{ ";
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << pm[*i] << " ";
    }
    cout << "}\n";
}

template <typename Graph>
void test()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;

    typedef exterior_vertex_property<Graph, unsigned> CentralityProperty;
    typedef typename CentralityProperty::container_type CentralityContainer;
    typedef typename CentralityProperty::map_type CentralityMap;

    Graph g;
    build_graph(g);

    CentralityContainer centralities(num_vertices(g));
    CentralityMap cents(centralities, g);
    degree_centrality(g, cents);

    print_map(g, cents);
}

int
main(int argc, char *argv[])
{
    typedef undirected_graph<> Graph;
    typedef directed_graph<> Digraph;

    cout << "\n*** undirected_graph<> *** \n";
    test<Graph>();

    cout << "\n*** directed_graph<> *** \n";
    test<Digraph>();
}
