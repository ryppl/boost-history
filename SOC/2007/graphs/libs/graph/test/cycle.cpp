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

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/cycle.hpp>
#include <boost/graph/generators/cycle_graph.hpp>
#include <boost/graph/generators/prism_graph.hpp>
#include <boost/graph/generators/complete_graph.hpp>

using namespace std;
using namespace boost;
using namespace __cxxabiv1;

struct cycle_printer : public cycle_visitor
{
    template <typename Path, typename Graph>
    void cycle(const Path& p, const Graph& g)
    {
        cout << "path: ";
        for(typename Path::const_iterator i = p.begin(); i != p.end(); ++i) {
            cout << get(vertex_index, g, *i) << " ";
        }
        cout << "\n";
    };
};

template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;

    static const unsigned N = 5;
    vector<Vertex> v(N);

    // add some vertices
    for(size_t i = 0; i < N; ++i) {
        // v[i] = add_vertex(g);
        v[i] = add_vertex(g);
    }

    // add some edges (with weights)
    add_edge(v[0], v[1], g);
    add_edge(v[1], v[2], g);
    add_edge(v[2], v[0], g);

    add_edge(v[0], v[3], g);
    add_edge(v[3], v[4], g);
    add_edge(v[4], v[0], g);
};

void test_1()
{
    typedef directed_graph<> Graph;

    Graph g;
    // build_graph(g);
    make_prism_graph(g, 4, 3, with_bidirected_cycle(), with_bidirected_spokes());
    // make_complete_graph(g, 4);

    visit_cycles(g, cycle_printer());
    std::cout << "number of triangles: " << count_triangles(g) << "\n";
}

void test_2()
{
    typedef undirected_graph<> Graph;

    Graph g;
    // build_graph(g);
    make_prism_graph(g, 3, 2);

    visit_cycles(g, cycle_printer());
    std::cout << "number of triangles: " << count_triangles(g) << "\n";
}


int
main(int argc, char *argv[])
{
    test_1();
    // test_2();
}
