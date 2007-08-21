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
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <boost/graph/generators/prism_graph.hpp>
#include <boost/graph/generators/complete_graph.hpp>

using namespace std;
using namespace boost;

template <typename OutStream>
struct clique_printer : public clique_visitor
{
    clique_printer(OutStream& os)
        : mOs(os)
    { }

    template <typename VertexSet, typename Graph>
    inline void
    clique(const VertexSet& vs, Graph& g)
    {
        mOs << "clique: ";
        typename VertexSet::const_iterator i, end = vs.end();
        for(i = vs.begin(); i != end; ++i) {
            mOs << get(vertex_index, g, *i) << " ";
        }
        mOs << std::endl;
    }

    OutStream& mOs;
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

template <typename Graph>
void test()
{
    Graph g;

    // build_graph(g);
    // make_prism_graph(g, 3, 2);
    make_complete_graph(g, 4);

    bron_kerbosch_all_cliques(g, clique_printer<ostream>(cout));
}

int
main(int argc, char *argv[])
{
    typedef undirected_graph<> Graph;
    typedef directed_graph<> DiGraph;

    cout << "*** undirected ***\n";
    test<Graph>();

    cout << "*** directed ***\n";
    test<DiGraph>();
}
