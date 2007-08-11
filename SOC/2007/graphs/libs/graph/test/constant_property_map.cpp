// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/constant_property_map.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/closeness_centrality.hpp>
#include "io.hpp"

using namespace std;
using namespace boost;

// useful types
// number of vertices in the graph
static const unsigned N = 5;

template <typename Graph>
void build_graph(Graph& g)
{
    for(size_t i = 0; i < N; ++i) {
        add_vertex(g);
    }
};


template <typename Graph>
void test()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;

    typedef constant_property_map<Vertex, int> Map;

    Graph g;
    build_graph(g);

    Map m(1);

    VertexIterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        BOOST_ASSERT(m[*i] == 1);
    }
}

int
main(int argc, char *argv[])
{
    typedef undirected_graph<> Graph;
    test<Graph>();
}
