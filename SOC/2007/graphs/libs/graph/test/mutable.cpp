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

struct Foo
{
    int foo;
};

typedef undirected_graph<Foo> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;

typedef adjacency_list<listS, listS, undirectedS> Test;

int test_main(int, char*[])
{
    Graph g;
    Vertex v[5];
    Edge e[5];

    // simple starting conditions...
    BOOST_CHECK(num_vertices(g) == 0);
    BOOST_CHECK(num_edges(g) == 0);

    // build a couple verts and remove them
    v[0] = add_vertex(g);
    v[1] = add_vertex(g);
    BOOST_CHECK(num_vertices(g) == 2);

    remove_vertex(v[0], g);
    BOOST_CHECK(num_vertices(g) == 1);

    remove_vertex(v[1], g);
    BOOST_CHECK(num_vertices(g) == 0);

    // rebuild the graph, but add some edges this time
    for(int i = 0; i < 5; ++i) v[i] = add_vertex(g);
    
    e[0] = add_edge(v[0], v[1], g).first;
    e[1] = add_edge(v[1], v[2], g).first;
    e[2] = add_edge(v[2], v[3], g).first;
    BOOST_CHECK(num_edges(g) == 3);
    BOOST_CHECK(*edges(g).first == e[0]);

    remove_edge(v[0], v[1], g);		// removes e[0]
    BOOST_CHECK(num_edges(g) == 2);
    BOOST_CHECK(*edges(g).first == e[1]);

    remove_edge(e[1], g);		// removes e[1]
    BOOST_CHECK(num_edges(g) == 1);
    BOOST_CHECK(*edges(g).first == e[2]);

    remove_edge(edges(g).first, g);	// remove e[2]
    BOOST_CHECK(num_edges(g) == 0);


    // build a bunch of edges between two verts
    for(int i = 0; i < 3; ++i) e[i] = add_edge(v[0], v[1], g).first;
    BOOST_CHECK(num_edges(g) == 3);

    remove_edge(v[0], v[1], g);
    BOOST_CHECK(num_edges(g) == 0);

    Test f;
    v[0] = add_vertex(f);
    v[1] = add_vertex(f);
    add_edge(v[0], v[1], f);

    return 0;
}
