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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/clustering_coefficient.hpp>

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


void test_1()
{
    typedef undirected_graph<> Graph;

    Graph g;
    build_graph(g);

    Graph::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cerr << get_vertex_index(*i, g) << "\n";
        cerr << "  triples: " << num_centered_triples(g, *i) << "\n";
        cerr << "  triangles: " << num_centered_triangles(g, *i) << "\n";
        cerr << "  coef: " << clustering_coefficient(g, *i) << "\n\n";
    }
    cerr << "cc: " << clustering_coefficient(g) << "\n";
}

void test_2()
{
    typedef directed_graph<> Graph;

    Graph g;
    build_graph(g);

    Graph::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cerr << get_vertex_index(*i, g) << "\n";
        cerr << "  triples: " << num_centered_triples(g, *i) << "\n";
        cerr << "  triangles: " << num_centered_triangles(g, *i) << "\n";
        cerr << "  coef: " << clustering_coefficient(g, *i) << "\n\n";
    }
    cerr << "cc: " << clustering_coefficient(g) << "\n";
}

int
main(int argc, char *argv[])
{
    test_1();
    test_2();
}
