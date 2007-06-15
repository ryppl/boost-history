
// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/test/minimal.hpp>
#include <boost/graph/undirected_graph.hpp>

using namespace std;
using namespace boost;

typedef undirected_graph<> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

int test_main(int, char*[])
{
    Graph g;
    Vertex v[5];

    IndexMap x = get(vertex_index, g);

    // build up the graph
    for(int i = 0; i < 5; ++i) {
	v[i] = add_vertex(g);
    }

    // after the first build, we should have these conditions
    BOOST_CHECK(max_vertex_index(g) == 5);
    for(int i = 0; i < 5; ++i) {
	BOOST_CHECK(get_vertex_index(v[i], g) == i);
    }

    // remove some vertices and re-add them...
    for(int i = 0; i < 5; ++i) remove_vertex(v[i], g);
    BOOST_CHECK(num_vertices(g) == 0);

    for(int i = 0; i < 5; ++i) {
	v[i] = add_vertex(g);
    }

    // before renumbering, our vertices should be off by
    // about 5...
    BOOST_CHECK(max_vertex_index(g) == 10);
    for(int i = 0; i < 5; ++i) {
	BOOST_CHECK(get_vertex_index(v[i], g) == 5 + i);
    }

    // renumber them.
    renumber_vertex_indices(g);

    // and we should be back to the initial condition
    BOOST_CHECK(max_vertex_index(g) == 5);
    for(int i = 0; i < 5; ++i) {
	BOOST_CHECK(get_vertex_index(v[i], g) == i);
    }

    return 0;
}
